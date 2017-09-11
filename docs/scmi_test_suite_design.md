**SCMI Test Suite - Design Overview**
=======================================

Table of Contents:
- [Introduction](#introduction)
- [Design Goals](#design-goals)
- [Design Overview](#design-overview)
  * [Components](#components)
  * [Interfaces](#interfaces)
- [Code Structure](#code-structure)
- [Detailed Design](#detailed-design)
  * [Test execution flow for a standard test case](#standard-test-case)
  * [Test execution flow for a discovery driven test case](#discovery-driven-test-case)
  * [Test execution flow for a test having pre-condition test case defined](#test-define-with-pre-condition-test-case)

Introduction
-------
The SCMI test suite delivers a library of tests that are focused on validating the implementation of the SCMI protocol on a platform.
This document presents an overview of the framework and design of SCMI test suite.

Design goals
-------
The key design goals of the test suite are:
- The platform details are presented to the suite by a set of interfaces that must be implemented by the user. This abstraction allows platform-specific details such as transport layer, to be independent from the test design.
- The current design also allows for selective building of test libraries to enable testing on memory constrained platforms.
- The design also allows the test suite to be extended to have custom protocols and custom commands which are user-defined.

Design overview
-------
The SCMI test suite is divided into components and interfaces.

### Components

There are three key components:

`Test Agent`: This is the generic framework component which prepares an execution context and executes the test suites. This component has all the generic test execution logic that is leveraged by the rest of the test suite components.

`Protocol`: This has the protocol-specific source code and test library for the given protocol.

`Platform`: This has the platform-specific source code which implements the defined interfaces needed by the test suite. The user can specify the expected values and the agent characteristics here, and provide as an input to the test suite entry function.


### Interfaces
Interfaces must be implemented by the user to enable the suite to communicate with the platform.

The following interface is used by the test agent to send an SCMI command to the platform and receive the response.
> `int arm_scmi_send_message(uint32_t message_header_send, size_t parameter_count, const uint32_t *parameters, uint32_t *message_header_rcv, int32_t *status, size_t *return_values_count, uint32_t *return_values)`

The following interface is used by the test agent to receive either a delayed response or notification from the platform.
> `int arm_scmi_wait_for_response(uint32_t *message_header_rcv, int32_t *status, size_t *return_values_count, uint32_t *return_values, bool *message_ready, const uint32 timeout)`

The following interface is used by the test agent to dump the test execution output.
> `void arm_scmi_log_output(const char* output)`

Code Structure
-------
The source code of the test suite code is structured as below:

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
This section provides a detailed description of the design and its various components.

### Discovery driven
The key approach that is taken in the design of this test suite is to enable **discovery driven** test execution. <br>
The test suite provides the ability to package individual test library for each protocol at build time excpet for Base protocol. Base protocol provides the key discovery capability at the protocol level, which is used by the test suite to execute the respective test library, if present in the package.

Each test suite is a collection of test cases. The test library is structured as a collection of test suites per protocol. <br>
The first test suite in any protocol **must** contain a collection of test cases that target PROTOCOL\_VERSION, PROTOCOL\_ATTRIBUTES and PROTOCOL\_MESSAGE\_ATTRIBUTES SCMI commands. This information is used in the second level of discovery to know what SCMI commands are implemented by the platform. <br>
The remaining test suite is organized on a per-command basis. A majority of the collection of test cases target a single command. Each test suite provides a provision to specify a post condition and pre condition test case. For some suites, it is possible that a pre condition for a GET test is a SET test and in those situations there are multiple commands within a single test suite. <br> <br>

### Execution context
The test agent maintains a test execution context (in arm\_scmi\_test\_case structure). This encapsulates all the information that is required to represent a test case and execute it. The key elements within an execution context are:
 - `testcase_id`: unique test case identifier that follows the defined naming convention.
 - `protocol_id`: indicates the protocol that this test case targets. This is packaged into the SCMI message header which is passed to the platform.
 - `message_id`: indicates the message that this test case targets. This is packaged into the SCMI message header which is passed to the platform.
 - `parameter_discovery_driven`: indicates whether the test case must derive the parameters for the SCMI command based on discovery or test execution custom handler logic.
 - `send_cmd_as_async`: indicates that the command is sent with Async flag set if the flag is available for the command, and wait for the delayed response. The test case completes on receiving a response or until timeout expiry.
 - `timeout_in_sec`: is the timeout value for the test execution context when waiting for a delayed response or notification.
 - `invalid_paramters`: indicates that the custom test execution handler must generate an invalid input for the indexed parameter for the scmi command based on the already discovered information.
 - `parameter_count`: indicates the number of parameters (number of uint32_t values passed through the parameters member) for the scmi command under test. This is ignored if parameter\_discovery\_driven flag is set. In such a case, a custom execution handler fills the correct paramter_count.
 - `parameters`: contains a stream of uint32_t items corresponding to the input paramters for the command. The parameter_count indicates the number of elements in the stream.
 - `status`: specifies the expected return status on command completion. A special return value of SCMI\_STATUS\_NOT\_SPECIFIED can be used in scenarios where the return status cannot be pre-determined.
 - `expected_returns`: defines the set of the return values in terms of their type and any metadata for those return values. 
 - `get_return_values_function`: points to a custom handler that fills the required expected_returns for the given test case. If this is NULL, then the generic execution handler handles this.
 - `custom_tester_function`: points to a custom handler that has the special execution logic required to execute the given test case. If this is NULL, then the generic execution handler handles the test case execution.
 - `post_processing_function`: points to a custom handler that performs any test case-specific post processing. If this is NULL, then the generic execution handler handles this.

### Test execution flow

The following sections describe the flow of execution for a standard test, for a discovery driven test, and for a test where a pre-condition test is defined.

#### Standard test case

[base]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_vendor\_disc\_cmd\_support-01 is an example of a standard test case.

`Step 1`: Test library entry function arm\_scmi\_test\_agent\_execute() (in test\_agent/test\_executor.c) unconditionally executes arm\_scmi\_base\_execute() (in protocols/base/base\_exe.c) to discover the supported protocols on the platform. Optionally, the expected return values structure is initialized with the values passed by user through arm\_scmi\_platform\_info structure.

`Step 2`: Setup the test execution context for base protocol. The setup\_base() (in protocols/base/base\_exe.c) initializes the test execution context with all the test suites and the test cases packaged within those suites. Initally, the only test suite marked for execution is TS\_BASE\_DISCOVERY, which determines the commands that are supported in the platform. Based on this, subsequent test suites are enabled. This is a runtime decision by the per protocol 'discovery' test suite.

`Step 3`: Invoke arm\_scmi\_run() (in test_agent/test\_execute.c) which is the central function that is responsible for the execution of the test execution context. It iterates through all the test suites present in the execution context and sequentially executes test cases within each test suite.

`Step 3a`: If a get\_return\_values\_func function is defined for a test case, it is executed.

`Step 3b`: If a custom\_tester\_func function is defined for a test case, then the execution is handled by that custom function. Otherwise, the test case execution is handled by the generic arm\_scmi\_execute\_and\_assert\_test\_case() (in test\_agent/test\_execute.c). This function frames the scmi message and invokes the arm\_scmi\_send\_message() function that is implemented by the platform code, and asserts the response it gets from this invocation.

`Step3c`: If a post\_processing\_func function is defined for a test case, then the post processing after test execution is handled by this.

`Step4`: Update test report status and repeat.

#### Discovery driven test case

An example of a discovery driven test case is [clock]-{CLOCK\_ATTRIBUTES}-query\_clock\_attributes\_allclockid-01.

`Step 1`: Test library entry function arm\_scmi\_test\_agent\_execute() (in test\_agent/test\_executor.c) unconditionally executes arm\_scmi\_base\_execute() (in protocols/base/base\_exe.c) to discover the supported protocols on the platform. If the clock protocol test library was selected at build time to be part of the test suite, it executes arm\_scmi\_clock\_execute() (in protocols/clock/clock\_exe.c).

`Step 2`: Setup the test execution context for clock protocol. <br>
The function setup\_clock() (in protocols/clock/clock\_exe.c) initializes the test execution context with all the test suites and test cases packaged in those test suites. At this initialization phase, the only test suite marked for execution is TS\_CLOCK\_DISCOVERY, which determines the supported commands in the platform. Based on this, the subsequent test suites are enabled. This is a runtime decision made based on the per protocol 'discovery' test suite.

`Step 3`: Invoke arm\_scmi\_run() (in test_agent/test\_execute.c) which is the central function that is responsible for the execution of the test execution context. It iterates through all the test suites present in the execution context and sequentially executes test cases within each test suite.

`Step 3a`: If a get\_return\_values\_func function is defined for a test case, then that is executed as a test preparation.

`Step 3b`: If a custom\_tester\_func function is defined for a test case then the execution is handled by that custom function. In the case of this example, arm\_scmi\_custom\_tester\_clock\_attributes() (in protocols/clock/clock\_exe.c) is defined as the custom\_tester\_func. This custom function handles both positive and negative test case execution logic specific to the targeted SCMI command.

`Step 3c`: If a post\_processing\_func function is defined for a test case, then the post processing after test execution is handled by this.

`Step 4`: Update test report status and repeat.

Note: In case of discovery driven test cases, the runtime extracted outputs from previously executed test commands are saved in a runtime tracker variable and used as an input in such test cases. In this specific case, the number of clock outputs from the PROTOCOL\_ATTRIBUTES command of clock protocol is used in the arm\_scmi\_custom\_tester\_clock\_attributes function to extract the attributes of all the available clocks visible to the agent from the platform.

#### Test define with pre-condition test case

Examples of test cases defined with preconditions are [performance]-{PERFORMANCE\_LEVELS\_SET}-percondition\_performance\_levels\_get\_alldomainid-01 and [performance]-{PERFORMANCE\_LEVEL\_GET}-performance\_levels\_get\_alldomainid-01.

`Step 1`: Test library entry function arm\_scmi\_test\_agent\_execute() (in test\_agent/test\_executor.c) unconditionally executes arm\_scmi\_base\_execute() (in protocols/base/base\_exe.c) to discover the supported protocols on the platform. If the performance test library was selected at build time to be part of the test suite, it executes arm\_scmi\_performance\_execute() (in protocols/performance/performance\_exe.c).

`Step 2`: Setup the test execution context for performance protocol. The setup\_performance() (in protocols/performance/performance\_exe.c) initializes the test execution context with all the test suites and test cases packaged in those test suites. At this initialization phase, the only test suite marked for execution is TS\_PERFORMANCE\_DISCOVERY which determines the commands that are supported in the platform. Based on this, the respective test suites are enabled. This is a runtime decision that is made based on the per protocol 'discovery' test suite.

`Step 3`: Invoke arm\_scmi\_run() (in test_agent/test\_execute.c) which is the central function responsible for the execution of the test execution context. It iterates through all the test suites present in the execution context and sequentially executes test cases within each test suite.

`Step 3a`: If a get\_return\_values\_func function is defined for a test case, it is executed.

`Step 3b`: If a custom\_tester\_func function is defined for a test case, then the execution is handled by that custom function. In this example, both the test cases have arm\_scmi\_custom\_test\_performance\_levels\_set() and arm\_scmi\_custom\_test\_performance\_levels\_get() (both in protocols/performance/performance\_exe.c) are defined as the custom functions respectively. For the test case which has precondition\_indicator set as PRECOND\_DEFINE, the arm\_scmi\_execute\_and\_assert\_test\_case() (in test\_agent/test\_execute.c) saves the test case and SCMI command execution status in a runtime tracker variable. This information is used in the test case that has percondition_indicator set as PRECOND\_USE, where the decision to skip or execute this test case is derived based on the saved status of PRECOND\_DEFINE test case status.

`Step 3c`: If a post\_processing\_func function is defined for a test case, then the post processing after test execution is handled by this.

`Step 4`: Update test report status and repeat.

Note: The runtime tracking of a precondition test status is reset at the beginning of every test suite. This means that the PRECOND\_DEFINE status has effect only within a test suite. However, a PRECOND\_DEFINE test case can be redefined within a test suite by a new test case and also, multiple test cases within a test suite can use the previously defined precondition status.

- - - - - - - - - - - - - - - - - - - -

_Copyright (c) 2017, Arm Limited and Contributors. All rights reserved_
