/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef SENSOR_COMMON_H_
#define SENSOR_COMMON_H_

#include <common.h>
#include <test_agent_exports.h>
#include <sensor_expected.h>

#define SNSR_PROTO_ID 0x15
#define SNSR_PROTO_VER_MSG_ID 0x0
#define SNSR_PROTO_ATTRIB_MSG_ID 0x1
#define SNSR_PROTO_MSG_ATTRIB_MSG_ID 0x2
#define SNSR_DESC_GET_MSG_ID 0x3
#define SNSR_TRIP_POINT_NOTIFY_ID 0x4
#define SNSR_TRIP_POINT_CONFIG_ID 0x5
#define SNSR_READING_GET_MSG_ID 0x6
#define MAX_NUMBER_SENSOR 32
#define SENSOR_ASYNC 1
#define SENSOR_SYNC 0

/* Enum to use constants instead of indexes for test suites */
enum sensor_test_suite_index {
	TS_SENSOR_DISCOVERY,
	TS_SENSOR_DESC_GET,
	TS_SENSOR_TRIP_POINT_NOTIFY,
	TS_SENSOR_TRIP_POINT_CONFIG,
	TS_SENSOR_READING_GET
};

struct arm_scmi_sensor_protocol_version {
	struct  {
		uint32_t version;
	} returns;
};

struct arm_scmi_sensor_protocol_attributes {
	struct {
		uint32_t attributes;
		uint32_t sensor_reg_address_low;
		uint32_t sensor_reg_address_high;
		uint32_t sensor_reg_len;
	} returns;
	/* all the enums must be at the end */
	enum {
		SNR_NUMBER_SENSORS_LOW=0,
		SNR_NUMBER_SENSORS_HIGH=15,
		SNR_MAX_NUM_PENDING_ASYNC_CMDS_SPRT_LOW=16,
		SNR_MAX_NUM_PENDING_ASYNC_CMDS_SPRT_HIGH=23,
		SNR_RESERVED_LOW=24,
		SNR_RESERVED_HIGH=31
	}attributes_bits;
};

struct arm_scmi_sensor_protocol_message_attributes {
	struct {
		uint32_t message_id;
	} parameters;
	struct {
		uint32_t attributes;
	} returns;
};

struct arm_scmi_sensor_description_get {
	struct {
		uint32_t desc_index;
	} parameters;
	struct {
		uint32_t num_sensor_flags;
		uint32_t sensor_descriptors;
	} returns;
	/*enums for num_sensor_flags*/
	enum {
		SNR_NUMBER_SNR_DESC_LOW=0,
		SNR_NUMBER_SNR_DESC_HIGH=11,
		SNR_NUM_SNR_FLAG_RESV_LOW=12,
		SNR_NUM_SNR_FLAG_RESV_HIGH=15,
		SNR_NUMBER_REMAIN_SNR_DESC_LOW=16,
		SNR_NUMBER_REMAIN_SNR_DESC_HIGH=31
	}num_sensor_flags_bits;
};
/*sensor descriptor struct and enums*/
struct arm_scmi_sensor_descriptor {
	uint32_t sensor_id;
	uint32_t sensor_attributes_low;
	uint32_t sensor_attributes_high;
	uint8_t sensor_name[SCMI_NAME_STR_SIZE];

	enum {
		SNR_DESC_ATTRLOW_NUM_TRIPPOINT_SPRT_LOW=0,
		SNR_DESC_ATTRLOW_NUM_TRIPPOINT_SPRT_HIGH=7,
		SNR_DESC_ATTRLOW_RESV_LOW=8,
		SNR_DESC_ATTRLOW_RESV_HIGH=30,
		SNR_DESC_ATTRLOW_ASYNC_READ_SUPPORT=31
	} sensor_attributes_low_bits;

	enum {
		SNR_DESC_ATTRHIGH_SNR_TYPE_LOW=0,
		SNR_DESC_ATTRHIGH_SNR_TYPE_HIGH=7,
		SNR_DESC_ATTRHIGH_RESV_LOWER_LOW=8,
		SNR_DESC_ATTRHIGH_RESV_LOWER_HIGH=10,
		SNR_DESC_ATTRHIGH_MULTIPLIER_LOW=11,
		SNR_DESC_ATTRHIGH_MULTIPLIER_HIGH=15,
		SNR_DESC_ATTRHIGH_RESV_UPPER_LOW=16,
		SNR_DESC_ATTRHIGH_RESV_UPPER_HIGH=21,
		SNR_DESC_ATTRHIGH_UPDATE_INTERVAL_MULT_LOW=22,
		SNR_DESC_ATTRHIGH_UPDATE_INTERVAL_MULT_HIGH=26,
		SNR_DESC_ATTRHIGH_UPDATE_INTERVAL_SEC_LOW=27,
		SNR_DESC_ATTRHIGH_UPDATE_INTERVAL_SEC_HIGH=31
	} sensor_attributes_high_bits;
};

struct arm_scmi_sensor_trip_point_notify {
	struct {
		uint32_t sensor_id;
		uint32_t sensor_event_control;
	} parameters;
	enum {
		SNR_EVENT_CTRL_NOTIFUCATION_ENABLE_LOW=0,
		SNR_EVENT_CTRL_NOTIFUCATION_ENABLE_HIGH=0,
		SNR_EVENT_CTRL_RESERVED_LOW=1,
		SNR_EVENT_CTRL_RESERVED_ENABLE_HIGH=31
	}sensor_event_control_bits;
};

struct arm_scmi_sensor_trip_point_config {
	struct {
		uint32_t sensor_id;
		uint32_t trip_point_ev_ctrl;
		uint32_t trip_point_val_low;
		uint32_t trip_point_val_high;
	} parameters;
	enum {
		SNR_TRIP_POINT_EV_CTRL_BITS_LOW=0,
		SNR_TRIP_POINT_EV_CTRL_BITS_HIGH=1,
		SNR_TRIP_POINT_EV_CTRL_LOWER_RESVD_LOW=2,
		SNR_TRIP_POINT_EV_CTRL_LOWER_RESVD_HIGH=3,
		SNR_TRIP_POINT_ID_LOW=4,
		SNR_TRIP_POINT_ID_HIGH=11,
		SNR_TRIP_POINT_EV_CTRL_UPPER_RESVD_LOW=12,
		SNR_TRIP_POINT_EV_CTRL_UPPER_RESVD_HIGH=31
	}trip_point_ev_ctrl_bits;
};

struct arm_scmi_sensor_reading_get {
	struct {
		uint32_t sensor_id;
		uint32_t flags;
	} parameters;
	struct {
		uint32_t sensor_value_low;
		uint32_t sensor_value_high;
	} returns;
	enum {
		SNR_READING_FLAG_SYNC_ASYNC=0,
		SNR_READING_FLAG_RESERVED_LOW=1,
		SNR_READING_FLAG_RESERVED_HIGH=31
	}sensor_reading_flags_bits;
};

extern struct arm_scmi_test_suite arm_scmi_sensor_test_suites[];
extern const size_t arm_scmi_sensor_test_suites_size;
extern struct arm_scmi_protocol_report arm_scmi_sensor_protocol_report;

void arm_scmi_post_processing_sensor_protocol_attributes(
		struct arm_scmi_protocol_execution_context *,
		arm_scmi_test_case_t *,
		struct arm_scmi_message *);
void arm_scmi_post_processing_sensor_protocol_message_attributes(
		struct arm_scmi_protocol_execution_context *,
		arm_scmi_test_case_t *, struct arm_scmi_message *);
void arm_scmi_custom_tester_sensor_description_get(
		struct arm_scmi_protocol_execution_context *,
		arm_scmi_test_case_t *);
void arm_scmi_custom_tester_sensor_trip_point_notify(
		struct arm_scmi_protocol_execution_context *,
		arm_scmi_test_case_t *);
void arm_scmi_custom_tester_sensor_trip_point_config(
		struct arm_scmi_protocol_execution_context *,
		arm_scmi_test_case_t *);
void arm_scmi_custom_tester_sensor_reading_get(
		struct arm_scmi_protocol_execution_context *,
		arm_scmi_test_case_t *);

#endif /* SENSOR_COMMON_H_ */
