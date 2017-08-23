/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef POWER_COMMON_H_
#define POWER_COMMON_H_

#include <common.h>
#include <base_expected.h>
#include <test_agent_exports.h>

#define PWR_PROTO_ID 0x11
#define PWR_PROTO_VER_MSG_ID 0x0
#define PWR_PROTO_ATTR_MSG_ID 0x1
#define PWR_PROTO_MSG_ATTR_MSG_ID 0x2
#define PWR_DOMAIN_ATTRIB_MSG_ID 0x3
#define PWR_STATE_SET_MSG_ID 0x4
#define PWR_STATE_GET_MSG_ID 0x5
#define PWR_STATE_NOTIFY_MSG_ID 0x6
#define PWR_NOTIFY_ENABLE_SET 1
#define PWR_NOTIFY_ENABLE_UNSET 0

/* Power Test Suites index names */
enum power_test_suite_index{
	TS_POWER_DISCOVERY,
	TS_POWER_DOMAIN_ATTRIBUTES,
	TS_POWER_STATE_SET,
	TS_POWER_STATE_GET,
	TS_POWER_STATE_NOTIFY
};

struct arm_scmi_power_protocol_version {
	struct  {
		uint32_t version;
	} returns;
};
struct arm_scmi_power_protocol_attributes {
	struct {
		uint32_t attributes;
		uint32_t statistics_address_low;
		uint32_t statistics_address_high;
		uint32_t statistics_len;
	} returns;
	/* all the enums must be at the end */
	enum {
		POWER_PROT_ATTR_NUMBER_DOMAINS_LOW=0,
		POWER_PROT_ATTR_NUMBER_DOMAINS_HIGH=15,
		POWER_PROT_ATTR_PROT_RESERVED_LOW=16,
		POWER_PROT_ATTR_PROT_RESERVED_HIGH=31
	} attributes_bits;
};
struct arm_scmi_power_protocol_message_attributes {
	struct {
		uint32_t message_id;
	} parameters;
	struct {
		uint32_t attributes;
	} returns;
};
struct arm_scmi_power_domain_attributes {
	struct {
		uint32_t domain_id;
	} parameters;
	struct {
		uint32_t attributes;
		uint8_t name[SCMI_NAME_STR_SIZE];
	} returns;
	/* all the enums must be at the end */
	enum {
		POWER_DOMAIN_ATTR_RESERVED_LOW=0,
		POWER_DOMAIN_ATTR_RESERVED_HIGH=28,
		POWER_DOMAIN_ATTR_SYNC_LOW=29,
		POWER_DOMAIN_ATTR_SYNC_HIGH=29,
		POWER_DOMAIN_ATTR_ASYNC_LOW=30,
		POWER_DOMAIN_ATTR_ASYNC_HIGH=30,
		POWER_DOMAIN_ATTR_STATE_NOTIFICATIONS_LOW=31,
		POWER_DOMAIN_ATTR_STATE_NOTIFICATIONS_HIGH=31
	} attributes_bits;
};
struct arm_scmi_power_state_set {
	struct {
		uint32_t flags;
		uint32_t domain_id;
		uint32_t power_state;
	} parameters;
	/* all the enums must be at the end */
	enum {
		POWER_STATE_SET_RESERVED_LOW=1,
		POWER_STATE_SET_RESERVED_HIGH=31,
		POWER_STATE_SET_ASYNC_LOW=0,
		POWER_STATE_SET_ASYNC_HIGH=0
	} flags_bits;
};
struct arm_scmi_power_state_get {
	struct {
		uint32_t domain_id;
	} parameters;
	struct {
		uint32_t power_state;
	} returns;
};
struct arm_scmi_power_state_notify {
	struct {
		uint32_t domain_id;
		uint32_t notify_enable;
	} parameters;
	/* all the enums must be at the end */
	enum {
		POWER_STATE_NOTIFY_RESERVED_LOW=1,
		POWER_STATE_NOTIFY_RESERVED_HIGH=31,
		POWER_STATE_NOTIFY_ENABLE_LOW=0,
		POWER_STATE_NOTIFY_ENABLE_HIGH=0
	} power_notify_bits;
};

extern struct arm_scmi_test_suite arm_scmi_power_test_suites[];
extern const size_t arm_scmi_power_test_suites_size;
extern struct arm_scmi_protocol_report arm_scmi_power_protocol_report;
void arm_scmi_post_processing_power_protocol_attributes(
		struct arm_scmi_protocol_execution_context *, arm_scmi_test_case_t *,
		struct arm_scmi_message *);
void arm_scmi_post_processing_power_protocol_message_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case, struct arm_scmi_message *received);
void arm_scmi_custom_test_power_domain_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case);
void arm_scmi_custom_test_power_state_get(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case);
void arm_scmi_custom_test_power_state_set(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case);
void arm_scmi_custom_test_power_state_notify(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case);
#endif /* POWER_COMMON_H_ */
