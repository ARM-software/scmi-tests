
# System Control and Management Interface - Compliance Suite


## NOTE

This repository has moved to a new location. Visit https://gitlab.arm.com/tests/scmi-tests for the latest updates.

## Introduction
**System Control and Management Interface** (SCMI)  is a set of Operating System-independent software interfaces that are used in system management.

For more information, download the [SCMI Platform Design Document](https://developer.arm.com/documentation/den0056/b/?lang=en).

## SCMI Compliance suite
SCMI compliance suite is a collection of self-checking, portable C tests. SCMI compliance suite enables adopters of SCMI Platform Design Document \(PDD\) to test their own implementations. It provides a predefined library of tests and a test framework to execute these tests. The test suite can be adapted to different platforms. It also enables the extension of the current test library to include platform-specific custom commands and custom protocols.

## Release details
 - Code quality: REL v2.0.
 - The tests are written for version 2.0 of the SCMI PDD.
 - The compliance suite maintains backward compatibility with version 1.0 of the SCMI PDD.
 - The compliance suite is not a substitute for design verification.
 - The compliance suite is verified against [SCP-firmware](https://github.com/ARM-software/SCP-firmware).
 - To know about the gaps in the test coverage, see the [Test checklist].

## GitHub branch
  Use the master branch to get the latest version of the code with bug fixes and new features.

## Test scenarios

The mapping of the rules in the specification to the test cases and the steps followed in the tests are mentioned in the [Test checklist].

## Getting started
See the [User Guide] for instructions to adapt, build and run the test suite.

### Additional reading
For details on the design of the SCMI test suite, see [Validation Methodology Document].

## License
The software is provided under Apache 2.0 license. Contributions to this project are accepted under the same license.

## Feedback and support
Arm values and welcomes any feedback and contributions to this project.

*   For feedback, use the issue tracker that is associated with this project [Issue Tracker](https://github.com/ARM-software/scmi-tests/issues).
*   For support, send your queries to [support-scmi-acs@arm.com](mailto:support-scmi-acs@arm.com).
*   Arm licensees can contact Arm directly through their partner managers.
*   Arm welcomes code contributions through GitHub pull requests.


- - - - - - - - - - - - - - - - - - - -

_Copyright (c) 2019-2020, Arm Limited and Contributors. All rights reserved._

[User Guide]:           ./docs/user_guide.md "SCMI Test Suite User Guide"
[Validation Methodology Document]:      ./docs/Arm_SCMI_Validation_Methodology.pdf "SCMI Test Suite Design"
[Test checklist]:       ./docs/scmi_testlist.md "SCMI Test Specification"
