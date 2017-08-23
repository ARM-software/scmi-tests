/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <sensor_common.h>
#include <sensor_exports.h>
#include <test_agent_exports.h>

/*The following data structure captures the test case representation for
 * various test cases identified for
  the sensor management protocol discovery commands*/
/*SENSOR MANAGEMENT PROTOCOL - DISCOVERY TEST LIST*/
arm_scmi_test_case_t sensor_discovery_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	{.testcase_id = "[sensor]-{PROTOCOL_VERSION}-query_protocol_version-01",
	 .protocol_id = SNSR_PROTO_ID,
	 .message_id = SNSR_PROTO_VER_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t [])
	 {
		{
		.type =  T_UINT32_HEX,
		.def = &(return_value_def_t){
			.name = "PROTOCOL VERSION",
			.offset = OFFSET_BYTES_RET(struct arm_scmi_sensor_protocol_version,
					version)
			}
		},
		T_EOT_T
	 }
	},
	/*----------Start of Test Case Description 02--------------*/
	{.testcase_id =  "[sensor]-{PROTOCOL_ATTRIBUTES}-query_protocol_attributes-01",
	 .protocol_id = SNSR_PROTO_ID,
	 .message_id = SNSR_PROTO_ATTRIB_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
	 {
		{
		.type = T_BIT_FIELD,
		.value.integer = RESERVED,
		.flags = CHECK,
		.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
						struct arm_scmi_sensor_protocol_attributes, attributes),
				.name = "SENSOR ATTRIBUTES_RESERVED",
				.msb = SNR_RESERVED_HIGH,
				.lsb = SNR_RESERVED_LOW
				}
		},
		{
		.type = T_BIT_FIELD,
		.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
					struct arm_scmi_sensor_protocol_attributes, attributes),
				.name = "SENSOR ATTRIBUTES_MAX_NUM_PENDING_ASYNC_RATE_CHGS_SUPPORTED",
				.msb = SNR_MAX_NUM_PENDING_ASYNC_CMDS_SPRT_HIGH,
				.lsb = SNR_MAX_NUM_PENDING_ASYNC_CMDS_SPRT_LOW
				}
		},
		{
		.type = T_BIT_FIELD,
		.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
					struct arm_scmi_sensor_protocol_attributes, attributes),
			.name = "SENSOR ATTRIBUTES_NUMBER_SENSORS",
			.msb = SNR_NUMBER_SENSORS_HIGH,
			.lsb = SNR_NUMBER_SENSORS_LOW
			}
		},
		T_EOT_T
	 },
	 .post_proccesing_func = arm_scmi_post_processing_sensor_protocol_attributes
	},
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are checking if the mandatory "sensor description get" command
	 * is supported by sensor mgmt protocol*/
	{.testcase_id = "[sensor]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_sensor_description_get_cmd_support-01",
	 .protocol_id = SNSR_PROTO_ID,
	 .message_id = SNSR_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { SNSR_DESC_GET_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
						struct arm_scmi_sensor_protocol_message_attributes,
						attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
		arm_scmi_post_processing_sensor_protocol_message_attributes
	},
	/*----------Start of Test Case Description 04--------------*/
	/*Here we are checking if the mandatory "sensor trip point notify" command
	 * is supported by sensor mgmt protocol*/
	{.testcase_id = "[sensor]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_sensor_trip_point_notify_cmd_support-01",
	 .protocol_id = SNSR_PROTO_ID,
	 .message_id = SNSR_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { SNSR_TRIP_POINT_NOTIFY_ID },
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
						struct arm_scmi_sensor_protocol_message_attributes,
						attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
		arm_scmi_post_processing_sensor_protocol_message_attributes
	},
	/*----------Start of Test Case Description 05--------------*/
	/*Here we are checking if the mandatory "sensor trip point config" command
	 * is supported by sensor mgmt protocol*/
	{.testcase_id = "[sensor]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_sensor_trip_point_config_cmd_support-01",
	 .protocol_id = SNSR_PROTO_ID,
	 .message_id = SNSR_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { SNSR_TRIP_POINT_CONFIG_ID },
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
						struct arm_scmi_sensor_protocol_message_attributes,
						attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
		arm_scmi_post_processing_sensor_protocol_message_attributes
	},
	/*----------Start of Test Case Description 06--------------*/
	/*Here we are checking if the mandatory "sensor trip point config" command
	 * is supported by sensor mgmt protocol*/
	{.testcase_id = "[sensor]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_sensor_reading_get_cmd_support-01",
	 .protocol_id = SNSR_PROTO_ID,
	 .message_id = SNSR_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { SNSR_READING_GET_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
						struct arm_scmi_sensor_protocol_message_attributes,
						attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
		arm_scmi_post_processing_sensor_protocol_message_attributes
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
};

/*The following data structure captures the test case representation for
 * various test cases identified for
  sensor management protocol focusing on sensor description get command*/
/*SENSOR_DESCRIPTION_GET - COMMAND TEST LIST*/
arm_scmi_test_case_t sensor_description_get_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the discovery of sensor attributes for all supported sensor ids.*/
	{.testcase_id = "[sensor]-{SENSOR_DESCRIPTION_GET}-query_sensor_description_get_allsensorid-01",
	 .protocol_id = SNSR_PROTO_ID,
	 .message_id = SNSR_DESC_GET_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .parameter_discovery_driven = true,
	 .custom_tester_func = arm_scmi_custom_tester_sensor_description_get
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the sensor description get command for a negative scenario.
	  We check if a non-existant sensor id query is handled correctly*/
	{.testcase_id = "[sensor]-{SENSOR_DESCRIPTION_GET}-query_sensor_description_get_non_existant_sensorid-01",
	 .protocol_id = SNSR_PROTO_ID,
	 .message_id = SNSR_DESC_GET_MSG_ID,
	 .status = SCMI_STATUS_INVALID_PARAMETERS,
	 .parameter_discovery_driven = true,
	 .invalid_parameter =
		OFFSET_INVALID_PARAM(struct arm_scmi_sensor_description_get, desc_index),
	 .custom_tester_func = arm_scmi_custom_tester_sensor_description_get
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
};

/*The following data structure captures the test case representation for
 * various test cases identified for
  sensor management protocol focusing on sensor config set command*/
/*SENSOR_TRIP_POIT_NOTIFY - COMMAND TEST LIST*/
arm_scmi_test_case_t sensor_trip_point_notify_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the setting of sensor config for the calling agent
	 * across all supported sensor ids.*/
	{.testcase_id = "[sensor]-{SENSOR_TRIP_POINT_NOTIFY}-set_sensor_trip_point_notification_allsensorid-01",
	 .protocol_id = SNSR_PROTO_ID,
	 .message_id = SNSR_TRIP_POINT_NOTIFY_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .parameter_discovery_driven = true,
	 .custom_tester_func = arm_scmi_custom_tester_sensor_trip_point_notify
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the sensor config set command for a negative scenario.
	  We check if a non-existant sensor id query is handled correctly*/
	{.testcase_id = "[sensor]-{SENSOR_TRIP_POINT_NOTIFY}-set_sensor_trip_point_notification_non_existant_sensorid-01",
	 .protocol_id = SNSR_PROTO_ID,
	 .message_id = SNSR_TRIP_POINT_NOTIFY_ID,
	 .status = SCMI_STATUS_NOT_FOUND,
	 .parameter_discovery_driven = true,
	 .invalid_parameter =
		OFFSET_INVALID_PARAM(struct arm_scmi_sensor_trip_point_notify,
		sensor_id),
	 .custom_tester_func = arm_scmi_custom_tester_sensor_trip_point_notify
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
};

/*The following data structure captures the test case representation for various test cases identified for
  sensor management protocol focusing on sensor trip point set command*/
/*SENSOR_TRIP_POINT_CONFIG - COMMAND TEST LIST*/
arm_scmi_test_case_t sensor_trip_point_config_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the setting of trip points for all supported sensor ids.*/
	{.testcase_id = "[sensor]-{SENSOR_TRIP_POINT_CONFIG}-set_sensor_trip_point_config_allsensorid-01",
	 .protocol_id = SNSR_PROTO_ID,
	 .message_id = SNSR_TRIP_POINT_CONFIG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .parameter_discovery_driven = true,
	 .custom_tester_func = arm_scmi_custom_tester_sensor_trip_point_config
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the sensor config set command for a negative scenario.
	  We check if a non-existant sensor id query is handled correctly*/
	{.testcase_id = "[sensor]-{SENSOR_TRIP_POINT_CONFIG}-set_sensor_trip_point_config_non_existant_sensorid-01",
	 .protocol_id = SNSR_PROTO_ID,
	 .message_id = SNSR_TRIP_POINT_CONFIG_ID,
	 .status = SCMI_STATUS_NOT_FOUND,
	 .parameter_discovery_driven = true,
	 .invalid_parameter =
		OFFSET_INVALID_PARAM(struct arm_scmi_sensor_trip_point_config,
		sensor_id),
	 .custom_tester_func = arm_scmi_custom_tester_sensor_trip_point_config
	},
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are testing the sensor config set command for a negative scenario.
	  We check if a non-existant trip point id query is handled correctly*/
	{.testcase_id = "[sensor]-{SENSOR_TRIP_POINT_CONFIG}-set_sensor_trip_point_config_non_existant_trip_pointid-01",
	 .protocol_id = SNSR_PROTO_ID,
	 .message_id = SNSR_TRIP_POINT_CONFIG_ID,
	 .status = SCMI_STATUS_NOT_FOUND,
	 .parameter_discovery_driven = true,
	 .invalid_parameter =
		OFFSET_INVALID_PARAM(struct arm_scmi_sensor_trip_point_config,
		trip_point_ev_ctrl),
	 .custom_tester_func = arm_scmi_custom_tester_sensor_trip_point_config
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
};

/*The following data structure captures the test case representation for various test cases identified for
  sensor management protocol focusing on sensor reading get command*/
/*SENSOR_READING_GET - COMMAND TEST LIST*/
arm_scmi_test_case_t sensor_reading_get_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the getting of sensor reading for all supported sensor ids.
	  This will be issued as a Sync command*/
	{.testcase_id = "[sensor]-{SENSOR_READING_GET}-sensor_reading_get_sync_allsensorid-01",
	 .protocol_id = SNSR_PROTO_ID,
	 .message_id = SNSR_READING_GET_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .parameter_discovery_driven = true,
	 .custom_tester_func = arm_scmi_custom_tester_sensor_reading_get
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the getting of sensor reading for all supported sensor ids.
	  This will be issued as a Async command
	  The agent should wait for SENSOR_READING_COMPLETE delayed response before declaring a success*/
	/*TO-DO: Test for ASYNC commands*/
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are testing the sensor reading get command for a negative scenario.
	  We check if a non-existant sensor id query is handled correctly*/
	{.testcase_id = "[sensor]-{SENSOR_READING_GET}-sensor_reading_get_non_existant_sensorid-01",
	 .protocol_id = SNSR_PROTO_ID,
	 .message_id = SNSR_READING_GET_MSG_ID,
	 .status = SCMI_STATUS_NOT_FOUND,
	 .parameter_discovery_driven = true,
	 .invalid_parameter =
			OFFSET_INVALID_PARAM(struct arm_scmi_sensor_reading_get, sensor_id),
	 .custom_tester_func = arm_scmi_custom_tester_sensor_reading_get
	},

	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
};

/*
  * This array is used to process the test suites within the sensor_exe file.
  * The test suite index within the array must correspond to the test suite id in
  * the enum scmi_test_sensor_test_suite_ids.
  */
arm_scmi_test_suite_t arm_scmi_sensor_test_suites[] = {
		{sensor_discovery_test_suite},
		{sensor_description_get_test_suite},
		{sensor_trip_point_notify_test_suite},
		{sensor_trip_point_config_test_suite},
		{sensor_reading_get_test_suite}
};

const size_t arm_scmi_sensor_test_suites_size =
NUM_ELEMS(arm_scmi_sensor_test_suites);

/*
* Protocol executor function to be called in the test_executor file.
*/
struct arm_scmi_build_protocol_executor arm_scmi_sensor_protocol_executor =
{
.protocol_id = SNSR_PROTO_ID,
.protocol_executor = arm_scmi_sensor_execute
};

/*
  * This structure is used to record the Sensor Protocol test case results.
  */
struct arm_scmi_protocol_report arm_scmi_sensor_protocol_report = {
	.protocol_id = SNSR_PROTO_ID,
	.test_suite_report_size = NUM_ELEMS(arm_scmi_sensor_test_suites),
	/* The test suite index within the array must correspond to the test
	 * suite id in the enum scmi_test_sensor_test_suite_ids.
	 */
	.test_suite_report_table = (struct arm_scmi_test_suite_report[]) {
 		{
			.test_suite_name = "SENSOR_DISCOVERY",
			.test_case_count = COUNT_TEST_CASES(sensor_discovery_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(sensor_discovery_test_suite)]) {}
 		},
		{
			.test_suite_name = "SENSOR_DESCRIPTION_GET",
			.test_case_count = COUNT_TEST_CASES(sensor_description_get_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(sensor_description_get_test_suite)]) {}
		},
		{
			.test_suite_name = "SENSOR_TRIP_POINT_NOTIFY",
			.test_case_count = COUNT_TEST_CASES(sensor_trip_point_notify_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(sensor_trip_point_notify_test_suite)]) {}
		},
		{
			.test_suite_name = "SENSOR_TRIP_POINT_CONFIG",
			.test_case_count = COUNT_TEST_CASES(sensor_trip_point_config_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(sensor_trip_point_config_test_suite)]) {}
		},
		{
			.test_suite_name = "SENSOR_READING_GET",
			.test_case_count = COUNT_TEST_CASES(sensor_reading_get_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(sensor_reading_get_test_suite)]) {}
		}
	}
};

