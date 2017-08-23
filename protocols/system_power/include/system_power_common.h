/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef SYSTEM_POWER_COMMON_H_
#define SYSTEM_POWER_COMMON_H_

#include <common.h>
#include <test_agent_exports.h>
#include <system_power_expected.h>

#define SYSTEM_POWER_PROTO_ID 0x12
#define SYSTEM_POWER_PROTO_VER_MSG_ID 0x0
#define SYSTEM_POWER_PROTO_ATTRIB_MSG_ID 0x1
#define SYSTEM_POWER_PROTO_MSG_ATTRIB_MSG_ID 0x2
#define SYSTEM_POWER_STATE_SET_MSG_ID 0x3
#define SYSTEM_POWER_STATE_GET_MSG_ID 0x4
#define SYSTEM_POWER_STATE_NOTIFY_MSG_ID 0x5

/* Power Test Suites index names */
enum system_power_test_suite_index{
	TS_SYSTEM_DISCOVERY,
	TS_SYSTEM_POWER_STATE_GET
};

struct arm_scmi_system_power_protocol_version {
	struct  {
		uint32_t version;
	} returns;
};

struct arm_scmi_system_power_protocol_attributes {
	struct {
		uint32_t attributes;
	} returns;
	/* all the enums must be at the end */
	enum {
		SYSTEM_POWER_PROTOCOLS_ATTR_RESERVED_LOW=0,
		SYSTEM_POWER_PROTOCOLS_ATTR_RESERVED_HIGH=31
	}attributes_bits;
};

struct arm_scmi_system_power_protocol_message_attributes {
	struct {
		uint32_t message_id;
	} parameters;
	struct {
		uint32_t attributes;
	} returns;
	enum {
		SYSTEM_POWER_MSG_PROTO_ATTRIB_RESERVED_LOW=0,
		SYSTEM_POWER_MSG_PROTO_ATTRIB_RESERVED_HIGH=29,
		SYSTEM_POWER_MSG_PROTO_ATTRIB_SUSPEND=30,
		SYSTEM_POWER_MSG_PROTO_ATTRIB_WARMREST=31
	} system_power_message_attributes_bits;
};

struct arm_scmi_system_power_state_set {
	struct {
		uint32_t flags;
		uint32_t system_state;
	} parameters;
	enum {
		SYSTEM_POWER_REQUEST_TYPE_LOW=0,
		SYSTEM_POWER_REQUEST_TYPE_HIGH=0,
		SYSTEM_POWER_REQUEST_RESERVED_LOW=1,
		SYSTEM_POWER_REQUEST_RESERVED_HIGH=31
	} system_power_request_flags;
};


extern struct arm_scmi_test_suite arm_scmi_system_power_test_suites[];
extern const size_t arm_scmi_system_power_test_suites_size;
extern struct arm_scmi_protocol_report arm_scmi_system_power_protocol_report;
void arm_scmi_post_processing_system_power_protocol_message_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case, struct arm_scmi_message *received);
void arm_scmi_custom_tester_system_power_state_set(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case);

#endif /* SYSTEM_POWER_COMMON_H_ */
