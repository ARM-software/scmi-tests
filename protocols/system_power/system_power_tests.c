/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <system_power_common.h>
#include <test_agent_exports.h>
#include <system_power_exports.h>

/*
 * The following data structure captures the test case representation for
 * various test cases identified for the system power management protocol
 * discovery commands.
 */

/*SYSTEM POWER MANAGEMENT PROTOCOL - DISCOVERY TEST LIST*/
arm_scmi_test_case_t system_power_discovery_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	{.testcase_id = "[system_power]_{PROTOCOL_VERSION}-query_protocol_version-01",
	 .protocol_id = SYSTEM_POWER_PROTO_ID,
	 .message_id = SYSTEM_POWER_PROTO_VER_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t [])
		 {
			{
			.type =  T_UINT32_HEX,
			.def = &(return_value_def_t){
				.name = "PROTOCOL VERSION",
				.offset = OFFSET_BYTES_RET(
					struct arm_scmi_system_power_protocol_version, version)
				}
			},
			T_EOT_T
		 }
	},
	/*----------Start of Test Case Description 02--------------*/
	{.testcase_id = "[system_power]-{PROTOCOL_ATTRIBUTES}-query_protocol_attributes-01",
	 .protocol_id = SYSTEM_POWER_PROTO_ID,
	 .message_id = SYSTEM_POWER_PROTO_ATTRIB_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
	 {
		{
		.type = T_BIT_FIELD,
		.value.integer = RESERVED,
		.flags = CHECK,
		.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_system_power_protocol_attributes, attributes),
				.name = "ATTRIBUTES_RESERVED",
				.msb = SYSTEM_POWER_PROTOCOLS_ATTR_RESERVED_HIGH,
				.lsb = SYSTEM_POWER_PROTOCOLS_ATTR_RESERVED_LOW
				}
		},
		T_EOT_T
	 },
	},
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are checking if the mandatory system power state set command is
	 * supported by system power mgmt protocol*/
	{.testcase_id = "[system_power]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_system_power_state_set_cmd_support-01",
	 .protocol_id = SYSTEM_POWER_PROTO_ID,
	 .message_id = SYSTEM_POWER_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { SYSTEM_POWER_STATE_SET_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .post_proccesing_func =
			 arm_scmi_post_processing_system_power_protocol_message_attributes
	},
	/*----------Start of Test Case Description 04--------------*/
	/*Here we are testing the setting of system power state for power up*/
	{.testcase_id = "[system_power]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_system_power_state_get_cmd_support-01",
	 .protocol_id = SYSTEM_POWER_PROTO_ID,
	 .message_id = SYSTEM_POWER_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { SYSTEM_POWER_STATE_GET_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .post_proccesing_func =
			 arm_scmi_post_processing_system_power_protocol_message_attributes
	},
	/*----------Start of Test Case Description 04--------------*/
	/*Here we are testing the setting of system power state for power up*/
	{.testcase_id = "[system_power]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_system_power_state_notify_cmd_support-01",
	 .protocol_id = SYSTEM_POWER_PROTO_ID,
	 .message_id = SYSTEM_POWER_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { SYSTEM_POWER_STATE_NOTIFY_MSG_ID },
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	 .post_proccesing_func =
			 arm_scmi_post_processing_system_power_protocol_message_attributes
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
};

/*The following data structure captures the test case representation for various
 * test cases identified for system power management protocol focusing on
 * system power state get command*/
/*SYSTEM POWER_STATE_GET - COMMAND TEST LIST*/
arm_scmi_test_case_t system_power_state_get_test_suite[] = {
	/*NOTE: Expecting the executor to check the agent 'capabilities/properties'
	 * to determine the expected value!*/
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the getting of system power state*/
	{.testcase_id = "[system_power]-{SYSTEM_POWER_STATE_GET}-query_system_power_get_power_state-01",
	 .protocol_id = SYSTEM_POWER_PROTO_ID,
	 .message_id = SYSTEM_POWER_STATE_GET_MSG_ID,
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
};

/*
 * This array is used to process the test suites within the system_exe file.
 * The test suite index within the array must correspond to the test suite id in
 * the enum scmi_test_power_test_suite_ids.
 */
arm_scmi_test_suite_t arm_scmi_system_power_test_suites[] = {
	{system_power_discovery_test_suite},
	{system_power_state_get_test_suite}
	};

const size_t arm_scmi_system_power_test_suites_size =
		NUM_ELEMS(arm_scmi_system_power_test_suites);

/*
 * Protocol executor function to be called in the test_executor file.
 */
struct arm_scmi_build_protocol_executor
	arm_scmi_system_power_protocol_executor = {
	.protocol_id = SYSTEM_POWER_PROTO_ID,
	.protocol_executor = arm_scmi_system_power_execute
};

/*
 * This structure is used to record the System Power Management Protocol test case results.
 */
struct arm_scmi_protocol_report arm_scmi_system_power_protocol_report = {
	.protocol_id = SYSTEM_POWER_PROTO_ID,
	.test_suite_report_size = NUM_ELEMS(arm_scmi_system_power_test_suites),
	/* The test suite index within the array must correspond to the test
	 * suite id in the enum scmi_test_system_test_suite_ids.
	 */
	.test_suite_report_table = (struct arm_scmi_test_suite_report[]) {
		{
			.test_suite_name = "SYSTEM_POWER_DISCOVERY",
			.test_case_count = COUNT_TEST_CASES(
				system_power_discovery_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(system_power_discovery_test_suite)]) {}
		},
		{
			.test_suite_name = "SYSTEM_POWER_STATE_GET",
			.test_case_count = COUNT_TEST_CASES(
				system_power_state_get_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(system_power_state_get_test_suite)]) {}
		}
	}
};

