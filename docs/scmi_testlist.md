
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
- [Reset Management Protocol Tests](#reset-management-protocol-tests)

Introduction
-------
This document outlines the test scenarios and how individual test cases are implemented in the current version of the SCMI test suite.

General Rules
-------
In writing the tests, these rules have been followed:

----------

> All test case identifiers **must** be unique.

----------

> Every test case compares return values against expected results that are provided by the user through the platform properties to determine a PASS or FAIL verdict. In case some protocols are not supported by particular platform, those test cases need to be skipped.

----------

> All message fields that are defined by specification as 'Reserved, Must be Zero' **must** be checked and reported as compliance failure if not adhered to.

----------

> The SCMI test library comprises of a collection of test suites that are grouped per protocol.

----------

> Some tests might require a **pre-condition** or a **post-condition** check. These checks are captured as part of test cases that **must** come immediately preceding or just after the relevant test case payload function.

> A typical example for a test case that needs a precondition is the get command which has a set command as a precondition.

> A typical example for a test case that needs a postcondition is a subscribe for notification command which has unsubscribe for notification command as the postcondition.


Base Protocol Tests
----------
This section outlines the test specification for SCMI Base Protocol.

| Test ID    | Test Intent           | Verification Steps  | Comments/Commands Used  |
| ---------- | --------------------- | ------------------- | ----------------------- |
| test_b001  | Query the protocol version information. | Check Version against expected value. | PROTOCOL\_VERSION |
| test_b002  | Query the protocol attributes. | Check num\_agents and num\_protocols against expected value. | PROTOCOL\_ATTRIBUTES |
| test_b003  | Query for mandatory command support. | Check command implementation status. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_b004  | Send invalid command id for base protocol.<br />Query the protocol message attributes with invalid msg_id. | Check NOT\_FOUND status is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_b005  | Query for vendor information. | Check vendor name against expected value. | BASE\_DISCOVER\_VENDOR |
| test_b006  | Pre-Condition: BASE\_DISCOVER\_SUB\_VENDOR support.<br />Query for sub vendor information. | Check sub vendor name against expected value. | PROTOCOL\_MESSAGE\_ATTRIBUTES<br />BASE\_DISCOVER\_SUB\_VENDOR |
| test_b007  | Query for vendor specific implementation version. | Check implementation version against expected value. | BASE\_DISCOVER\_IMPLEMENTATION\_VERSION|
| test_b008  | Query protocol list with valid and invalid skip value. | 1. Check protocol list against expected value for valid skip value.<br /> 2. Check INAVLID\_PARAMETERS return status for invalid skip value. | BASE\_DISCOVER\_LIST\_PROTOCOLS |
| test_b009  | Pre-Condition: BASE\_DISCOVER\_AGENT support.<br /> Query for agent name with valid and invalid agent id. | 1. Check agent name starts with "platform" when agent id = 0.<br /> 2. Check agent name with expected value for valid agent id.<br /> 3. Check agent name matches expected value when agent_id = 0xFFFFFFFF.<br /> 4. Check NOT_FOUND return status for invalid agent id. | PROTOCOL\_MESSAGE\_ATTRIBUTES<br /> BASE\_DISCOVER\_AGENT |
| test_b010  | Pre-Condition: BASE\_NOTIFY\_ERRORS support.<br /> 1. Register and Unregister for error notification from platform.<br /> 2. Send command with invalid parameters. | 1. Check SUCCESS status returned for register and unregister.<br /> 2. Check INVALID\_PARAMETERS return status for invalid notify_enable. | PROTOCOL\_MESSAGE\_ATTRIBUTES<br /> BASE\_NOTIFY\_ERRORS |
| test_b011  | Pre-Condition: BASE\_SET\_DEVICE\_PERMISSIONS support.<br /> 1. Try setting device permissions for invalid agent.<br /> 2. Send command with invalid parameters.<br />  3. Remove acces of a invalid device.<br /> | 1. Check NOT_FOUND status is returned.<br /> 2. Check INVALID_PARAMETERS status is returned. <br /> 3. Check NOT_FOUND status is returned.| PROTOCOL\_MESSAGE\_ATTRIBUTES<br /> BASE\_SET\_DEVICE\_PERMISSIONS |
| test_b012  | Pre-Condition: BASE\_SET\_DEVICE\_PERMISSIONS support.<br /> 1. Deny agent access to a valid device.<br /> 2. Try accessing denied device.<br /> 3. Restore the protocol access with BASE\_SET\_DEVICE\_PERMISSIONS. | 1. Check NOT_FOUND status is returned when access denied agent.<br /> 2. Agent should be able to access device after permissions restored.<br />| BASE\_SET\_DEVICE\_PERMISSIONS |
| test_b013  | Pre-Condition: BASE\_SET\_PROTOCOL\_PERMISSIONS support.<br />1. Try setting protocol permission for invalid agent.<br /> 2. Try to remove access for base\_protocol.<br /> 3. Send command with invalid parameters.<br /> 4. Remove access of invalid device. | 1. Check NOT\_FOUND status is returned.<br /> 2. Check NOT\_FOUND status is returned.<br /> 3. Check INVALID\_PARAMETERS is returned.<br /> 4. Check NOT_FOUND status is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES<br /> BASE\_SET\_PROTOCOL\_PERMISSIONS |
| test_b014  | Pre-Condition: BASE\_SET\_PROTOCOL\_PERMISSIONS support.<br /> 1. Deny agent access to a valid protocol.<br /> 2. Try accessing command of denied protocol.<br /> 3. Restore the protocol access with BASE\_SET\_PROTOCOL\_PERMISSIONS. | 1. Check NOT_FOUND status is returned when access denied protocol.<br /> 2. Agent should be able to access protocol after permissions restored.<br />| BASE\_SET\_PROTOCOL\_PERMISSIONS |
| test_b015  | Pre-Condition: BASE\_RESET\_AGENT\_CONFIGURATION support.<br /> 1. Try resetting device and protocol permissions for invalid agent.<br /> 2. Send command with invalid flags value.<br /> | 1. Check NOT_FOUND status is returned.<br /> 2. Check INVALID_PARAMETERS status is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES<br /> BASE\_RESET\_AGENT\_CONFIGURATION |
| test_b016  | Pre-Condition: BASE\_SET\_DEVICE\_PERMISSIONS and BASE\_RESET\_AGENT\_CONFIGURATION support.<br /> 1. Deny agent access to a valid device.<br /> 2. Try accessing denied device.<br /> 3. Restore the device access with BASE\_RESET\_AGENT\_CONFIGURATION. | 1. Check NOT_FOUND status is returned when access denied device.<br /> 2. Agent should be able to access device after permissions restored.<br />| BASE\_SET\_DEVICE\_PERMISSIONS<br /> BASE\_RESET\_AGENT\_CONFIGURATION |
| test_b017  | Pre-Condition: BASE\_SET\_PROTOCOL\_PERMISSIONS and BASE\_RESET\_AGENT\_CONFIGURATION support.<br /> 1. Deny agent access to a valid protocol.<br /> 2. Try accessing command of denied protocol.<br /> 3. Restore the protocol access with BASE\_RESET\_AGENT\_CONFIGURATION. | 1. Check NOT_FOUND status is returned when access denied protocol.<br /> 2. Agent should be able to access protocol after permissions restored.<br />| BASE\_SET\_PROTOCOL\_PERMISSIONS<br /> BASE\_RESET\_AGENT\_CONFIGURATION |


Power Domain Management Protocol Tests
---------
This section outlines the test specification for SCMI Power Domain Management Protocol.

| Test ID    | Test Intent           | Verification Step  | Comments/Commands Used  |
| ---------- | --------------------- | ------------------ | ----------------------- |
| test_p001  | Query the protocol version information. | Check Version against expected value. | PROTOCOL\_VERSION |
| test_p002  | Query the protocol attributes. | Check power domain protocol attributes against expected values. | PROTOCOL\_ATTRIBUTES |
| test_p003  | Query for mandatory command availability. | Check command implementation status. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_p004  | 1. Send invalid command id for power domain protocol.<br /> 2. Query the protocol message attributes with invalid msg_id. | Check NOT\_FOUND status is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_p005  | Query power domain attributes with valid power domain id. | Check power domain attributes with expected value for valid power domain id.| POWER\_DOMAIN\_ATTRIBUTES |
| test_p006  | Query power domain attributes with non-existent power domain id. | Check NOT_FOUND return status for non-existent power domain id. | POWER\_DOMAIN\_ATTRIBUTES |
| test_p007  | Pre-Condition: Power domain should support power state set. <br /> Send command with invalid flags and power state value.| Check INVALID_PARAMETERS status is returned.| POWER\_STATE\_SET<br /> POWER\_DOMAIN\_ATTRIBUTES |
| test_p008  | Send power state set command for not supported domain.| Check NOT\_SUPPORTED status is returned.| POWER\_STATE\_SET<br /> POWER\_DOMAIN\_ATTRIBUTES |
| test_p009  | Try setting power state of non-existent power domain id | Check NOT_FOUND status is returned. | POWER\_STATE\_SET |
| test_p010  | Query power state get for valid domain id. | Check SUCCESS return status. | POWER\_STATE\_GET |
| test_p011  | Query power state get for non-existent power domain id. | Check NOT_FOUND return status for non-existent power domain id. | POWER\_STATE\_GET |
| test_p012  | Pre-Condition: POWER\_STATE\_NOTIFY support.<br /> Send command with invalid notify_enable. | Check INVALID\_PARAMETERS status is returned. | POWER\_STATE\_NOTIFY<br /> POWER\_DOMAIN\_ATTRIBUTES |
| test_p013  | Pre-Conditions: POWER\_STATE\_NOTIFY support. <br /> Send command for not supported domain. | Check NOT\_SUPPORTED status is returned. | POWER\_STATE\_NOTIFY<br /> POWER\_DOMAIN\_ATTRIBUTES |
| test_p014  | Pre-Condition: POWER\_STATE\_NOTIFY support.<br /> Enable power state notification for non-existent domain. | Check NOT_FOUND status is returned | POWER\_STATE\_NOTIFY |
| test_p015  | Pre-Condition: POWER\_STATE\_CHANGE\_REQUESTED\_NOTIFY support<br /> Invoke command with invalid notify_enable. | Check INVALID\_PARAMETERS status is returned. | POWER\_STATE\_CHANGE\_REQUESTED\_NOTIFY |
| test_p016  | Pre-Condition: POWER\_STATE\_CHANGE\_REQUESTED\_NOTIFY support<br /> Set power state change notification for non-existent power domain. | Check NOT\_FOUND status is returned. | POWER\_STATE\_CHANGE\_REQUESTED\_NOTIFY |

System Power Management Protocol Tests
---------
This section outlines the test specification for SCMI System Power Management Protocol.

| Test ID    | Test Intent           | Verification Step  | Comments/Commands Used  |
| ---------- | --------------------- | ------------------ | ----------------------- |
| test_s001  | Query the protocol version information. | Check Version against expected value. | PROTOCOL\_VERSION |
| test_s002  | Query the protocol attributes. | Check attributes byte is zero. | PROTOCOL\_ATTRIBUTES |
| test_s003  | Query for mandatory command availability. | Check command implementation status. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_s004  | 1. Send invalid command id for system power protocol.<br /> 2. Query the protocol message attributes with invalid msg_id. | Check NOT\_FOUND status is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_s005  | Pre-Condition: SYSTEM\_POWER\_STATE\_SET support.<br /> Query system power state set with invalid flag. | Check INVALID\_PARAMETERS status is returned. | SYSTEM\_POWER\_STATE\_SET |
| test_s006  | Pre-Condition: SYSTEM\_POWER\_STATE\_GET support.<br /> Query system power state and check state with expected value. | Check SUCCESS status is returned. | SYSTEM_POWER_STATE_GET |
| test_s007  | Pre-Condition: SYSTEM\_POWER\_STATE\_NOTIFY support.<br /> Query system power state notify with invalid notify\_enable. | Check INVALID\_PARAMETERS status is returned. | SYSTEM\_POWER\_STATE\_NOTIFY |

Performance Domain Management Protocol Tests
---------
This section outlines the test specification for SCMI Performance Domain Management Protocol.

| Test ID    | Test Intent           | Verification Step  | Comments/Commands Used  |
| ---------- | --------------------- | ------------------ | ----------------------- |
| test_d001  | Query the protocol version information. | Check Version against expected value. | PROTOCOL\_VERSION |
| test_d002  | Query the protocol attributes. | Check performance protocol attributes against expected values. | PROTOCOL\_ATTRIBUTES |
| test_d003  | Query for mandatory command availability. | Check command implementation status. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_d004  | 1. Send invalid command id for performance protocol.<br /> 2. Query the protocol message attributes with invalid msg_id. | Check NOT\_FOUND status is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_d005  | Query performance domain attributes with valid performance domain id. | Check performance domain attributes with expected values.| PERFORMANCE\_DOMAIN\_ATTRIBUTES |
| test_d006  | Query performance domain attributes with non-existent performance domain id. | Check NOT_FOUND return status for non-existent performance domain id. | PERFORMANCE\_DOMAIN\_ATTRIBUTES |
| test_d007  | 1. Query performance describe level for all domains.<br /> 2. Query  performance describe level with invalid level index | 1. Check SUCCESS status is returned.<br /> 2. Check INVALID\_PARAMETERS status is returned.| PERFORMANCE\_DESCRIBE\_LEVELS |
| test_d008  | Query performance describe level for non-existing performance domain. | Check NOT\_FOUND status is returned.| PERFORMANCE\_DESCRIBE\_LEVELS |
| test_d009  | 1. Get current performance limits.<br /> 2. Set new performance limits.<br /> 3.Read limits and verify with new performance limits.<br /> 4. Restore the default limits. | Check SUCCESS status is returned. | PERFORMANCE\_LIMITS\_SET<br /> PERFORMANCE\_LIMITS\_GET |
| test_d010  | 1. Query with invalid maximum limit.<br /> 2. Query with invalid minimum limit.<br /> 3. Query with invalid minimum greater than maximum limit. | 1 & 2. Check OUT\_OF\_RANGE status is returned.<br /> 3. Check INVALID\_PARAMETERS status is returned. | PERFORMANCE\_LIMITS\_SET |
| test_d011  | 1. Query set limit with not-supported performance domain.<br /> 2. Query performance domain set limit with non-existent performance domain id. | 1. Check DENIED status is returned.<br /> 2. Check NOT\_FOUND status is returned. | PERFORMANCE\_LIMITS\_SET |
| test_d012  | Query performance domain get limit with non-existent performance domain id. | Check NOT\_FOUND status is returned. | PERFORMANCE\_LIMITS\_GET |
| test_d013  | 1. Get current performance level .<br /> 2. Set new performance level.<br /> 3.Read level and verify with new performance level.<br /> 4. Restore the default level. | Check SUCCESS status is returned. | PERFORMANCE\_LEVEL\_SET<br /> PERFORMANCE\_LEVEL\_GET |
| test_d014  | 1. Query with performance level greater than maximum limit.<br /> 2. Query with performance level less than minimum limit. | Check OUT\_OF\_RANGE status is returned. | PERFORMANCE\_LEVEL\_SET |
| test_d015  | 1. Query set level with not-supported performance domain.<br /> 2. Query performance domain set level  with non-existent performance domain id. | 1. Check DENIED status is returned.<br /> 2. Check NOT\_FOUND status is returned. | PERFORMANCE\_LEVEL\_SET |
| test_d016  | Query performance domain get level with non-existent performance domain id. | Check NOT\_FOUND status is returned. | PERFORMANCE\_LEVEL\_GET |
| test_d017  | Pre-Condition: PERFORMANCE\_NOTIFY\_LIMITS command support.<br /> Query with invalid notify_enable. | Check INVALID\_PARAMETERS status is returned. | PERFORMANCE\_NOTIFY\_LIMITS |
| test_d018  | Pre-Condition: PERFORMANCE\_NOTIFY\_LIMITS command support.<br /> 1. Query notify limit with not-supported performance domain.<br /> 2. Query performance domain notify limit with non-existent performance domain id. | 1. Check NOT\_SUPPORTED status is returned.<br /> 2. Check NOT\_FOUND status is returned. | PERFORMANCE\_NOTIFY\_LIMITS |
| test_d019  | Pre-Condition: PERFORMANCE\_NOTIFY\_LEVEL command support.<br /> Query with invalid notify_enable. | Check INVALID\_PARAMETERS status is returned. | PERFORMANCE\_NOTIFY\_LEVEL |
| test_d020  | Pre-Condition: PERFORMANCE\_NOTIFY\_LEVEL command support.<br /> 1. Query notify level with not-supported performance domain.<br /> 2. Query performance domain notify level with non-existent performance domain id. | 1. Check NOT\_SUPPORTED status is returned.<br /> 2. Check NOT\_FOUND status is returned. | PERFORMANCE\_NOTIFY\_LEVEL |
| test_d021  | Pre-Condition: PERFORMANCE\_DESCRIBE\_FASTCHANNEL command support.<br /> Query describe fast channel for all valid domains. | Check SUCCESS status is returned.| PERFORMANCE\_DESCRIBE\_FASTCHANNEL |
| test_d022  | Pre-Condition: PERFORMANCE\_DESCRIBE\_FASTCHANNEL command support.<br /> Query describe fast channel with non-existent performance domain id. | Check NOT\_FOUND status is returned. | PERFORMANCE\_DESCRIBE\_FASTCHANNEL |
| test_d023  | Pre-Condition: PERFORMANCE\_DESCRIBE\_FASTCHANNEL command support.<br /> Query describe fast channel with non-existent performance message id. | Check NOT\_FOUND status is returned. | PERFORMANCE\_DESCRIBE\_FASTCHANNEL |
| test_d024  | Pre-Condition: PERFORMANCE\_DESCRIBE\_FASTCHANNEL command support.<br />  Query describe fast channel with not-supported performance domain. | Check NOT\_SUPPORTED status is returned. | PERFORMANCE\_DESCRIBE\_FASTCHANNEL |
| test_d025  | Pre-Condition: PERFORMANCE\_DESCRIBE\_FASTCHANNEL command support.<br />  Query describe fast channel with not-supported message id. | Check NOT\_SUPPORTED status is returned. | PERFORMANCE\_DESCRIBE\_FASTCHANNEL |
| test_d026  |  1. Get the domain which has limit change notify and set limit support.<br /> 2. Enable limit notification.<br /> 3. Get current performance limits for the domain.<br /> 4. Set new performance limits.<br /> 5. Check if notification of new limit is received & verify limits.<br /> 6. Disable limit change notification.<br /> 7. Restore the default limits | Check SUCCESS status is returned. | PERFORMANCE\_LIMITS\_SET<br /> PERFORMANCE\_LIMITS\_GET<br /> PERFORMANCE\_NOTIFY\_LIMITS |
| test_d027  |  1. Get the domain which has level change notify and set level support.<br /> 2. Enable level notification.<br /> 3. Get current performance level for the domain.<br /> 4. Set new performance level.<br /> 5. Check if notification of new level is received & verify level.<br /> 6. Disable level change notification.<br /> 7. Restore the default level | Check SUCCESS status is returned. | PERFORMANCE\_LEVEL\_SET<br /> PERFORMANCE\_LEVEL\_GET<br /> PERFORMANCE\_NOTIFY\_LEVEL |

Clock Management Protocol Tests
---------
This section outlines the test specification for SCMI Clock Management Protocol.

| Test ID    | Test Intent           | Verification Step  | Comments/Commands Used  |
| ---------- | --------------------- | ------------------ | ----------------------- |
| test_c001  | Query the protocol version information. | Check Version against expected value. | PROTOCOL\_VERSION |
| test_c002  | Query the protocol attributes. | Check clock protocol attributes against expected values. | PROTOCOL\_ATTRIBUTES |
| test_c003  | Query for mandatory command availability. | Check command implementation status. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_c004  | 1. Send invalid command id for clock protocol.<br /> 2. Query the protocol message attributes with invalid msg_id. | Check NOT\_FOUND status is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_c005  | Query clock attributes for all system clocks. | Check clock attributes are returned. | CLOCK\_ATTRIBUTES |
| test_c006  | Query clock attributes with invalid clock id. | Check NOT\_FOUND status is returned.| CLOCK\_ATTRIBUTES |
| test_c007  | 1. Query clock describe rate for all system clocks.<br /> 2. Query clock describe rate with invalid rate index | 1. Check SUCCESS status is returned.<br /> 2. Check OUT\_OF\_RANGE status is returned.| CLOCK\_DESCRIBE\_RATES |
| test_c008  | Query clock describe rate for non-existing clock id. | Check NOT\_FOUND status is returned.| CLOCK\_DESCRIBE\_RATES |
| test_c009  | 1. Get current clock rates.<br /> 2. Set new clock rates in synchronous mode.<br /> 3. Get clock rates and verify with new rates.<br /> 4. Restore the default clock rates. | Check SUCCESS status is returned. | CLOCK\_RATE\_SET<br /> CLOCK\_RATE\_GET |
| test_c010  | 1. Get current clock rates.<br /> 2. Set new clock rates in asynchronous mode.<br /> 3. Wait   for delayed response and verify with new rates.<br /> 4. Restore the default clock rates. | Check SUCCESS status is returned. | CLOCK\_RATE\_SET<br /> CLOCK\_RATE\_GET |
| test_c011  | 1. Query set clock rate with invalid rates.<br /> 2. Query set clock rate with invalid flags. | Check INVALID\_PARAMETERS status is returned. | CLOCK\_RATE\_SET |
| test_c012  | Set clock rate for invalid clock id. | Check NOT\_FOUND status is returned. | CLOCK\_RATE\_SET |
| test_c013  | Query clock rates for all system clocks. | Check clock rates are returned. | CLOCK\_RATE\_GET |
| test_c014  | Query clock rate for invalid clock id. | Check NOT\_FOUND status is returned. | CLOCK\_RATE\_GET |
| test_c015  |  Configure clock with invalid attributes. | Check INVALID\_PARAMETERS status is returned. | CLOCK\_CONFIG\_SET |
| test_c016  | Configure clock for invalid clock id. | Check NOT\_FOUND status is returned. | CLOCK\_CONFIG\_SET |
| test_c017  | 1.Configure clock device with new state.<br /> 2. Get clock attributes and verify clock device state.<br /> 3. Restore the default clock device state | Check SUCCESS status is returned. | CLOCK\_CONFIG\_SET<br /> CLOCK\_ATTRIBUTES |

Sensor Management Protocol Tests
---------
This section outlines the test specification for SCMI Sensor Management Protocol.

| Test ID    | Test Intent           | Verification Step  | Comments/Commands Used  |
| ---------- | --------------------- | ------------------ | ----------------------- |
| test_m001  | Query the protocol version information. | Check Version against expected value. | PROTOCOL\_VERSION |
| test_m002  | Query the protocol attributes. | Check num\_sensors against expected value. | PROTOCOL\_ATTRIBUTES |
| test_m003  | Query for mandatory command availability. | Check command implementation status. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_m004  | 1. Send invalid command id for sensor protocol.<br /> 2. Query the protocol message attributes with invalid msg_id. | Check NOT\_FOUND status is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_m005  | 1. Query sensor descriptor.<br /> 2. Query sensor descriptor with invalid descriptor index. | 1. Check descriptor values against expected values. <br /> 2.Check INVALID\_PARAMETERS status is returned. | SENSOR\_DESCRIPTION\_GET |
| test_m006  | Pre-Condition: SENSOR\_TRIP\_POINT\_NOTIFY support.<br /> Query sensor trip point notify with invalid event control. | Check INVALID\_PARAMETERS status is returned. | SENSOR\_TRIP\_POINT\_NOTIFY |
| test_m007  | Pre-Condition: SENSOR\_TRIP\_POINT\_NOTIFY support.<br /> Query sensor trip point notify with invalid sensor id. | Check NOT\_FOUND status is returned. | SENSOR\_TRIP\_POINT\_NOTIFY |
| test_m008  | Pre-Condition: SENSOR\_TRIP\_POINT\_CONFIG support.<br /> 1. Query trip point config with incorrect reserve bits.<br /> 2. Query trip point config with invalid trip point id| Check INVALID\_PARAMETERS status is returned. | SENSOR\_TRIP\_POINT\_CONFIG |
| test_m009  | Pre-Condition: SENSOR\_TRIP\_POINT\_CONFIG support.<br /> Query trip point config with invalid sensor id| Check NOT\_FOUND status is returned. | SENSOR\_TRIP\_POINT\_CONFIG |
| test_m010  | Pre-Condition: SENSOR\_TRIP\_POINT\_CONFIG support.<br /> Query trip point config with valid parameters| Check SUCCESS status is returned. | SENSOR\_TRIP\_POINT\_CONFIG |
| test_m011  | Pass invalid flags value for sensor read. | Check INVALID\_PARAMETERS status is returned.| SENSOR\_READING\_GET |
| test_m012  | Read sensor data for invalid sensor id. | Check NOT\_FOUND status is returned. | SENSOR\_READING\_GET |
| test_m013  | Read sensor data in sync mode. | Check SUCCESS status is returned. | SENSOR\_READING\_GET |
| test_m014  | Pre-Condition: Sensor should support async read.<br /> Read sensor data in async mode. | 1. Check SUCCESS status is returned. <br /> 2. Wait for delayed response | SENSOR\_READING\_GET |
| test_m015  | Read sensor data in async mode for unsupported sensors. | Check NOT\_SUPPORTED status is returned. | SENSOR\_READING\_GET |

Reset Management Protocol Tests
---------
This section outlines the test specification for SCMI Reset Management Protocol.

| Test ID    | Test Intent           | Verification Step  | Comments/Commands Used  |
| ---------- | --------------------- | ------------------ | ----------------------- |
| test_r001  | Query the protocol version information. | Check Version against expected value. | PROTOCOL\_VERSION |
| test_r002  | Query the protocol attributes. | Check num of reset domains against expected value. | PROTOCOL\_ATTRIBUTES |
| test_r003  | Query for mandatory command availability. | Check command implementation status. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_r004  | 1. Send invalid command id for reset protocol.<br /> 2. Query the protocol message attributes with invalid msg_id. | Check NOT\_FOUND status is returned. | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| test_r005  | Query reset domain attributes. | Check attributes values against expected values.  | RESET\_DOMAIN\_ATTRIBUTES |
| test_r006  | Query reset domain attributes with invalid domain id. | Check NOT\_FOUND status is returned. | RESET\_DOMAIN\_ATTRIBUTES |
| test_r007  | Query reset protocol reset command with invalid domain id. | Check NOT\_FOUND status is returned. | RESET |
| test_r008  | 1. Query reset protocol reset command with invalid reserved bits. <br /> 2. Query reset protocol reset command with invalid flags. | 1. Check INVALID\_PARAMETERS status is returned. <br /> 2. Check NOT\_SUPPORTED status is returned. | RESET |
| test_r009  | Query reset protocol reset command with invalid reset_state. | Check INVALID\_PARAMETERS status is returned. | RESET |
| test_r010  | Pre-Condition: RESET\_PROTOCOL\_NOTIFY support.<br /> Query reset protocol notify with invalid domain id. |  Check NOT\_FOUND status is returned.  | RESET\_PROTOCOL\_NOTIFY |
| test_r011  | Pre-Condition: RESET\_PROTOCOL\_NOTIFY support.<br /> Query reset protocol notify with invalid notify enable. |  Check INVALID\_PARAMETERS status is returned.  | RESET\_PROTOCOL\_NOTIFY |

- - - - - - - - - - - - - - - - - - - -

_Copyright (c) 2019-2020, Arm Limited and Contributors. All rights reserved._
