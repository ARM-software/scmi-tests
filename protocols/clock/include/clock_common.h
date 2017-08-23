/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef CLOCK_COMMON_H_
#define CLOCK_COMMON_H_

#include <common.h>
#include <clock_expected.h>
#include <test_agent_exports.h>

#define CLK_PROTO_ID 0x14
#define CLK_PROTO_VER_MSG_ID 0x0
#define CLK_PROTO_ATTRIB_MSG_ID 0x1
#define CLK_PROTO_MSG_ATTRIB_MSG_ID 0x2
#define CLK_ATTRIB_MSG_ID 0x3
#define CLK_DESC_RATE_MSG_ID 0x4
#define CLK_RATE_SET_MSG_ID 0x5
#define CLK_RATE_GET_MSG_ID 0x6
#define CLK_CONFIG_SET_MSG_ID 0x7
#define MAX_NUM_CLOCK 16
#define MAX_NUM_RATES_SUPPORTED 16
#define THREE_TUPLE_RATE_FORMAT 1
#define LOWEST_RATE 0
#define HIGHEST_RATE 1
#define STEP_SIZE 2
#define CLOCK_ENABLED 1
#define CLOCK_DISABLED 0

/* Enum to use constants instead of indexes for test suites */
enum clock_test_suite_index {
	TS_CLOCK_DISCOVERY,
	TS_CLOCK_ATTRIBUTE,
	TS_CLOCK_DESC_RATE,
	TS_CLOCK_RATE_SET,
	TS_CLOCK_RATE_GET,
	TS_CLOCK_CONFIG_SET
};

struct arm_scmi_clock_protocol_version {
	struct  {
		uint32_t version;
	} returns;
};

struct arm_scmi_clock_protocol_attributes {
	struct {
		uint32_t attributes;
	} returns;
	/* all the enums must be at the end */
	enum {
		CLK_NUMBER_CLOCKS_LOW=0,
		CLK_NUMBER_CLOCKS_HIGH=15,
		CLK_MAX_NUM_PENDING_ASYNC_RATE_CHGS_SPRT_LOW=16,
		CLK_MAX_NUM_PENDING_ASYNC_RATE_CHGS_SPRT_HIGH=23,
		CLK_RESERVED_LOW=24,
		CLK_RESERVED_HIGH=31
	}attributes_bits;
};

struct arm_scmi_clock_attributes {
	struct {
		uint32_t clock_id;
	} parameters;
	struct {
		uint32_t attributes;
		uint8_t clock_name[SCMI_NAME_STR_SIZE];
	} returns;
	/* enums for attributes */
	enum {
		CLK_ATTRIBUTES_ENABLED = 0,
		CLK_ATTRIBUTES_RESERVED_LOW = 1,
		CLK_ATTRIBUTES_RESERVED_HIGH = 31
	} clk_attributes_bits;
};

struct arm_scmi_clock_protocol_message_attributes {
	struct {
		uint32_t message_id;
	} parameters;
	struct {
		uint32_t attributes;
	} returns;
};

struct arm_scmi_clock_describe_rates {
	struct {
		uint32_t clock_id;
		uint32_t rate_index;
	} parameters;
	struct {
		uint32_t num_rates_flags;
		uint32_t rates[2];
	} returns;
	/* enums for num_rates_flags */
	enum {
		CLK_DESCRATE_NUM_RATES_RETURNED_LOW=0,
		CLK_DESCRATE_NUM_RATES_RETURNED_HIGH=11,
		CLK_DESCRATE_RETURN_FORMAT_LOW=12,
		CLK_DESCRATE_RETURN_FORMAT_HIGH=12,
		CLK_DESCRATE_RESERVED_LOW=13,
		CLK_DESCRATE_RESERVED_HIGH=15,
		CLK_DESCRATE_NUM_RATES_REMAINING_LOW=16,
		CLK_DESCRATE_NUM_RATES_REMAINING_HIGH=31
	} num_rates_flags_bits;
};

struct arm_scmi_clock_rate_set {
	struct {
		uint32_t flags;
		uint32_t clock_id;
		uint32_t rate;
	} parameters;
	/* enums for clock rate set flag parameter */
	enum {
		CLK_RATESET_ASYNC_FLAG_LOW=0,
		CLK_RATESET_ASYNC_FLAG_HIGH=1,
		CLK_RATESET_IGNORE_DELAY_RSP_LOW=1,
		CLK_RATESET_IGNORE_DELAY_RSP_HIGH=1,
		CLK_RATESET_IGNORE_ROUNDDOWN_LOW=2,
		CLK_RATESET_IGNORE_ROUNDDOWN_HIGH=2,
		CLK_RATESET_RESERVED_LOW=3,
		CLK_RATESET_RESERVED_HIGH=31
	} rate_set_flags;
};

struct arm_scmi_clock_rate_get {
	struct {
		uint32_t clock_id;
	} parameters;
	struct {
		uint32_t rate;
	} returns;
};

struct arm_scmi_clock_config_set {
	struct {
		uint32_t clock_id;
		uint32_t attributes;
	} parameters;
	enum {
		CLK_CONFIGSET_ATTR_ENABLED=0,
		CLK_CONFIGSET_ATTR_RESERVED_LOW=1,
		CLK_CONFIGSET_ATTR_RESERVED_HIGH=31,
	} config_set_flags;
};

extern struct arm_scmi_test_suite arm_scmi_clock_test_suites[];
extern const size_t arm_scmi_clock_test_suites_size;
extern struct arm_scmi_protocol_report arm_scmi_clock_protocol_report;
void arm_scmi_post_processing_clock_protocol_message_attributes(
		struct arm_scmi_protocol_execution_context *,
		arm_scmi_test_case_t *, struct arm_scmi_message *);
void arm_scmi_post_processing_clock_protocol_message_attributes(
		struct arm_scmi_protocol_execution_context *,
		arm_scmi_test_case_t *,
		struct arm_scmi_message *);
void arm_scmi_custom_tester_clock_describe_rates(
		struct arm_scmi_protocol_execution_context *,
		arm_scmi_test_case_t *);
void arm_scmi_custom_tester_clock_attributes(
		struct arm_scmi_protocol_execution_context *,
		arm_scmi_test_case_t *);
void arm_scmi_custom_tester_clock_rate_set(
		struct arm_scmi_protocol_execution_context *,
		arm_scmi_test_case_t *);
void arm_scmi_custom_tester_clock_rate_get(
		struct arm_scmi_protocol_execution_context *,
		arm_scmi_test_case_t *);
void arm_scmi_post_processing_clock_protocol_attributes(
		struct arm_scmi_protocol_execution_context *,
		arm_scmi_test_case_t *,
		struct arm_scmi_message *);
void arm_scmi_custom_tester_clock_config_set(
		struct arm_scmi_protocol_execution_context *,
		arm_scmi_test_case_t *);

#endif /* CLOCK_COMMON_H_ */
