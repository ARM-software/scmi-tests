/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef PERFORMANCE_COMMON_H_
#define PERFORMANCE_COMMON_H_

#include <common.h>
#include <performance_expected.h>
#include <test_agent_exports.h>

#define PERF_MGMT_PROTO_ID 0x13
#define PERF_MGMT_PROTO_VER_MSG_ID 0x0
#define PERF_MGMT_PROTO_ATTRIB_MSG_ID 0x1
#define PERF_MGMT_PROTO_MSG_ATTRIB_MSG_ID 0x2
#define PERF_DOMAIN_ATTRIB_MSG_ID 0x3
#define PERF_DESC_LVL_MSG_ID 0x4
#define PERF_LIMIT_SET_MSG_ID 0x5
#define PERF_LIMIT_GET_MSG_ID 0x6
#define PERF_LVL_SET_MSG_ID 0x7
#define PERF_LVL_GET_MSG_ID 0x8
#define PERF_NOTIFY_LIMIT_MSG_ID 0x9
#define PERF_NOTIFY_LVL_MSG_ID 0xA
#define MAX_PERFORMANCE_DOMAIN_COUNT 10

/* Performance Test Suites index names */
enum performance_test_suite_index{
	TS_PERFORMANCE_DISCOVERY,
	TS_PERFORMANCE_DOMAIN_ATTRIBUTES,
	TS_PERFORMANCE_DESCRIBE_LEVELS,
	TS_PERFORMANCE_LIMITS_SET,
	TS_PERFORMANCE_LIMITS_GET,
	TS_PERFORMANCE_LEVEL_SET,
	TS_PERFORMANCE_LEVEL_GET
};

struct arm_scmi_performance_protocol_version {
	struct  {
		uint32_t version;
	} returns;
};
struct arm_scmi_performance_protocol_attributes {
	struct {
		uint32_t attributes;
		uint32_t statistics_address_low;
		uint32_t statistics_address_high;
		uint32_t statistics_len;
	} returns;
	/* all the enums must be at the end */
	enum  {
		PERFORMANCE_PROT_ATTR_NUMBER_DOMAINS_LOW=0,
		PERFORMANCE_PROT_ATTR_NUMBER_DOMAINS_HIGH=15,
		PERFORMANCE_PROT_POWER_VALUES_LOW=16,
		PERFORMANCE_PROT_POWER_VALUES_HIGH=16,
		PERFORMANCE_PROT_ATTR_PROT_RESERVED_LOW=17,
		PERFORMANCE_PROT_ATTR_PROT_RESERVED_HIGH=31
	}arm_scmi_performance_protocol_attributes_bits;
};
struct arm_scmi_performance_protocol_message_attributes {
	struct {
		uint32_t message_id;
	} parameters;
	struct {
		uint32_t attributes;
	} returns;
};
struct arm_scmi_performance_domain_attributes {
	struct {
		uint32_t domain_id;
	} parameters;
	struct {
		uint32_t attributes;
		uint32_t rate_limit;
		uint32_t sustained_freq;
		uint32_t sustained_perf_level;
		uint8_t name[SCMI_NAME_STR_SIZE];
	} returns;
	enum {
		PERFORMANCE_DOMAIN_ATTR_LIMIT_SET=31,
		PERFORMANCE_DOMAIN_ATTR_LEVEL_SET=30,
		PERFORMANCE_DOMAIN_ATTR_LIMIT_NOTIFY=29,
		PERFORMANCE_DOMAIN_ATTR_LEVEL_NOTIFY=28,
		PERFORMANCE_DOMAIN_ATTR_RESERVED_LOW=0,
		PERFORMANCE_DOMAIN_ATTR_RESERVED_HIGH=27,
		PERFORMANCE_DOMAIN_ATTR_RATE_LIMIT_LOW=0,
		PERFORMANCE_DOMAIN_ATTR_RATE_LIMIT_HIGH=19,
		PERFORMANCE_DOMAIN_ATTR_RATE_RESERVED_LOW=20,
		PERFORMANCE_DOMAIN_ATTR_RATE_RESERVED_HIGH=31
	}arm_scmi_performance_domain_attributes_bits;
};
struct arm_scmi_performance_describe_levels {
	struct {
		uint32_t domain_id;
		uint32_t level_index;
	} parameters;
	struct {
		uint32_t num_levels;
		struct arm_scmi_perf_levels {
			uint32_t performance_level_value;
			uint32_t power_cost;
			uint32_t attributtes;
		} perf_levels;
	} returns;
	enum {
		PERFORMANCE_DESC_LEVELS_NUM_LEVELS_REMAINING_LOW=16,
		PERFORMANCE_DESC_LEVELS_NUM_LEVELS_REMAINING_HIGH=31,
		PERFORMANCE_DESC_LEVELS_NUM_LEVELS_RESERVED_LOW=12,
		PERFORMANCE_DESC_LEVELS_NUM_LEVELS_RESERVED_HIGH=15,
		PERFORMANCE_DESC_LEVELS_NUM_LEVELS_LOW=0,
		PERFORMANCE_DESC_LEVELS_NUM_LEVELS_HIGH=11,
		PERFORMANCE_DESC_LEVELS_ATTR_TRANSITION_LOW=0,
		PERFORMANCE_DESC_LEVELS_ATTR_TRANSITION_HIGH=15,
		PERFORMANCE_DESC_LEVELS_ATTR_RESERVED_LOW=16,
		PERFORMANCE_DESC_LEVELS_ATTR_RESERVED_HIGH=31
	}arm_scmi_performance_describe_levels_bits;
};
struct arm_scmi_performance_limits_set {
	struct {
		uint32_t domain_id;
		uint32_t range_max;
		uint32_t range_min;
	} parameters;
};
struct arm_scmi_performance_limits_get {
	struct {
		uint32_t domain_id;
	} parameters;
	struct {
		uint32_t range_max;
		uint32_t range_min;
	} returns;
};
struct arm_scmi_performance_levels_set {
	struct {
		uint32_t domain_id;
		uint32_t performance_level;
	} parameters;
};
struct arm_scmi_performance_levels_get {
	struct {
		uint32_t domain_id;
	} parameters;
	struct {
		uint32_t performance_level;
	} returns;
};

extern struct arm_scmi_test_suite arm_scmi_performance_test_suites[];
extern const size_t arm_scmi_performance_test_suites_size;
extern struct arm_scmi_protocol_report arm_scmi_performance_protocol_report;
void arm_scmi_post_processing_performance_protocol_attributes(
		struct arm_scmi_protocol_execution_context *, arm_scmi_test_case_t *,
		struct arm_scmi_message *);
void arm_scmi_post_processing_performance_protocol_message_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case, struct arm_scmi_message *received);
void arm_scmi_custom_test_performance_domain_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case);
void arm_scmi_custom_test_performance_describe_levels(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case);
void arm_scmi_custom_test_performance_limits_set(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case);
void arm_scmi_custom_test_performance_limits_get(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case);
void arm_scmi_custom_test_performance_levels_set(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case);
void arm_scmi_custom_test_performance_levels_get(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case);

#endif /* PERFORMANCE_COMMON_H_ */
