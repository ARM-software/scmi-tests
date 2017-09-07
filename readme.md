# System Control and Management Interface - Test Suite

## SCMI
**System Control and Management Interface** (SCMI)  is a set of Operating System-independent software interfaces that are used in system management.

For more information, download the [SCMI Platform Design Document](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.den0056a/index.html).

## SCMI test suite
SCMI test suite enables adopters of System Control and Management Interface \(SCMI\) PDD \(Platform Design Document\) to test their own implementations. SCMI test suite provides you a predefined library of tests and a test framework to execute these tests. The test suite allows you to adapt it for your own platforms. It also enables you to extend the current test library to include your own platform-specific custom commands and custom protocols.

## Release details
 - Code quality: REL v1.0.
 - The tests are written for version 1.0 of the SCMI PDD.
 - The compliance suite is not a substitute for design verification.
 - To know about the gaps in the test coverage, see the Test coverage section.

### Test coverage

The following test coverage is available with the current release of the test suite.

| Protocol | Description |
| - | - |
| Base protocol | Tests for all commands except for testing notifications. |
| Power domain management protocol | Tests for all commands except for positive tests for set power state domain and testing notifications. |
| System power management protocol |    Tests for all commands except for positive tests for system power set and testing notifications. |
| Performance domain management protocol |    Tests for all commands except for testing notifications. |
| Clock management protocol |    Tests for all commands except for testing notifications. |
| Sensor management protocol | Tests for all commands except for testing notifications. |

### Future enhancements

The following features or capability additions are planned as part of future releases:
-   Testing notifications and delayed responses for relevant commands.
-   Test library extension with new test cases for additional protocols as defined in newer version of the PDD.
-   Testing multiple commands in a single test case.

## Getting started
Get the SCMI test suite source code from [GitHub](https://github.com/ARM-software/scmi-tests).

See the [User Guide] for instructions to adapt, build, run the test suite, and interpret the results from the test execution.

### Additional reading
For details on the design of the SCMI test suite, see [Test Suite Design].

## License
The software is provided under Apache 2.0 license [License]. Contributions to this project are accepted under the same license.

## Feedback and support
Arm values and welcomes any feedback and contributions to this project.

*   For feedback, use the issue tracker that is associated with this project [Issue Tracker](https://github.com/ARM-software/scmi-tests/issues).
*   For support, send your queries to [support-scmi-acs@arm.com](mailto:support-scmi-acs@arm.com).
*   Arm licensees can contact us directly through their partner managers.

### Contributor acknowledgements
See [Contributors].

- - - - - - - - - - - - - - - - - - - -

_Copyright (c) 2017, Arm Limited and Contributors. All rights reserved._

[License]:			./license.md "Apache 2.0 License for SCMI Test Suite"
[User Guide]:			./docs/user_guide.md "SCMI Test Suite User Guide"
[Test Suite Design]:		./docs/scmi_test_suite_design.md "SCMI Test Suite Design"
[Test Specification]:		./docs/scmi_test_specification.md "SCMI Test Specification"
[Contributors]:			./acknowledgements.md
