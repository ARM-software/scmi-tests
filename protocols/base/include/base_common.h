/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef BASE_COMMON_H_
#define BASE_COMMON_H_

#include <common.h>
#include <base_expected.h>
#include <test_agent_exports.h>

#define BASE_PROTO_VERSION_MSG_ID 0x0
#define BASE_PROTO_ATTR_MSG_ID 0x1
#define BASE_PROTO_MSG_ATTR_MSG_ID 0x2
#define BASE_DISC_VENDOR_MSG_ID 0x3
#define BASE_DISC_SUBVENDOR_MSG_ID 0x4
#define BASE_DISC_IMPLEMENTATION_VERSION_MSG_ID 0x5
#define BASE_DISC_PROTO_LIST_MSG_ID 0x6
#define BASE_DISC_AGENT_MSG_ID 0x7
#define BASE_NOTIFY_ERRORS_MSG_ID 0x8

#define BASE_NOTIFY_ENABLE_SET 1
#define BASE_NOTIFY_ENABLE_UNSET 0

/* Base Test Suites index names */
enum arm_scmi_base_test_suite_ids{
	TS_BASE_DISCOVERY,
	TS_BASE_VENDOR,
	TS_BASE_SUBVENDOR,
	TS_BASE_IMPLEMENTATION,
	TS_BASE_PROTOCOLS,
	TS_BASE_AGENT,
	TS_BASE_NOTIFY_ERRORS
};

struct arm_scmi_base_protocol_version {
	struct  {
		uint32_t version;
	} returns;
};
struct arm_scmi_base_protocol_attributes {
	struct {
		uint32_t attributes;
	} returns;
	/* all the enums must be at the end */
	enum {
		BASE_NUMBER_PROTOCOLS_LOW=0,
		BASE_NUMBER_PROTOCOLS_HIGH=7,
		BASE_NUMBER_AGENTS_LOW=8,
		BASE_NUMBER_AGENTS_HIGH=15,
		BASE_RESERVED_LOW=16,
		BASE_RESERVED_HIGH=31
	}attributes_bits;
};
struct arm_scmi_base_protocol_message_attributes {
	struct {
		uint32_t message_id;
	} parameters;
	struct {
		uint32_t attributes;
	} returns;
};
struct arm_scmi_base_discover_vendor {
	struct {
		uint8_t vendor_identifier[SCMI_NAME_STR_SIZE];
	} returns;
};
struct arm_scmi_base_discover_subvendor {
	struct {
		uint8_t vendor_identifier[SCMI_NAME_STR_SIZE];
	} returns;
};
struct arm_scmi_base_discover_implementation_version {
	struct {
		uint32_t implementation_version;
	} returns;
};
struct arm_scmi_base_discover_list_protocols {
	struct {
		uint32_t skip;
	} parameters;
	struct {
		uint32_t num_protocols;
		/* 'protocols' is a dynamic array is fine for offset as long there is
		 * no more return value after it. If there are more return values then
		 * the size must be determined run-time.
		 */
		uint32_t protocols;
	} returns;
};
struct arm_scmi_base_discover_agent {
	struct {
		uint32_t agent_id;
	} parameters;
	struct {
		uint8_t name[SCMI_NAME_STR_SIZE];
	} returns;
};
struct arm_scmi_base_notify_error {
	struct {
		uint32_t notify_enable;
	} parameters;
	enum {
		NOTIFY_ENABLE=0,
		NOTIFY_ENABLE_RESERVED_LOW=1,
		NOTIFY_ENABLE_RESERVED_HIGH=31
	} notify_enable_bits;
};

/* Global variables filled in the implementation */
extern struct arm_scmi_test_suite arm_scmi_test_base_test_suites[];
extern struct arm_scmi_protocol_report arm_scmi_base_protocol_report;
extern const size_t arm_scmi_test_base_test_suites_size;
void arm_scmi_post_processing_base_protocol_attributes(
		struct arm_scmi_protocol_execution_context *, arm_scmi_test_case_t *,
		struct arm_scmi_message *);
void arm_scmi_post_processing_base_protocol_message_attributes(
		struct arm_scmi_protocol_execution_context *, arm_scmi_test_case_t *,
		struct arm_scmi_message *);
void arm_scmi_custom_tester_base_disco_agent_msg(
		struct arm_scmi_protocol_execution_context *, arm_scmi_test_case_t *);
void arm_scmi_custom_tester_base_disco_list_protocols(
		struct arm_scmi_protocol_execution_context *, arm_scmi_test_case_t *);

#endif /* BASE_COMMON_H_ */
