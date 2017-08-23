**SCMI Tests - Test Specification**
===================================
Table of Contents:
- [Introduction](#introduction)
- [General Rules](#general-rules)
- [Base Protocol Tests](#base-protocol-tests)
- [Power Domain Management Protocol Tests](#power-domain-management-protocol-tests)
- [Performance Domain Management Protocol Tests](#performance-domain-management-protocol-tests)
- [System Power Domain Management Protocol Tests](#system-power-domain-management-protocol-tests)
- [Clock Management Protocol Tests](#clock-management-protocol-tests)
- [Sensor Management Protocol Tests](#sensor-management-protocol-tests)

Introduction
-------
This outlines the test specification detailing the individual test cases designed and implemented as part of current version of the scmi test suite.

General Rules
-------

> The test case identifiers **must** follow a common naming pattern as [*protocol*]-{*command*}-*descriptive\_name*-*numeric\_id*
> The command part of the test case identifier **must** be defined same as the protocol command name described in the SCMI specification.

----------

> All test case identifiers **must** be unique.
> The test designers should ensure that if there are multiple test case id's with same protocol,  command and descriptive-name fields then the numeric-id should be made different to make it unique.

----------

> Every test case compares return values against expected results provided by user via the platform properties to determine a PASS/FAIL verdict. If  expected result is unspecified then the return values will be reported as 'INFO' without any PASS/FAIL verdict

----------

> All message fields defined by specification as 'Reserved, Must be Zero' **must** be checked and reported as compliance failure if not adhered to.

----------

> A collection of [test cases](http://glossary.astqb.org/search/test%20case) is defined as a [test suite](http://glossary.astqb.org/search/test%20suite).
>
> The scmi test library comprises of a collection of test suites grouped per protocol.
> The first test suite in each of those groups **must** have test cases targeting PROTOCOL\_VERSION, PROTOCOL\_ATTRIBUTES, PROTOCOL\_MESSAGE\_ATTRIBUTES commands for the given protocol. This allows the discovery of the command support and determines the ability or need to execute the remainder of the test suites in the group.
> The remaining test suites **should** be organised on a per command basis to enable the discovery based test execution.

----------

> If there are test cases needing a **pre-condition** or a **post-condition** they are captured as separate test cases which **must** come  **just before** or **just after** the relevant test case in the test suite.  Ensuring the correct positioning of the precondition or post condition test cases within the test suite is a test developer responsibility. Such test cases **must** have 'precondition_' or 'postcondition_' prefix in the test case name followed by the descriptive name of the original test case for which this relates to.

> A typical example for a test case needing a precondition is the get command which will have a set command as a precondition.

> A typical example for a test case needing a postcondition is a subscribe for notification command which will have unsubscribe for notification command as a postcondition.

Base Protocol Tests
----------
This section outlines the test specification for SCMI Base Protocol.

| Test Case ID        | Test Aim           | Verification Step  | SCMI Command |
| ------------------- | ------------------ | --------- | ----------- |
| [base]-{PROTOCOL\_VERSION}-query\_protocol\_version-01      | Query the protocol version information | Check Version against expected value | PROTOCOL\_VERSION  |
| [base]-{PROTOCOL\_ATTRIBUTES}-query\_protocol\_attributes-01 | Query the protocol attributes | Check num\_agents and num\_protocols against expected value | PROTOCOL\_ATTRIBUTES |
| [base]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_vendor\_disc\_cmd\_support-01 | Query for mandatory BASE\_DISCOVER\_VENDOR command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [base]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_subvendor\_disc\_cmd\_support-01 | Query for optional  BASE\_DISCOVER\_SUB\_VENDOR command availability |    Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [base]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_impl\_version\_cmd\_support-01 | Query for mandatory BASE\_DISCOVER\_IMPLEMENTATION\_VERSION command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [base]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_proto\_list\_cmd\_support-01 | Query for mandatory BASE\_DISCOVER\_LIST\_PROTOCOLS command availability      | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [base]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_agent\_disc\_cmd\_support-01 | Query for optional  BASE\_DISCOVER\_AGENT command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [base]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_notify\_error\_cmd\_support-01 | Query for mandatory BASE\_NOTIFY\_ERRORS command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [base]-{BASE\_DISCOVER\_VENDOR}-query\_vendor\_info-01 | Query for vendor information | Check vendor name against expected value | BASE\_DISCOVER\_VENDOR |
| [base]-{BASE\_DISCOVER\_SUB\_VENDOR}-query\_subvendor\_info-01 | Query for sub-vendor information | Check sub-vendor name against expected value | BASE\_DISCOVER\_SUB\_VENDOR |
| [base]-{BASE\_DISCOVER\_IMPLEMENTATION\_VERSION}-query\_implementation\_ver-01 | Query for vendor specific implementation version information | Check implementation version against expected value | BASE\_DISCOVER\_IMPLEMENTATION\_VERSION |
| [base]-{BASE\_DISCOVER\_LIST\_PROTOCOLS}-query\_protocol\_list-01 | Query the list of supported protocols |    Check protocol id list against expected value | BASE\_DISCOVER\_LIST\_PROTOCOLS |
| [base]-{BASE\_DISCOVER\_LIST\_PROTOCOLS}-query\_invalid\_skipindex-01 | Query for protocol list with invalid skip parameter | Check command status against error code defined in specification | BASE\_DISCOVER\_LIST\_PROTOCOLS |
| [base]-{BASE\_DISCOVER\_AGENT}-query\_agent\_id0-01 | Query for agent information for agent with id 0 | Check if return value start with "platform" as defined in specification | BASE\_DISCOVER\_AGENT |
| [base]-{BASE\_DISCOVER\_AGENT}-query\_non\_zero\_agentid-01 | Query agent information for all agents other than platform id 0 | Check agent name against expected value | BASE\_DISCOVER\_AGENT |
| [base]-{BASE\_DISCOVER\_AGENT}-query\_invalid\_agentid-01 | Query for agent information with invalid agent id | Check command status against error code defined in specification | BASE\_DISCOVER\_AGENT |
| [base]-{BASE\_NOTIFY\_ERRORS}-reg\_err\_notification-01 | Register for error notification from platform  PostCondition: Unregister for error notification | Check return status against SUCCESS as per specification | BASE\_NOTIFY\_ERRORS |
| [base]-{BASE\_NOTIFY\_ERRORS}-postcondition\_reg\_err\_notification-01 | This is a post condition step for the previous test case that registers for error notification from platform. In this post condition step it will unregister for the notifications | Check return status against SUCCESS as per specification | BASE\_NOTIFY\_ERRORS |

Power Domain Management Protocol Tests
---------
This section outlines the test specification for SCMI Power Domain Management Protocol.

| Test Case ID        | Test Aim           | Verification Step  | SCMI Command |
| ------------------- | ------------------ | --------- | ----------- |
| [power]-{PROTOCOL\_VERSTION}-query\_protocol\_version-01 | Query the protocol version information | Check Version against expected value | PROTOCOL\_VERSION |
| [power]-{PROTOCOL\_ATTRIBUTES}-query\_protocol\_attributes-01 | Query the protocol attributes | Check num\_power\_domains, statistics support and shared memory region details against expected values | PROTOCOL\_ATTRIBUTES |
| [power]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_power\_domain\_attrib\_cmd\_support-01 | Query for mandatory POWER\_DOMAIN\_ATTRIBUTES command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [power]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_power\_state\_set\_cmd\_support-01 | Query for mandatory POWER\_STATE\_SET command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [power]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_power\_state\_get\_cmd\_support-01 | Query for mandatory POWER\_STATE\_GET command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [power]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_power\_state\_notify\_cmd\_support-01 | Query for optional POWER\_STATE\_NOTIFY command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [power]-{POWER\_DOMAIN\_ATTRIBUTES}-query\_power\_domain\_attributes\_alldomainid-01 | Query for power domain attributes of all supported power domain ids | Check power domain attributes about notifications support, async/sync set command support and name of the power domain against expected values | POWER\_DOMAIN\_ATTRIBUTES |
| [power]-{POWER\_DOMAIN\_ATTRIBUTES}-query\_non\_existant\_powerdomainid-01 | Query for attributes about a non-existant power domain  | Check command status against error code defined in specification | POWER\_DOMAIN\_ATTRIBUTES |
| [power]-{POWER\_STATE\_SET}-power\_state\_set\_non\_existant\_domainid-01 | Set power state on a non-existant power domain | Check command status against error code defined in specification | POWER\_STATE\_SET  |
| [power]-{POWER\_STATE\_GET}-power\_state\_get\_allpowerdomainid-01 | Get current power state information across all power domains | Report power state retrieved as Info | POWER\_STATE\_GET |
| [power]-{POWER\_STATE\_GET}-power\_state\_get\_invalid\_powerdomainid-01 | Get current power state information from a non-existant power domain | Check command status against error code defined in specification | POWER\_STATE\_GET |
| [power]-{POWER\_STATE\_NOTIFY}-register\_power\_state\_notify\_allpowerdomainid-01 | Register for power state notifications across all power domains. PostCondition: Unregister for power state notification | Report power state notification command status as Info | POWER\_STATE\_NOTIFY |
| [power]-{POWER\_STATE\_NOTIFY}-postcondition\_register\_power\_state\_notify\_allpowerdomainid-01 | This is a post condition step for the previous test case which registers for power state notifications across all power domains. In this post condition step it will unregister for the notifications | Report power state notification command status as Info | POWER\_STATE\_NOTIFY |
| [power]-{POWER\_STATE\_NOTIFY}-register\_power\_state\_notify\_invalid\_powerdomainid-01 | Issue power state notification command on an invalid power domain | Check command status against error code defined in specification | POWER\_STATE\_NOTIFY |

Performance Domain Management Protocol Tests
---------
This section outlines the test specification for SCMI Performance Domain Management Protocol.

| Test Case ID        | Test Aim           | Verification Step  | SCMI Command |
| ------------------- | ------------------ | --------- | ----------- |
| [performance]-{PROTOCOL\_VERSION}-query\_protocol\_version-01 | Query the protocol version information | Check Version against expected value | PROTOCOL\_VERSION |
| [performance]-{PROTOCOL\_ATTRIBUTES}-query\_protocol\_attributes-01 | Query the protocol attributes | Check power value unit, statistics support and shared memory region details against expected values | PROTOCOL\_ATTRIBUTES |
| [performance]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_performance\_domain\_attrib\_cmd\_support-01 | Query for mandatory PERFORMANCE\_DOMAIN\_ATTRIBUTES command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [performance]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_performance\_describe\_levels\_cmd\_support-01 | Query for mandatory PERFORMANCE\_DESCRIBE\_LEVELS command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [performance]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_performance\_limits\_set\_cmd\_support-01 | Query for mandatory PERFORMANCE\_LIMITS\_SET command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [performance]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_performance\_limits\_get\_cmd\_support-01 | Query for mandatory PERFORMANCE\_LIMITS\_GET command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [performance]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_performance\_level\_set\_cmd\_support-01 | Query for mandatory PERFORMANCE\_LEVEL\_SET command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [performance]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_performance\_level\_get\_cmd\_support-01 | Query for mandatory PERFORMANCE\_LEVEL\_GET command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [performance]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_performance\_notify\_limits\_cmd\_support-01 | Query for optional  PERFORMANCE\_NOTIFY\_LIMITS command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [performance]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_performance\_notify\_level\_cmd\_support-01 | Query for optional  PERFORMANCE\_NOTIFY\_LEVEL command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [performance]-{PERFORMANCE\_DOMAIN\_ATTRIBUTES}-query\_performance\_domain\_attributes\_alldomainid-01 | Query for information about the all available performance domains from the  platform | Check performance domain attributes about level and limit change notifications support, ability to set performance limit and level and name of the performance domain against expected values | PERFORMANCE\_DOMAIN\_ATTRIBUTES |
| [performance]-{PERFORMANCE\_DOMAIN\_ATTRIBUTES}-query\_non\_existant\_performance\_domainid-01 | Query for attributes about a non-existant performance domain  | Check command status against error code defined in specification | PERFORMANCE\_DOMAIN\_ATTRIBUTES |
| [performance]-{PERFORMANCE\_DESCRIBE\_LEVELS}-query\_performance\_describe\_levels\_alldomainid-01 | Query for performance level information across all performance domains  | Check performance level value, power cost and worst-case latency against expected values | PERFORMANCE\_DESCRIBE\_LEVELS |
| [performance]-{PERFORMANCE\_DESCRIBE\_LEVELS}-query\_performance\_describe\_levels\_non\_existant\_domain-01 | Query for describing performance levels of a non-existant performance domain  | Check command status against error code defined in specification | PERFORMANCE\_DESCRIBE\_LEVELS |
| [performance]-{PERFORMANCE\_LIMITS\_SET}-performance\_limits\_set\_alldomainid-01 | Set performance limit on all  performance domains. This will set a new minimum limit as the lowest level + 1 and new maximum limit as highest level - 1, where lowest and highest levels are obtained from previous describe levels test on the corresponding performance domain | Check command status against error code defined in specification if agent is not allowed to set performance limit on a performance domain; Otherwise check command success status | PERFORMANCE\_LIMITS\_SET |
| [performance]-{PERFORMANCE\_LIMITS\_SET}-performance\_limits\_set\_invalid\_domainid-01 | Set performance limit on a non-existant performance domain | Check command status against error code defined in specification | PERFORMANCE\_LIMITS\_SET |
| [performance]-{PERFORMANCE\_LIMITS\_SET}-performance\_limits\_set\_over\_range-01 | Set performance limit on a  performance domain where the maximum limit is set as highest level + 1, where highest level is obtained from previous describe levels test on the corresponding  performance domain | Check command status against error code defined in specification | PERFORMANCE\_LIMITS\_SET |
| [performance]-{PERFORMANCE\_LIMITS\_SET}-performance\_limits\_set\_under\_range-01 | Set performance limit on a  performance domain where the minimum limit is set as lowest level - 1, where lowest level is obtained from previous describe levels test on the corresponding  performance domain | Check command status against error code defined in specification | PERFORMANCE\_LIMITS\_SET |
| [performance]-{PERFORMANCE\_LIMITS\_SET}-precondition\_performance\_limits\_get\_alldomainid-01 | This is a precondition step for next test case, which get the performance limit for the allowed domain. In this precondition step it sets the performance limit to a known valid value. | Check command status and wait for PERFORMANCE\_LIMITS\_CHANGED notification if PERFORMANCE\_NOTIFY\_LIMITS is supported or if that notification mechanism is not supported wait for a set amount of time before proceeding to next test case | PERFORMANCE\_LIMITS\_SET |
| [performance]-{PERFORMANCE\_LIMITS\_GET}-performance\_limits\_get\_alldomainid-01 | Get performance limit for the allowed performance domain. PreCondition: Set the performance limit to a known value | Check the maximum and minimum limit obtained is same as the one set in the previous precondition test | PERFORMANCE\_LIMITS\_GET |
| [performance]-{PERFORMANCE\_LIMITS\_GET}-performance\_limits\_get\_invalid\_domainid-01 | Get performance limit for a non-existant performance domain | Check command status against error code defined in specification | PERFORMANCE\_LIMITS\_GET |
| [performance]-{PERFORMANCE\_LEVEL\_SET}-performance\_level\_set\_alldomainid-01 | Set performance level on all  performance domains. This will set a new desired level  as the lowest level + 1, where lowest level is obtained from previous describe levels test on the corresponding performance domain | Check command status against error code defined in specification if agent is not allowed to set performance level on a performance domain; Otherwise check command success status | PERFORMANCE\_LEVEL\_SET |
| [performance]-{PERFORMANCE\_LEVEL\_SET}-performance\_level\_set\_invalid\_domainid-01 | Set performance level on a non-existant performance domain | Check command status against error code defined in specification | PERFORMANCE\_LEVEL\_SET |
| [performance]-{PERFORMANCE\_LEVEL\_SET}-performance\_level\_set\_invalid\_level-01 | Set an invalid performance level on an allowed performance domain by specifying the level to set as highest level + 1, where highest level is obtained from previous describe levels test on the corresponding performance domain | Check command status against error code defined in specification | PERFORMANCE\_LEVEL\_SET |
| [performance]-{PERFORMANCE\_LEVEL\_SET}-precondition\_performance\_level\_get\_alldomain-01 | This is a precondition step for next test case, which get the performance level for the allowed domain. In this precondition step it sets the performance level to a known valid value. | Check command status and wait for PERFORMANCE\_LEVEL\_CHANGED notification if PERFORMANCE\_NOTIFY\_LEVELS is supported or if that notification mechanism is not supported wait for a set amount of time before proceeding to next test case | PERFORMANCE\_LEVEL\_SET |
| [performance]-{PERFORMANCE\_LEVEL\_GET}-performance\_level\_get\_alldomainid-01 | Get performance level for the allowed performance domain. PreCondition: Set the performance level to a known value | Check the level obtained is same as the one set in the previous precondition test. This test will give valid values only if this test agent is the only agent active during the test session | PERFORMANCE\_LEVEL\_GET |
| [performance]-{PERFORMANCE\_LEVEL\_GET}-performance\_level\_get\_invalid\_domainid-01 | Get performance level for a non-existant performance domain | Check command status against error code defined in specification | PERFORMANCE\_LEVEL\_GET |

System Power Domain Management Protocol Tests
---------
This section outlines the test specification for SCMI System Power Domain Management Protocol.

| Test Case ID        | Test Aim           | Verification Step  | SCMI Command |
| ------------------- | ------------------ | --------- | ----------- |
| [system\_power]-{PROTOCOL\_VERSION}-query\_protocol\_version-01 | Query the protocol version information | Check Version against expected value | PROTOCOL\_VERSION |
| [system\_power]-{PROTOCOL\_ATTRIBUTES}-query\_protocol\_attributes-01 | Query the protocol attributes | Check command return status | PROTOCOL\_ATTRIBUTES |
| [system\_power]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_system\_power\_state\_set\_cmd\_support-01 | Query for mandatory SYSTEM\_POWER\_STATE\_SET command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [system\_power]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_system\_power\_state\_get\_cmd\_support-01 | Query for **conditionally** mandatory SYSTEM\_POWER\_STATE\_GET command availability. This becomes mandatory only for a management agent in a system implementing OSPM view | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [system\_power]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_system\_power\_state\_notify\_cmd\_support-01 | Query for **conditionally** mandatory SYSTEM\_POWER\_STATE\_NOTIFY command availability. This becomes mandatory only for systems implementing OSPM view | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [system\_power]-{SYSTEM\_POWER\_STATE\_GET}-system\_power\_state\_get-01 | This is a **negative** test query for system power state; except when the test agent is configured as a management agent | Check command status against error code defined in specification or Success status in case of test agent configured as management agent | SYSTEM\_POWER\_STATE\_GET |

Clock Management Protocol Tests
-------
This section outlines the test specification for SCMI Clock Management Protocol.

| Test Case ID        | Test Aim           | Verification Step  | SCMI Command |
| ------------------- | ------------------ | --------- | ----------- |
| [clock]-{PROTOCOL\_VERSION}-query\_protocol\_version-01 | Query the protocol version information | Check Version against expected value | PROTOCOL\_VERSION |
| [clock]-{PROTOCOL\_ATTRIBUTES}-query\_protocol\_attributes-01 | Query the protocol attributes | Check number of clocks and maximum asynchronous clock rate change information against expected values | PROTOCOL\_ATTRIBUTES |
| [clock]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_clock\_attribute\_cmd\_support-01 | Query for mandatory CLOCK\_ATTRIBUTES command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [clock]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_clock\_describe\_rates\_cmd\_support-01 | Query for mandatory CLOCK\_DESCRIBE\_RATES command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [clock]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_clock\_rate\_set\_cmd\_support-01 | Query for mandatory CLOCK\_RATE\_SET command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [clock]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_clock\_rate\_get\_cmd\_support-01 | Query for mandatory CLOCK\_RATE\_GET command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [clock]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_clock\_config\_set\_cmd\_support-01 | Query for mandatory CLOCK\_CONFIG\_SET command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [clock]-{CLOCK\_ATTRIBUTES}-query\_clock\_attributes\_allclockid-01 | Query for attributes for all visible clock ids | Check the clock name and clock enable status against the expected value | CLOCK\_ATTRIBUTES |
| [clock]-{CLOCK\_ATTRIBUTES}-query\_non\_existant\_clock-01 | Query for clock attributes of a non-existant clock | Check command status against error code defined in specification | CLOCK\_ATTRIBUTES |
| [clock]-{CLOCK\_DESCRIBE\_RATES}-query\_allowed\_clock\_rates-01 | Query for clock rates for the allowed clock | Check the rate format and the clock rates against the expected value | CLOCK\_DESCRIBE\_RATES |
| [clock]-{CLOCK\_DESCRIBE\_RATES}-query\_non\_existant\_clockid-01 | Query for clock rates of a non-existant clock | Check command status against error code defined in specification | CLOCK\_DESCRIBE\_RATES |
| [clock]-{CLOCK\_CONFIG\_SET}-precondition\_clock\_rate\_set\_allowed\_clocks-01 | This is a preconditon step for next test case, which set rates for allowed clocks. In this precondition step it configures clock to be enabled for setting the rate. | Check command status for success | CLOCK\_CONFIG\_SET |
| [clock]-{CLOCK\_RATE\_SET}-clock\_rate\_set\_allowed\_clocks-01 | Set clock rate for the allowed clocks to a rate one step higher than the minimum rate supported by a given clock. This request should be made synchronously | Check command status for success | CLOCK\_RATE\_SET |
| [clock]-{CLOCK\_RATE\_SET}-clock\_rate\_set\_non\_existant\_clockid-01 | Set clock rate for a non-existant clock | Check command status against error code defined in specification | CLOCK\_RATE\_SET |
| [clock]-{CLOCK\_RATE\_SET}-precondition\_clock\_rate\_get\_allowed\_clocks-01 | This is a precondition step for next test case, which get the clock rate for the allowed clocks. In this precondition step it sets the clock rate to a known valid value. This command is to be issued synchronously | Check command status for success | CLOCK\_RATE\_SET |
| [clock]-{CLOCK\_RATE\_GET}-clock\_rate\_get\_allowed\_clocks-01 | Get clock rate for the allowed clocks. Precondition: Set the clock rate to a known value| Check the rate obtained is same as the one set in the previous precondition test. | CLOCK\_RATE\_GET |
| [clock]-{CLOCK\_RATE\_GET}-clock\_rate\_get\_non\_existant\_clockid-01 | Get clock rate for a non-existant clock | Check command status against error code defined in specification | CLOCK\_RATE\_GET |
| [clock]-{CLOCK\_CONFIG\_SET}-clock\_config\_set\_enabled\_allowed_clocks-01 | Set the clock configuration to enable all the allowed clocks. PostCondition: Set the clock status to the original state as discovered from clock attributes | Check command status for SUCCESS | CLOCK\_CONFIG\_SET |
| [clock]-{CLOCK\_CONFIG\_SET}-postcondition_clock\_config\_set\_enabled\_allowed_clocks-01 | This post condition test case will set the clock status to the original state as discovered from clock attributes | Check command status for SUCCESS | CLOCK\_ATTRIBUTES |
| [clock]-{CLOCK\_CONFIG\_SET}-clock\_config\_set\_disabled\_allowed_clocks-01 | Set the clock configuration to disable all the allowed clocks. PostCondition: Set the clock status to the original state as discovered from clock attributes | Check command status for SUCCESS | CLOCK\_CONFIG\_SET |
| [clock]-{CLOCK\_CONFIG\_SET}-postcondition_clock\_config\_set\_disabled\_allowed_clocks-01 | This postcondition test case will set the clock status to the original state as discovered from clock attributes | Check command status for SUCCESS | CLOCK\_ATTRIBUTES |

Sensor Management Protocol Tests
-------
This section outlines the test specification for SCMI Sensor  Management Protocol.

| Test Case ID        | Test Aim           | Verification Step  | SCMI Command |
| ------------------- | ------------------ | --------- | ----------- |
| [sensor]-{PROTOCOL\_VERSION}-query\_protocol\_version-01 | Query the protocol version information | Check Version against expected value | PROTOCOL\_VERSION |
| [sensor]-{PROTOCOL\_ATTRIBUTES}-query\_protocol\_attributes-01 | Query the protocol attributes | Check number of sensors, maximum asynchronous commands supported and shared memory region details against expected values | PROTOCOL\_ATTRIBUTES |
| [sensor]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_sensor\_description\_get\_cmd\_support-01 | Query for mandatory SENSOR\_DESCRIPTON\_GET command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [sensor]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_sensor\_trip\_point\_notify\_cmd\_support-01 | Query for optional  SENSOR\_TRIP\_POINT\_NOTIFY command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [sensor]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_sensor\_reading\_get\_cmd\_support-01 | Query for mandatory SENSOR\_READING\_GET command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [sensor]-{PROTOCOL\_MESSAGE\_ATTRIBUTES}-query\_sensor\_config\_set\_cmd\_support-01 | Query for **conditionally** mandatory SENSOR\_CONFIG\_SET command availability | Check command implementation status | PROTOCOL\_MESSAGE\_ATTRIBUTES |
| [sensor]-{SENSOR\_DESCRIPTION\_GET}-query\_sensor\_description\_allsensorid-01 | Query for sensor descriptors for all sensors in the platform | Check the number of sensor trip points, supported for shared memory, sensor updated interval, sensor resolution, sensor type, sensor type unit and sensor name against the expected value | SENSOR\_DESCRIPTION\_GET |
| [sensor]-{SENSOR\_DESCRIPTION\_GET}-query\_sensor\_description\_get\_non\_existant\_sensorid-01 | Query for sensor descriptors for  a non-existant sensor | Check command status against error code defined in specification | SENSOR\_DESCRIPTION\_GET |
| [sensor]-{SENSOR\_TRIP\_POINT\_NOTIFY}-set\_sensor\_trip\_point\_notification\_allsensorid-01 | Set trip point notification on all sensors.  This will set the event notification to disabled | Check command success status | SENSOR\_TRIP\_POINT\_NOTIFY |
| [sensor]-{SENSOR\_TRIP\_POINT\_NOTIFY}-set\_sensor_trip\_point\_notification\_non\_existant\_sensorid-01 | Set sensor trip point notification on a non-existant sensor | Check command status against error code defined in specification | SENSOR\_TRIP\_POINT\_NOTIFY |
| [sensor]-{SENSOR\_TRIP\_POINT\_CONFIG}-set\_sensor\_trip\_point\_config\_allsensorid-01 | Disable event generation for all supported trip points across all sensors | Check command status for success | SENSOR\_TRIP\_POINT\_CONFIG |
| [sensor]-{SENSOR\_TRIP\_POINT\_CONFIG}-set\_sensor\_trip\_point\_config\_non\_existant\_sensorid-01 | Set trip point config on a non-existant sensor | Check command status against error code defined in specification | SENSOR\_TRIP\_POINT\_CONFIG |
| [sensor]-{SENSOR\_READING\_GET}-sensor\_reading\_get\_allsensorid-01 | Read sensor value from all sensors. This is done in synchronous mode | Check command success status| SENSOR\_READING\_GET |
| [sensor]-{SENSOR\_READING\_GET}-sensor\_reading\_get\_non\_existant\_sensorid-01 | Read sensor value from a non-existant sensor. | Check command status against error code defined in specification | SENSOR\_READING\_GET |

- - - - - - - - - - - - - - - - - - - -

_Copyright (c) 2017, ARM Limited and Contributors. All rights reserved._
