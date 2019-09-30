**SCMI Tests - Scenario document**
===================================
Table of Contents:
- [Introduction](#introduction)
- [General Rules](#general-rules)
- [Base Protocol Tests](#base-protocol-tests)
- [Power Domain Management Protocol Tests](#power-domain-management-protocol-tests)
- [System Power Management Protocol Tests](#system-power-management-protocol-tests)
- [Performance Domain Management Protocol Tests](#performance-domain-management-protocol-tests)
- [Clock Management Protocol Tests](#clock-management-protocol-tests)
- [Sensor Management Protocol Tests](#sensor-management-protocol-tests)

Introduction
-------
This document outlines the test scenarios and how individual test cases are implemented in the current version of the SCMI test suite.

General Rules
-------
In writing the tests, these rules have been followed:

----------

> All test case identifiers **must** be unique.
> All test case have a payload function which has test algorithm.

----------

> Every test case compares return values against expected results that are provided by the user through the platform properties to determine a PASS or FAIL verdict. In case some protocols are not supported by particular platform, those test cases needs to be skipped.

----------

> All message fields that are defined by specification as 'Reserved, Must be Zero' **must** be checked and reported as compliance failure if not adhered to.

----------

> The SCMI test library comprises of a collection of test suites that are grouped per protocol.

----------

> Some tests might require a **pre-condition** or a **post-condition** check. These checks are captured as part of test cases that **must** come immediately preceding or just after the relevant test case payload function. The test in that case must have pre_condition or post_condition function.

> A typical example for a test case needing a precondition is the get command which has a set command as a precondition.

> A typical example for a test case needing a postcondition is a subscribe for notification command which has unsubscribe for notification command as the postcondition.


Base Protocol Tests
----------
This section outlines the test specification for SCMI Base Protocol.

| Test ID    | Test Intent           | Verification Steps  | Comments/Commands Used  |
| ---------- | --------------------- | ------------------- | ----------------------- |
| test_b001  | Query the protocol version information. | Check Version against expected value. | PROTOCOL\_VERSION |
| test_b002  | Query the protocol attributes. | Check num\_agents and num\_protocols against expected value. | PROTOCOL\_ATTRIBUTES |
| test_b003  | Query for mandatory command support. | Check command implementation status. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_b004  | Query for vendor information. | Check vendor name against expected value. | BASE\_DISCOVER\_VENDOR |
| test_b005  | Query for vendor specific implementation version. | Check implementation version against expected value. | BASE\_DISCOVER\_VENDOR |
| test_b006  | Query for sub vendor information. | Check sub vendor name against expected value. | BASE\_DISCOVER\_SUB\_VENDOR |
| test_b007  | Pre-Condition: BASE\_DISCOVER\_AGENT support.<br /> Query for agent name with valid and invalid agent id. | 1. Check agent name starts with "platform" when agent id = 0.<br /> 2. Check agent name with expected value for valid agent id.<br /> 3. Check agent name matches expected value when agent_id = 0xFFFFFFFF.<br /> 4. Check NOT_FOUND return status for invalid agent id. | PROTOCOL\_MESSAGE\_ATTRIBUTES<br /> BASE\_DISCOVER\_AGENT |
| test_b008  | Pre-Condition: BASE\_NOTIFY\_ERRORS support.<br /> 1. Register and Unregister for error notification from platform.<br /> 2. Send command with invalid parameters. | 1. Check SUCCESS status returned for register and unregister.<br /> 2. Check INVALID\_PARAMETERS return status for invalid notify_enable. | PROTOCOL\_MESSAGE\_ATTRIBUTES<br /> BASE\_NOTIFY\_ERRORS |
| test_b009  | Query protocol list with valid and invalid skip value. | 1. Check protocol list against expected value for valid skip value.<br /> 2. Check INAVLID\_PARAMETERS return status for invalid skip value. | BASE\_DISCOVER\_LIST\_PROTOCOLS |
| test_b010  | Send invalid command id for base protocol.<br />Query the protocol message attributes with invalid msg_id. | Check NOT\_FOUND status is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_b011  | Pre-Condition: BASE\_SET\_PROTOCOL\_PERMISSIONS support.<br />1. Try setting protocol permission for invalid agent.<br /> 2. Try to remove access for base\_protocol.<br /> 3. Send command with invalid parameters. | 1. Check NOT\_FOUND status is returned.<br /> 2. Check NOT\_FOUND status is returned.<br /> 3. Check INVALID\_PARAMETERS is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES<br /> BASE\_SET\_PROTOCOL\_PERMISSIONS |
| test_b012  | Pre-Condition: BASE\_SET\_DEVICE\_PERMISSIONS support.<br /> 1. Try setting device permissions for invalid agent.<br /> 2. Send command with invalid parameters.<br /> | 1. Check NOT_FOUND status is returned.<br /> 2. Check INVALID_PARAMETERS status is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES<br /> BASE\_SET\_DEVICE\_PERMISSIONS |
| test_b013  | Pre-Condition: BASE\_RESET\_AGENT\_CONFIGURATION support.<br /> 1. Try resetting device and protocol permissions for invalid agent.<br /> 2. Send command with invalid flags value.<br /> | 1. Check NOT_FOUND status is returned.<br /> 2. Check INVALID_PARAMETERS status is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES<br /> BASE\_RESET\_AGENT\_CONFIGURATION |
| test_b014  | Pre-Condition: BASE\_SET\_PROTOCOL\_PERMISSIONS support.<br /> 1. Deny agent access to a valid protocol.<br /> 2. Try accessing command of denied protocol.<br /> 3. Restore the protocol access with BASE\_SET\_PROTOCOL\_PERMISSIONS. | 1. Check NOT_FOUND status is returned when access denied protocol.<br /> 2. Agent should be able to access protocol after permissions restored.<br />| BASE\_SET\_PROTOCOL\_PERMISSIONS |
| test_b015  | Pre-Condition: BASE\_SET\_PROTOCOL\_PERMISSIONS and BASE\_RESET\_AGENT\_CONFIGURATION support.<br /> 1. Deny agent access to a valid protocol.<br /> 2. Try accessing command of denied protocol.<br /> 3. Restore the protocol access with BASE\_RESET\_AGENT\_CONFIGURATION. | 1. Check NOT_FOUND status is returned when access denied protocol.<br /> 2. Agent should be able to access protocol after permissions restored.<br />| BASE\_SET\_PROTOCOL\_PERMISSIONS<br /> BASE\_RESET\_AGENT\_CONFIGURATION |
| test_b016  | Pre-Condition: BASE\_SET\_DEVICE\_PERMISSIONS support.<br /> 1. Deny agent access to a valid device.<br /> 2. Try accessing denied device.<br /> 3. Restore the protocol access with BASE\_SET\_DEVICE\_PERMISSIONS. | 1. Check NOT_FOUND status is returned when access denied agent.<br /> 2. Agent should be able to access device after permissions restored.<br />| BASE\_SET\_DEVICE\_PERMISSIONS |
| test_b017  | Pre-Condition: BASE\_SET\_DEVICE\_PERMISSIONS and BASE\_RESET\_AGENT\_CONFIGURATION support.<br /> 1. Deny agent access to a valid device.<br /> 2. Try accessing denied device.<br /> 3. Restore the device access with BASE\_RESET\_AGENT\_CONFIGURATION. | 1. Check NOT_FOUND status is returned when access denied device.<br /> 2. Agent should be able to access device after permissions restored.<br />| BASE\_SET\_DEVICE\_PERMISSIONS<br /> BASE\_RESET\_AGENT\_CONFIGURATION |

Power Domain Management Protocol Tests
---------
This section outlines the test specification for SCMI Power Domain Management Protocol.

| Test ID    | Test Intent           | Verification Step  | Comments/Commands Used  |
| ---------- | --------------------- | ------------------ | ----------------------- |
| test_p001  | Query the protocol version information. | Check Version against expected value. | PROTOCOL\_VERSION |
| test_p002  | Query the protocol attributes. | Check num\_agents and num\_protocols against expected value. | PROTOCOL\_ATTRIBUTES |
| test_p003  | Query for mandatory command availability. | Check command implementation status. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_p004  | Query power domain attributes with valid and non-existent power domain id. | 1. Check power domain attributes with expected value for valid power domain id.<br /> 2. Check NOT_FOUND return status for non-existent power domain id. | POWER\_DOMAIN\_ATTRIBUTES |
| test_p005  | 1. Try setting power state of non-existent power domain id.<br /> 2. Send command with invalid flags and power state value.<br /> 3. Set valid power state for valid power domain id.<br /> 4. Read the power state of domain whose state was changed | 1. Check NOT_FOUND status is returned.<br /> 2. Check INVALID_PARAMETERS status is returned.<br /> 3. Check SUCCESS is returned and verify state with power state get command. | POWER\_STATE\_SET<br /> POWER\_STATE\_GET |
| test_p006  | Query power state get for non-existent power domain id. | 1. Check NOT_FOUND return status for non-existent power domain id. | POWER\_STATE\_GET |
| test_p007  | Pre-Condition: POWER\_STATE\_NOTIFY support.<br /> 1. Enable notification for power state changed.<br /> 2. Enable power state notification for non-existent domain. <br /> 3. Send command with invalid notify_enable.<br /> 4. Change power state of domain.<br /> 5. Disable the power state notification.<br /> 6. Change the power state  | 1. Check SUCCESS status is returned.<br /> 2. Check NOT_FOUND status is returned.<br /> 3. Check INVALID_PARAMETERS status is returned.<br /> 4. Check power state is changed and power state notification is received.<br /> 5. Check SUCCESS status is returned.<br /> 6. Check no power state notification is generated  | POWER\_STATE\_NOTIFY<br /> POWER\_STATE\_GET<br /> POWER_STATE_SET |
| test_p008  | 1. Send invalid command id for power domain protocol.<br /> 2. Query the protocol message attributes with invalid msg_id. | Check NOT\_FOUND status is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_p009  | Pre-Condition: POWER\_STATE\_CHANGE\_REQUESTED\_NOTIFY support<br /> 1. Set power state change notification for non-existent power domain.<br /> 2. Invoke command with invalid notify_enable. | 1. Check NOT\_FOUND status is returned.<br /> 2. Check INVALID\_PARAMETERS status is returned. | POWER\_STATE\_CHANGE\_REQUESTED\_NOTIFY |

System Power Management Protocol Tests
---------
This section outlines the test specification for SCMI System Power Management Protocol.

| Test ID    | Test Intent           | Verification Step  | Comments/Commands Used  |
| ---------- | --------------------- | ------------------ | ----------------------- |
| test_s001  | Query the protocol version information. | Check Version against expected value. | PROTOCOL\_VERSION |
| test_s002  | Query the protocol attributes. | Check attributes byte is zero. | PROTOCOL\_ATTRIBUTES |
| test_s003  | Query for mandatory command availability. | Check command implementation status. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_s004  | Pre-Condition: SYSTEM\_POWER\_STATE\_GET support.<br /> Query system power state and check state with expected value. | Check SUCCESS status is returned. | SYSTEM_POWER_STATE_GET |
| test_s005  | 1. Send invalid command id for system power domain protocol.<br /> 2. Query the protocol message attributes with invalid msg_id. | Check NOT\_FOUND status is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES |

Performance Domain Management Protocol Tests
---------
This section outlines the test specification for SCMI Performance Domain Management Protocol.

| Test ID    | Test Intent           | Verification Step  | Comments/Commands Used  |
| ---------- | --------------------- | ------------------ | ----------------------- |
| test_d001  | Query the protocol version information. | Check Version against expected value. | PROTOCOL\_VERSION |
| test_d002  | Query the protocol attributes. | Check num\_performance\_domains against expected value. | PROTOCOL\_ATTRIBUTES |
| test_d003  | Query for mandatory command availability. | Check command implementation status. | PROTOCOL\_MESSAGE\_ATTRIBUTES |

Clock Management Protocol Tests
---------
This section outlines the test specification for SCMI Clock Management Protocol.

| Test ID    | Test Intent           | Verification Step  | Comments/Commands Used  |
| ---------- | --------------------- | ------------------ | ----------------------- |
| test_c001  | Query the protocol version information. | Check Version against expected value. | PROTOCOL\_VERSION |
| test_c002  | Query the protocol attributes. | Check num\_clocks against expected value. | PROTOCOL\_ATTRIBUTES |
| test_c003  | Query for mandatory command availability. | Check command implementation status. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_c004  | 1. Query clock attributes with invalid clock id.<br /> 2. Query clock attributes for all system clocks. | 1. Check NOT\_FOUND status is returned.<br /> 2. Check clock attributes are returned. | CLOCK\_ATTRIBUTES |
| test_c005  | 1. Query clock rate for invalid clock id.<br /> 2. Query clock rates for all system clocks. | 1. Check NOT\_FOUND status is returned.<br /> 2. Check clock rates are returned. | CLOCK\_RATE\_GET |
| test_c006  | 1. Configure clock rate for invalid clock id.<br /> 2. Configure clock rate with invalid attributes. | 1. Check NOT\_FOUND status is returned.<br /> 2. Check INVALID\_PARAMETERS status is returned. | CLOCK\_CONFIG\_SET |
| test_c007  | 1. Send invalid command id for clock protocol.<br /> 2. Query the protocol message attributes with invalid msg_id. | Check NOT\_FOUND status is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES |

Sensor Management Protocol Tests
---------
This section outlines the test specification for SCMI Sensor Management Protocol.

| Test ID    | Test Intent           | Verification Step  | Comments/Commands Used  |
| ---------- | --------------------- | ------------------ | ----------------------- |
| test_m001  | Query the protocol version information. | Check Version against expected value. | PROTOCOL\_VERSION |
| test_m002  | Query the protocol attributes. | Check num\_sensors against expected value. | PROTOCOL\_ATTRIBUTES |
| test_m003  | Query for mandatory command availability. | Check command implementation status. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_m004  | 1. Send invalid command id for sensor protocol.<br /> 2. Query the protocol message attributes with invalid msg_id. | Check NOT\_FOUND status is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_m005  | 1. Read sensor data for invalid sensor id.<br /> 2. Pass invalid flags value for sensor read. | 1. Check NOT\_FOUND status is returned.<br /> 2. Check NOT\_FOUND status is returned. | SENSOR\_READING\_GET |
- - - - - - - - - - - - - - - - - - - -

_Copyright (c) 2019, Arm Limited and Contributors. All rights reserved._
