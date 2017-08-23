**SCMI Test Suite User Guide**
==============================

Table of Contents:

- [Introduction](#introduction)
- [Host Machine Requirements and Tools](#host-machine-requirements-and-tools)
- [Getting SCMI Test Suite source code](#getting-scmi-test-suite-source-code)
- [Understanding the SCMI Test Suite design and library of tests](#understanding-the-scmi-test-suite-design-and-library-of-tests)
- [Building the SCMI Test Suite](#building-the-scmi-test-suite)
  * [Build the test suite as a library](#build-the-test-suite-as-a-library)
  * [Build for self test mocker platform](#build-for-self-test-mocker-platform)
  * [Build for arm juno platform](#build-for-arm-juno-platform)
- [Executing the test suite](#executing-the-test-suite)
  * [Running the test agent on host machine](#running-the-test-agent-on-host-machine)
  * [Running the test agent on juno platform](#running-the-test-agent-on-juno-platform)
- [Test Execution Report](#test-execution-report)

Introduction
-------

This document outlines how to fetch, adapt and build SCMI test suite. Examples to build and run the test suite for a 'self test mocker' platform on host machine and Juno ARM Development Platform (ADP) is provided as reference.

Host Machine Requirements and Tools
-------

The software has been built on Ubuntu 14.04 LTS (64-bit). Packages used for building the software were installed from that distribution unless otherwise specified. Eventhough we have tested this only on Ubuntu 14.04 we expect it to work on later versions of Ubuntu.

Install the following tools with the command:

>`sudo apt-get install build-essential gcc make git`

Download and install AArch64 little-endian GCC cross compiler as specified in [Linaro Release Notes][Linaro Release Notes].

Getting SCMI Test Suite source code
-------

Download the SCMI Test Suite source code from Github:

>`git clone https://github.com/ARM-software/scmi-tests`

Understanding the SCMI Test Suite design and library of tests
-------

The SCMI test suite design can be found at [SCMI Test Suite Design][SCMI Test Suite Design]. This provides an overview of the design considerations, the interfaces that needs to be implemented by the user to adapt this test suite for their own platform and the overall code organisation. Some control flows are also detailed to better explain the execution flow.

The library of tests included in the test suite is detailed in a test specification at [SCMI Test Specification][SCMI Test Specification]

Building the SCMI Test Suite
-------

### Build the test suite as a library

The test can be built as a library which the user can link to their execution environment to launch the test agent to run the tests on the user platform.
Issue the following command to build the test suite as a library:

>`CROSS_COMPILE=<path/to/your/AArch64 compiler>/aarch64-linux-gnu- make clean`
>
>`CROSS_COMPILE=<path/to/your/AArch64 compiler>/aarch64-linux-gnu- make PROTOCOLS=<comma separated protocol list>`

This will build libscmi_test.a library which can be linked and used in your test execution environment

If the user does not specify PROTOCOLS variable when invoking make command, by default Base Protocol test library alone is generated.

>`CROSS_COMPILE=<path/to/your/AArch64 compiler>/aarch64-linux-gnu- make clean`
>
>`CROSS_COMPILE=<path/to/your/AArch64 compiler>/aarch64-linux-gnu- make PROTOCOLS=base,clock,power,system_power,performance,sensor`

This will build the library to include tests for all the specified protocols. Do note that the comma separated values are the subfolder names found in `<test suite clone location>/protocols` folder.
If there are memory constraints on the platform fewer tests can be packed by modifying the PROTOCOLS variable.

### Build for self test mocker platform

In order to self test the test framework and test libraries easily a mocker platform is implemented which just provides some fake responses to the SCMI commands issued by the test suite. The user can build the mocker platform with the following command:

>`make clean`
>
>`make PLAT=self_test/mocker PROTOCOLS=base,clock,performance,power,system_power,sensor`

This will build the library libscmi_test.a.

Do note that this mocker platform is to enable 'testing of the test framework' only. When the test library is extended to support new protocols or commands, it is **not** necessary to support the same in the mocker platform, unless it is warranted for testing the framework changes.

### Build for arm juno platform

In order to allow the adopters of SCMI test suite to have an reference implementation and example of the needed porting for a real platform Juno ADP is chosen. This reference implementation builds the scmi test suite as an OSPM agent running from Linux using publicly available mailbox test driver interface. The user can build the test suite for Juno platform with the following command:

>`CROSS_COMPILE=<path/to/your/AArch64 compiler>/aarch64-linux-gnu- make clean`
>
>`CROSS_COMPILE=<path/to/your/AArch64 compiler>/aarch64-linux-gnu- make PLAT=arm/juno PROTOCOLS=base,clock,power,performance,system_power,sensor`

This will build the library libscmi_test.a  and additionally the executable scmi_test_agent which links to the test suite library.

It is important to note that user will have to rebuild the Linux kernel and device trees for Juno following the [Guide to test SCMI on Juno][Testing On Juno].

Executing the test suite
-------

### Running the test agent on host machine

In order to run the test suite on the host machine issue the command:

>`./scmi_test_agent`

For the self_test/mocker platform the test logs are dumped on the console itself.

### Running the test agent on juno platform

In order to run the test suite on the Juno platform issue the following command from the place where the test executable is packaged in the filesystem which was booted on Juno:

>`cd <path/to/executable in filesystem>`
>
>`./scmi_test_agent -board <variant>`

Here `<variant>` can be r0, r1 or r2.
For the arm/juno platform the test logs are captured in a report file at the same location where the test suite is executed with the name arm\_scmi\_test\_log.txt

Test Execution Report
-------
The following is an example test execution report from running tests on self_test/mocker platform with Base protocol alone included in the test library:

```
Protocol Base.-

	BASE_DISCOVERY:
			MESSAGE_ID = 0x00
			CHECK HEADER: PASSED [0x00004000]
			CHECK STATUS: PASSED [SCMI_STATUS_SUCCESS]
			RETURN@0x0 'PROTOCOL VERSION': PASSED [0x00010000]
		[base]-{PROTOCOL_VERSION}-query_protocol_version-01: CONFORMANT
			MESSAGE_ID = 0x01
			CHECK HEADER: PASSED [0x00004001]
			CHECK STATUS: PASSED [SCMI_STATUS_SUCCESS]
			RETURN@0x0 'ATTRIBUTES_RESERVED': PASSED [0]
			RETURN@0x0 'ATTRIBUTES_NUMBER_AGENTS': PASSED [1]
			RETURN@0x0 'ATTRIBUTES_NUMBER_PROTOCOLS': PASSED [5]
		[base]-{PROTOCOL_ATTRIBUTES}-query_protocol_attributes-01: CONFORMANT
			MESSAGE_ID = 0x02
			PARAMETERS uin32_t[1] = |3|
			CHECK HEADER: PASSED [0x00004002]
			CHECK STATUS: PASSED [SCMI_STATUS_SUCCESS]
			RETURN@0x0 'MESSAGE ATTRIBUTES': PASSED [0x00000000]
			CHECK STATUS@'message_id' = 0x0003: PASSED [SCMI_STATUS_SUCCESS]
		[base]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_vendor_disc_cmd_support-01: CONFORMANT
			MESSAGE_ID = 0x02
			PARAMETERS uin32_t[1] = |4|
			CHECK HEADER: PASSED [0x00004002]
			INFO STATUS: INFO [SCMI_STATUS_SUCCESS]
			RETURN@0x0 'MESSAGE ATTRIBUTES': PASSED [0x00000000]
			CHECK STATUS@'message_id' = 0x0004: PASSED [SCMI_STATUS_SUCCESS]
		[base]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_subvendor_disc_cmd_support-01: CONFORMANT
			MESSAGE_ID = 0x02
			PARAMETERS uin32_t[1] = |5|
			CHECK HEADER: PASSED [0x00004002]
			CHECK STATUS: PASSED [SCMI_STATUS_SUCCESS]
			RETURN@0x0 'MESSAGE ATTRIBUTES': PASSED [0x00000000]
			CHECK STATUS@'message_id' = 0x0005: PASSED [SCMI_STATUS_SUCCESS]
		[base]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_impl_version_cmd_support-01: CONFORMANT
			MESSAGE_ID = 0x02
			PARAMETERS uin32_t[1] = |6|
			CHECK HEADER: PASSED [0x00004002]
			CHECK STATUS: PASSED [SCMI_STATUS_SUCCESS]
			RETURN@0x0 'MESSAGE ATTRIBUTES': PASSED [0x00000000]
			CHECK STATUS@'message_id' = 0x0006: PASSED [SCMI_STATUS_SUCCESS]
		[base]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_proto_list_cmd_support-01: CONFORMANT
			MESSAGE_ID = 0x02
			PARAMETERS uin32_t[1] = |7|
			CHECK HEADER: PASSED [0x00004002]
			INFO STATUS: INFO [SCMI_STATUS_SUCCESS]
			RETURN@0x0 'MESSAGE ATTRIBUTES': PASSED [0x00000000]
			CHECK STATUS@'message_id' = 0x0007: PASSED [SCMI_STATUS_SUCCESS]
		[base]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_agent_disc_cmd_support-01: CONFORMANT
			MESSAGE_ID = 0x02
			PARAMETERS uin32_t[1] = |8|
			CHECK HEADER: PASSED [0x00004002]
			INFO STATUS: INFO [SCMI_STATUS_SUCCESS]
			RETURN@0x0 'MESSAGE ATTRIBUTES': PASSED [0x00000000]
			CHECK STATUS@'message_id' = 0x0008: PASSED [SCMI_STATUS_SUCCESS]
		[base]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_notify_error_cmd_support-01: CONFORMANT

	BASE_DISCOVER_VENDOR:
			MESSAGE_ID = 0x03
			CHECK HEADER: PASSED [0x00004003]
			CHECK STATUS: PASSED [SCMI_STATUS_SUCCESS]
			RETURN@0x0 'VENDOR IDENTIFIER': PASSED ['VENDOR 1']
		[base]-{BASE_DISCOVER_VENDOR}-query_vendor_info-01: CONFORMANT

	BASE_DISCOVER_SUB_VENDOR:
			MESSAGE_ID = 0x04
			CHECK HEADER: PASSED [0x00004004]
			CHECK STATUS: PASSED [SCMI_STATUS_SUCCESS]
			RETURN@0x0 'SUBVENDOR IDENTIFIER': PASSED ['SUBVENDOR 1']
		[base]-{BASE_DISCOVER_SUB_VENDOR}-query_subvendor_info-01: CONFORMANT

	BASE_DISCOVER_IMPLEMENTATION_VERSION:
			MESSAGE_ID = 0x05
			CHECK HEADER: PASSED [0x00004005]
			CHECK STATUS: PASSED [SCMI_STATUS_SUCCESS]
			RETURN@0x0 'IMPLEMENTATION VERSION': PASSED [0x00000001]
		[base]-{BASE_DISCOVER_IMPLEMENTATION_VERSION}-query_implementation_ver-01: CONFORMANT

	BASE_DISCOVER_LIST_PROTOCOLS:
			MESSAGE_ID = 0x06
			PARAMETERS uin32_t[1] = |0|
			CHECK HEADER: PASSED [0x00004006]
			CHECK STATUS: PASSED [SCMI_STATUS_SUCCESS]
			NUMBER OF PROTOCOLS: PASSED [5]
			PROTOCOL ID: PASSED [17]
			PROTOCOL ID: FAILED [Expected: 18, Received: 19]
			PROTOCOL ID: FAILED [Expected: 19, Received: 20]
			PROTOCOL ID: FAILED [Expected: 20, Received: 21]
			PROTOCOL ID: FAILED [Expected: 21, Received: 18]
		[base]-{BASE_DISCOVER_LIST_PROTOCOLS}-query_protocol_list-01: NON CONFORMANT
			MESSAGE_ID = 0x06
			PARAMETERS uin32_t[1] = |6|
			CHECK HEADER: PASSED [0x00004006]
			CHECK STATUS: PASSED [SCMI_STATUS_INVALID_PARAMETERS]
		[base]-{BASE_DISCOVER_LIST_PROTOCOLS}-query_invalid_skipindex-01: CONFORMANT

	BASE_DISCOVER_AGENT:
			MESSAGE_ID = 0x07
			PARAMETERS uin32_t[1] = |0|
			CHECK HEADER: PASSED [0x00004007]
			CHECK STATUS: PASSED [SCMI_STATUS_SUCCESS]
			AGENT NAME: PASSED [platform_mock]
		[base]-{BASE_DISCOVER_AGENT}-query_agent_id0-01: CONFORMANT
			MESSAGE_ID = 0x07
			PARAMETERS uin32_t[1] = |1|
			CHECK HEADER: PASSED [0x00004007]
			INFO STATUS: INFO [UNKNOWN SCMI STATUS CODE(1)]
			AGENT NAME: PASSED ['agent']
		[base]-{BASE_DISCOVER_AGENT}-query_non_zero_agentid-01: CONFORMANT
			MESSAGE_ID = 0x07
			PARAMETERS uin32_t[1] = |2|
			CHECK HEADER: PASSED [0x00004007]
			CHECK STATUS: PASSED [SCMI_STATUS_NOT_FOUND]
		[base]-{BASE_DISCOVER_AGENT}-query_invalid_agentid-01: CONFORMANT

	BASE_NOTIFY_ERRORS:
			MESSAGE_ID = 0x08
			PARAMETERS uin32_t[1] = |1|
			CHECK HEADER: PASSED [0x00004008]
			CHECK STATUS: PASSED [SCMI_STATUS_SUCCESS]
		[base]-{BASE_NOTIFY_ERRORS}-reg_err_notification-01: CONFORMANT
			MESSAGE_ID = 0x08
			PARAMETERS uin32_t[1] = |0|
			CHECK HEADER: PASSED [0x00004008]
			CHECK STATUS: PASSED [SCMI_STATUS_SUCCESS]
		[base]-{BASE_NOTIFY_ERRORS}-postcondition_reg_err_notification-01: CONFORMANT
```

The test report list results per test case which is organised as a collection within individual test suites in a protocol. For every test case the following verification steps happen:

`Message Header`: The message header received from the platform is checked against what was in the send command. This check cannot be skipped.

`Return Value - Status`: The platform returns a status for a command issued by the agent and this is checked as per specification or as per expected values provided by the user. The test suite can skip the check against return status and report the received value as 'INFO'.

`Return Value - Others if relevant`: The remaining of the return values for a command response is valid based on status return value. Checks will be performed only if status return value returned SUCCESS and expected values were given by user or captured in the test library. If not, the returned values will be reported as 'INFO'.

Each individual check deliveres a PASS/FAIL verdict unless marked as INFO. A test case will be reported NON CONFORMANT if any of the checks returns FAIL in a test case. Otherwise it returns a CONFORMANT verdict. Any checks with INFO status do not influence the final veridict.

- - - - - - - - - - - - - - - - -

_Copyright (c) 2017, ARM Limited and Contributors. All rights reserverd._

[Linaro Release Notes]:		https://community.arm.com/dev-platforms/b/documents/posts/linaro-release-notes-current
[SCMI Test Suite Design]:	./scmi_test_suite_design.md
[SCMI Test Specification]:	./scmi_test_specification.md
[Testing On Juno]:		./guide_for_juno_testing.md
