**SCMI Test Suite - Design Overview**
=======================================

Table of Contents:
- [Introduction](#introduction)
- [Design Aim and Considerations](#design-aim-and-considerations)
- [Key Components and Interfaces](#key-components-and-interfaces)
  * [Components](#components)
  * [Interfaces](#interfaces)
- [Code Structure](#code-structure)
- [Detailed Design](#detailed-design)
  * [Test execution flow for a standard test case](#test-execution-flow-for-a-standard-test-case)
  * [Test execution flow for a discovery driven test case](#test-execution-flow-for-a-discovery-driven-test-case)
  * [Test execution flow for a test having pre-conditon test case defined](#test-execution-flow-for-a-test-having-pre-condition-test-case-defined)

Introduction
-------
This outlines the overview and approach took in the design of SCMI test suite.

Design Aim and Considerations
-------
The SCMI test suite delivers a library of tests focused on validating the implementation of SCMI protocol by the platform code. The design enables platform adaptation with a set of clearly defined interfaces the user will implement to use on their own platform. This abstraction hides platform specific details like transport layer from the generic design. The current design allows for selective building of test library to enable testing on memory constrained platforms. The design allows extension of the test suite to accommodate custom protocols and custom commands as defined by the user.

Key Components and Interfaces
-------
The SCMI test suite has 3 key components and some interfaces defined to be implemented by the user to adapt it for their platform.

### Components

`Test Agent Component`: This is the generic framework component which prepares an execution context and performs the execution. This component has all the generic test execution logic leveraged by the rest of the test suite components.

`Protocol Component`: This has the protocol specific execution logic and test library for the given protocol.

`Platform Component`: This has the adaptation logic implementing all the defined interfaces needed by the test suite. The user can specify the expected values and the agent characteristics here and provide as an input to the test suite entry function.

### Interfaces

The below interface is used by the test agent component to send a composed scmi command to the platform and receive the response.
> `int arm_scmi_send_message(uint32_t message_header_send, size_t parameter_count, const uint32_t *parameters, uint32_t *message_header_rcv, int32_t *status, size_t *return_values_count, uint32_t *return_values)`

The below interface is used by the test agent component to receive either a delayed response or notification from the platform.
> `int arm_scmi_wait_for_response(uint32_t *message_header_rcv, int32_t *status, size_t *return_values_count, uint32_t *return_values, bool *message_ready, const uint32 timeout)`

The below interface is used by the test agent component to dump output from the test execution.
> `void arm_scmi_log_output(const char* output)`

Code Structure
-------
The scmi test suite code is structured as below:

```
scmi-tests
	|___test_agent
	|
	|   Logic for the test agent component and generic test execution handling
	|
	|___protocols
	|	|
	|	|___base
	|	|
	|	|   Base protocol test execution logic and test library
	|	|
	|	|___clock
	|	|
	|	|   Clock protocol test execution logic and test library
	|	|
	|	|___<other protocols>
	|
	|	    Per protocol custom test execution logic and test library including any custom protocols and custom commands
	|
	|
	|___platform
		|
		|___arm
		|	|
		|	|___juno
		|	|
		|	|   Test agent from linux userspace using mailbox test driver interfaces and juno platform expected values
		|	|
	 	|	|___<other arm platforms>
		|
		|
		|
		|___self_test
			|
			|___mocker

			    Test agent running on host machine for mock testing test suite implementation
```

Detailed Design
-------
This section provides more details on the current design of the test suite and its various components.

The key approach taken in the design of this test suite is to enable **discovery driven** test execution. Since the test suite provides the ability to package individual test library for each protocol at build time, Base protocol is expected to be packaged always. Base protocol provides the key discovery capability at the protocol level, where the test suite knows what protocols are supported in the platform based on which the respective test libraries to be executed provided they are packaged at build time.

The test library is structured as a collection of test suites per protocol. Each test suite is a collection of test cases. The first tests suite in any protocol **must** be containing a collection of test cases targeting PROTOCOL\_VERSION, PROTOCOL\_ATTRIBUTES and PROTOCOL\_MESSAGE\_ATTRIBUTES scmi commands which provides the second level of discovery where the test suite know what scmi commands are implemented in the platform. The remaining of the test suite are organised on a per-command basis where the collection of tests cases targets typically a single command. Each test suite provides a provision to specify a post condition and pre condition test case, and in this case it is possible that a pre condition test case for a GET test is a SET test and in those situations we have a mix of commands in a single test suite.

The test agent maintains a test execution context (in arm\_scmi\_test\_case structure). This encapsulates all the info needed to represent a test case and execute it. The key elements in the test execution context are:
 - `testcase_id`: unique test case identifier following the agreed naming convention
 - `protocol_id`: indicating which protocol this test case targets. This will be packaged into the scmi message header which is passed to the platform.
 - `message_id`: indicating which message this test case targets. This will be packaged into the scmi message header which is passed to the platform.
 - `parameter_discovery_driven`: used to indicate whether the test case need to derive the parameters for the scmi command based on discovery or test execution custom handler logic. For such cases it is a must to have custom test execution handler logic present.
 - `send_cmd_as_async`: indicates the command needs to be sent with Async flag being set if available for the command and wait for the delayed response till a given timeout to complete this test case execution.
 - `timeout_in_sec`: is the timeout value for the test execution context to wait for a delayed response or notification corresponding to the command issues in asynchronous mode.
 - `invalid_paramters`: indicates that the custom test execution handler will have to generate invalid input for the indexed parameter for the scmi command based on the already discovered information.
 - `parameter_count`: indicates the number of parameters (number of uint32_t values passed via the parameters member, to be specific) for the scmi command under test. This will be ignored if parameter\_discovery\_driven flag is set which would have a custom execution handler that will fill the correct paramter_count.
 - `parameters`: indicates the stream of uint32_t items corresponding to the input paramters for the command. The parameter_count is used as the limiting value to seek into elements of this member.
 - `status`: indicates the expected return status once the framed command for this test case is executed. A special return value of SCMI\_STATUS\_NOT\_SPECIFIED is used to indicate situations when a definite status value cannot be expected.
 - `expected_returns`: indicates the definition of the return values in terms of what type they are and any metadata for those return values for test reporting.
 - `get_return_values_function`: points to a custom handler which fills the required expected returns for the given test case. If this is NULL then the generic execution handler will handle this.
 - `custom_tester_function`: points to a custom handler which has the special execution logic needed to execute the given test case. If this is NULL then the generic execution handler will handle the test case execution.
 - `post_processing_function`: points to a custom handler which will perform any test case specific post processing. If this is NULL then the generic execution handler will handle this.

### Test execution flow for a standard test case

[base]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_vendor\_disc\_cmd\_support-01 is an example of a standard test case.

`Step1`: Test library entry function arm\_scmi\_test\_agent\_execute() (in test\_agent/test\_executor.c) unconditionally executes arm\_scmi\_base\_execute() (in protocols/base/base\_exe.c) to discover the supported protocols on the platform. If expected values for base protocol is passed by user via arm\_scmi\_platform\_info structure then initialise the expected return values structure.

`Step2`: Setup the test execution context for base protocol. The setup\_base() (in protocols/base/base\_exe.c) will initialise the test execution context with all the test suites and the test cases packaged in those test suites for execution. At this initialisation phase the only test suite marked for execution is TS\_BASE\_DISCOVERY which will determine which commands are supported in the platform and based on that will mark subsequent test suites enabled for testing. This is a runtime decision made based on the per protocol 'discovery' test suite.

`Step3`: Invoke arm\_scmi\_run() (in test_agent/test\_execute.c) which is the central function performing the execution of the test execution context. Iterate through all test suites from the test execution context and execute each test case from the test suite one after then other.

`Step3a`: If a get\_return\_values\_func function is defined for a test case, then that is executed as a test preparation.

`Step3b`: If a custom\_tester\_func function is defined for a test case then the execution is handled by that custom function. Otherwise the test case execution is handled by the generic arm\_scmi\_execute\_and\_assert\_test\_case() (in test\_agent/test\_execute.c). This function frames the scmi message and invokes the arm\_scmi\_send\_message() function implemented by the platform adaptation code, and asserts the response it gets from this invocation.

`Step3c`: If a post\_processing\_func function is defined for a test case then the post processing after test execution is handled by this.

`Step4`: Update test report status and repeat.

### Test execution flow for a discovery driven test case

[clock]-{CLOCK\_ATTRIBUTES}-query\_clock\_attributes\_allclockid-01 is an example of a discovery driven test case.

`Step1`: Test library entry function arm\_scmi\_test\_agent\_execute() (in test\_agent/test\_executor.c) unconditionally executes arm\_scmi\_base\_execute() (in protocols/base/base\_exe.c) to discover the supported protocols on the platform. If the clock protocol test library was selected at build time to be part of the test suite it executes arm\_scmi\_clock\_execute() (in protocols/clock/clock\_exe.c).

`Step2`: Setup the test execution context for clock protocol. The setup\_clock() (in protocols/clock/clock\_exe.c) will initialise the test execution context with all the test suites and test cases packaged in those test suites for execution. At this initialisation phase the only test suite marked for execution is TS\_CLOCK\_DISCOVERY which will determine which commands are supported in the platform and based on that will mark the subsequent test suites enabled for testing. This is a runtime decision made based on the per protocol 'discovery' test suite.

`Step3`: Invoke arm\_scmi\_run() (in test\_agent/test\_execute.c) which is the central function performing the execution of the test execution context. Iterate through all test suites from the test execution context and execute each test case from the test suite one after the other.

`Step3a`: If a get\_return\_values\_func function is defined for a test case, then that is executed as a test preparation.

`Step3b`: If a custom\_tester\_func function is defined for a test case then the execution is handled by that custom function. In the case of this example arm\_scmi\_custom\_tester\_clock\_attributes() (in protocols/clock/clock\_exe.c) is defined as the custom\_tester\_func. This custom function handles both positive and negative test case execution logic specific to the targeted SCMI command.

`Step3c`: If a post\_processing\_func function is defined for a test case then the post processing after test execution is handled by this.

`Step4`: Update test report status and repeat.

Note: In case of discovery driven test cases, the runtime extracted outputs from previously executed test commands are save in a runtime tracker variable and is used a an input in such test cases. In this specific case the number of clocks output from the PROTOCOL\_ATTRIBUTES command of clock protocol is used in the arm\_scmi\_custom\_tester\_clock\_attributes function to extract the attributes of all the available clocks visible to the agent from the platform.

### Test execution flow for a test having pre-condition test case defined

[performance]-{PERFORMANCE\_LEVELS\_SET}-percondition\_performance\_levels\_get\_alldomainid-01 and [performance]-{PERFORMANCE\_LEVEL\_GET}-performance\_levels\_get\_alldomainid-01 are examples of test cases defining and using preconditions.

`Step1`: Test library entry function arm\_scmi\_test\_agent\_execute() (in test\_agent/test\_executor.c) unconditionally executes arm\_scmi\_base\_execute() (in protocols/base/base\_exe.c) to discover the supported protocols on the platform. If the performance test library was selected at build time to be part of the test suite it executes arm\_scmi\_performance\_execute() (in protocols/performance/performance\_exe.c).

`Step2`: Setup the test execution context for performance protocol. The setup\_performance() (in protocols/performance/performance\_exe.c) will initialise the test execution context with all the test suites and test cases packaged in those test suites for execution. At this initialisation phase the only test suite marked for execution is TS\_PERFORMANCE\_DISCOVERY which will determine which commands are supported in the platform and based on that will mark the subsequent test suite enabled for testing. This is a runtime decision made based in the per protocol 'discovery' test suite.

`Step3`: Invoke arm\_scmi\_run() (in test\_agent/test\_execute.c) which is the central function performing the execution of the test execution context. Iterate through all test suites from the test execution context and execute each test case from the test suite one after the other.

`Step3a`: If a get\_return\_values\_func function is defined for a test case, then that is executed as a test preparation.

`Step3b`: If a custom\_tester\_func function is defined for a test case then the execution is handled by that custom function. In the case of this example, both the test cases have arm\_scmi\_custom\_test\_performance\_levels\_set() and arm\_scmi\_custom\_test\_performance\_levels\_get() (both in protocols/performance/performance\_exe.c) are defined as the custom functions respectively. For the test case which has precondition\_indicator set as PRECOND\_DEFINE, the arm\_scmi\_execute\_and\_assert\_test\_case() (in test\_agent/test\_execute.c) saves the test case and scmi command execution status in a runtime tracker variable. This information is used in the test case which has got percondition_indicator set as PRECOND\_USE, where the decision to skip or execute this test case is derived based on the saved status of PRECOND\_DEFINE test case status.

`Step3c`: If a post\_processing\_func function is defined for a test case then the post processing after test execution is handled by this.

`Step4`: Update test report status and repeat.

Note: The runtime tracking of a precondition test status is reset at the begining of every test suite, meaning the PRECOND\_DEFINE status has effect only within a test suite. However a PRECOND\_DEFINE test case can be redefined within a test suite by a new test case and also multiple test cases within a test suite can use the previously defined precondition status.

- - - - - - - - - - - - - - - - - - - -

_Copyright (c) 2017, ARM Limited and Contributors. All rights reserved_
