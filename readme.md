**SCMI Test Suite - version 1.0**
=================================

SCMI test suite enables adopters of System Control and Management Interface \(SCMI\) PDD \(Platform Design Document\) to test their own implementations. SCMI test suite provides the user a predefined library of tests and a test framework to execute these tests. The test suite is designed to allow users to adapt it for their own platforms. It also enables the user to extend the current test library to include their own platform specific custom commands and custom protocols.

License
-------

The software is provided under Apache 2.0 license [License]. Contributions to this project is accepted under the same license.

Current Release
---------------

This is the first public release of SCMI test suite against v1.0 of SCMI PDD [at infocenter](http://infocenter.arm.com/help/topic/com.arm.doc.den0056a/DEN0056A_System_Control_and_Management_Interface.pdf). It provides a non-exhaustive library of tests against the different protocols and commands outlined in the PDD.

### Available Test Coverage

The following test coverage is available with the current release of the test suite.

*   Base Protocol - Tests for all commands except for testing notifications.
*   Power Domain Management Protocol - Tests for all commands except for positive tests for set power state domain and testing notifications.
*   System Power Management Protocol - Tests for all commands except for positive tests for system power set and testing notifications.
*   Performance Domain Management Protocol - Tests for all commands except for testing notifications.
*   Clock Management Protocol - Tests for all commands except for testing notifications.
*   Sensor Management Protocol - Tests for all commands except for testing notifications.

### Future Enhancements

The following features or capability additions are planned as part of future releases

*   Testing notifications and delayed responses for relevant commands.
*   Test library extension with new test cases for additional protocols as defined in newer version of the PDD
*   Testing multiple commands in a single test case

Getting Started
---------------

Get the SCMI test suite source code from [GitHub](https://github.com/ARM-software/scmi-tests).

See the [User Guide] for instructions to adapt, build and run the test suite and interpreting the results from the test execution.

Refer to [Test Suite Design] and [Test Specification] for more details.

### Feedback and Support

ARM values and welcome any feedback and contributions to this project.

*   For feedback please use the issue tracker associated with this project [Issue Tracker](https://github.com/ARM-software/scmi-tests/issues).
*   For support please send your queries to [support-scmi-acs@arm.com](mailto:support-scmi-acs@arm.com).
*   ARM licensees may contact us directly via their partner managers.

### Contributor Acknowledgements
Please refer to [Contributors].

- - - - - - - - - - - - - - - - - - - -

_Copyright (c) 2017, ARM Limited and Contributors. All rights reserved._

[License]:			./license.md "Apache 2.0 License for SCMI Test Suite"
[User Guide]:			./docs/user_guide.md "SCMI Test Suite User Guide"
[Test Suite Design]:		./docs/scmi_test_suite_design.md "SCMI Test Suite Design"
[Test Specification]:		./docs/scmi_test_specification.md "SCMI Test Specification"
[Contributors]:			./acknowledgements.md
