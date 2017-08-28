# System Control and Management Interface - Test Suite

## SCMI
**System Control and Management Interface** (SCMI)  is a set of operating system-independent software interfaces that are used in system management.

For more information, download the [SCMI Platform Design Document](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.den0056a/index.html).

## SCMI Test Suite
SCMI test suite enables adopters of System Control and Management Interface \(SCMI\) PDD \(Platform Design Document\) to test their own implementations. SCMI test suite provides the user a predefined library of tests and a test framework to execute these tests. The test suite is designed to allow users to adapt it for their own platforms. It also enables the user to extend the current test library to include their own platform specific custom commands and custom protocols.

## Release details
 - Code Quality: REL v1.0
 - The tests are written for version 1.0 of the SCMI PDD.
 - The compliance suite is not a substitute for design verification.
 - To know about the gaps in the test coverage, see the Test coverage section below.

### Test Coverage

The following test coverage is available with the current release of the test suite.

 - Base Protocol <br>
   Tests for all commands except for testing notifications.
 - Power Domain Management Protocol <br>
   Tests for all commands except for positive tests for set power state domain and testing notifications.
 - System Power Management Protocol <br>
   Tests for all commands except for positive tests for system power set and testing notifications.
 - Performance Domain Management Protocol <br>
   Tests for all commands except for testing notifications.
 - Clock Management Protocol <br>
   Tests for all commands except for testing notifications.
 - Sensor Management Protocol <br>
   Tests for all commands except for testing notifications.

### Future Enhancements

The following features or capability additions are planned as part of future releases
-   Testing notifications and delayed responses for relevant commands.
-   Test library extension with new test cases for additional protocols as defined in newer version of the PDD
-   Testing multiple commands in a single test case

## Getting Started
Get the SCMI test suite source code from [GitHub](https://github.com/ARM-software/scmi-tests).

See the [User Guide] for instructions to adapt, build and run the test suite and interpreting the results from the test execution.

### Additional Reading
For details on the design of the SCMI test suite, Refer to [Test Suite Design].

## License
The software is provided under Apache 2.0 license [License]. Contributions to this project is accepted under the same license.

## Feedback and Support
ARM values and welcomes any feedback and contributions to this project.

*   For feedback, please use the issue tracker associated with this project [Issue Tracker](https://github.com/ARM-software/scmi-tests/issues).
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
