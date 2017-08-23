/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <test_agent_exports.h>
#include <base_common.h>
/*The following data structure captures the test case representation for various
 * test cases identified for
  top level protocol discovery commands*/
/*BASE PROTOCOL - DISCOVERY TEST LIST*/
arm_scmi_test_case_t base_discovery_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	{.testcase_id = "[base]-{PROTOCOL_VERSION}-query_protocol_version-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_PROTO_VERSION_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t [])
	 {
		{
		.type =  T_UINT32_HEX,
		.def = &(return_value_def_t){
			.name = "PROTOCOL VERSION",
			.offset = OFFSET_BYTES_RET(struct arm_scmi_base_protocol_version,
			version)
			}
		},
		T_EOT_T
	 }
	},
	/*----------Start of Test Case Description 02--------------*/
	{.testcase_id = "[base]-{PROTOCOL_ATTRIBUTES}-query_protocol_attributes-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_PROTO_ATTR_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
	 {
		{
		.type = T_BIT_FIELD,
		.value.integer = RESERVED,
		.flags = CHECK,
		.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_base_protocol_attributes, attributes),
				.name = "ATTRIBUTES_RESERVED",
				.msb = BASE_RESERVED_HIGH,
				.lsb = BASE_RESERVED_LOW
				}
		},
		{
		.type = T_BIT_FIELD,
		.def = &(return_value_def_t){
			.offset = OFFSET_BYTES_RET(struct arm_scmi_base_protocol_attributes,
			attributes),
			.name = "ATTRIBUTES_NUMBER_AGENTS",
			.msb = BASE_NUMBER_AGENTS_HIGH,
			.lsb = BASE_NUMBER_AGENTS_LOW
			}
		},
		{
		.type = T_BIT_FIELD,
		.def =&(return_value_def_t){
			.offset = OFFSET_BYTES_RET(struct arm_scmi_base_protocol_attributes,
			attributes),
			.name = "ATTRIBUTES_NUMBER_PROTOCOLS",
			.msb = BASE_NUMBER_PROTOCOLS_HIGH,
			.lsb = BASE_NUMBER_PROTOCOLS_LOW
			}
		},
		T_EOT_T
	 },
	 .post_proccesing_func = arm_scmi_post_processing_base_protocol_attributes
	},
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are checking if the mandatory base discover vendor command is
	 * supported by base protocol*/
	{.testcase_id =
	"[base]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_vendor_disc_cmd_support-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_PROTO_MSG_ATTR_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { BASE_DISC_VENDOR_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
	 {
		{
		.type =  T_UINT32_HEX,
		.value.integer = 0x0,
		.def = &(return_value_def_t){
			.offset = OFFSET_BYTES_RET(
			struct arm_scmi_base_protocol_message_attributes, attributes),
				.name = "MESSAGE ATTRIBUTES"
			}
		},
		T_EOT_T
	 },
	.post_proccesing_func =
			arm_scmi_post_processing_base_protocol_message_attributes
	},
	/*----------Start of Test Case Description 04--------------*/
	/*Here we are checking if the *optional* base discover sub-vendor command
	 * is supported by base protocol*/
	{.testcase_id =
	"[base]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_subvendor_disc_cmd_support-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_PROTO_MSG_ATTR_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) {BASE_DISC_SUBVENDOR_MSG_ID},
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	 .expected_returns = (return_value_desc_t[])
	 {
		{
			.type = T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_base_protocol_message_attributes, attributes),
				.name = "MESSAGE ATTRIBUTES"
			}
		},
		T_EOT_T
	},
	.post_proccesing_func =
			arm_scmi_post_processing_base_protocol_message_attributes
	},
	/*----------Start of Test Case Description 05--------------*/
	/*Here we are checking if the mandatory base discover implementation version
	 *  command is supported by base protocol*/
	{.testcase_id =
	"[base]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_impl_version_cmd_support-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_PROTO_MSG_ATTR_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) {BASE_DISC_IMPLEMENTATION_VERSION_MSG_ID},
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
	 {
		{
			.type = T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_base_protocol_message_attributes, attributes),
				.name = "MESSAGE ATTRIBUTES"
			}
		},
		T_EOT_T
	},
	.post_proccesing_func =
			arm_scmi_post_processing_base_protocol_message_attributes
	},
	/*----------Start of Test Case Description 06--------------*/
	/*Here we are checking if the mandatory base discovery protocol list command
	 * is supported by base protocol.
	 * */
	{.testcase_id =
	"[base]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_proto_list_cmd_support-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_PROTO_MSG_ATTR_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) {BASE_DISC_PROTO_LIST_MSG_ID},
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
	 {
		{
			.type = T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_base_protocol_message_attributes, attributes),
			.name = "MESSAGE ATTRIBUTES"
			}
		},
		T_EOT_T
	},
	.post_proccesing_func =
			arm_scmi_post_processing_base_protocol_message_attributes
	},
	/*----------Start of Test Case Description 07--------------*/
	/*Here we are checking if the *optional* base discover agent command is
	 * supported by base protocol*/
	{.testcase_id =
	"[base]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_agent_disc_cmd_support-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_PROTO_MSG_ATTR_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) {BASE_DISC_AGENT_MSG_ID},
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	 .expected_returns = (return_value_desc_t[])
	 {
		{
			.type = T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_base_protocol_message_attributes, attributes),
			.name = "MESSAGE ATTRIBUTES"
			}
		},
		T_EOT_T
	},
	.post_proccesing_func =
			arm_scmi_post_processing_base_protocol_message_attributes
	},
	/*----------Start of Test Case Description 08--------------*/
	/*Here we are checking if the *optional* base discover notify error command
	 * is supported by base protocol*/
	{.testcase_id =
	"[base]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_notify_error_cmd_support-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_PROTO_MSG_ATTR_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) {BASE_NOTIFY_ERRORS_MSG_ID},
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	 .expected_returns = (return_value_desc_t[])
	 {
		{
			.type = T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_base_protocol_message_attributes, attributes),
			.name = "MESSAGE ATTRIBUTES"
			}
		},
		T_EOT_T
	},
	.post_proccesing_func =
			arm_scmi_post_processing_base_protocol_message_attributes
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
};
/*BASE_DISCOVER_VENDOR - COMMAND TEST LIST*/
arm_scmi_test_case_t base_discover_vendor_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the discovery vendor command*/
	{.testcase_id = "[base]-{BASE_DISCOVER_VENDOR}-query_vendor_info-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_DISC_VENDOR_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
	 {
		{
			.type = T_STRING,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(struct arm_scmi_base_discover_vendor,
				vendor_identifier),
				.len = FIELD_SIZE_BYTES(struct arm_scmi_base_discover_vendor,
				returns.vendor_identifier),
				.name = "VENDOR IDENTIFIER"
			}
		},
		T_EOT_T
	}
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	},
};
arm_scmi_test_case_t base_discover_subvendor_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the discovery subvendor command*/
	{.testcase_id = "[base]-{BASE_DISCOVER_SUB_VENDOR}-query_subvendor_info-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_DISC_SUBVENDOR_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
	{
		{
			.type = T_STRING,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_base_discover_subvendor, vendor_identifier),
				.len = FIELD_SIZE_BYTES(
				struct arm_scmi_base_discover_subvendor,
				returns.vendor_identifier),
				.name = "SUBVENDOR IDENTIFIER"
			}
		},
		T_EOT_T
	}
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	},
};
/*BASE_DISCOVER_IMPLEMENTATION_VERSION - COMMAND TEST LIST*/
arm_scmi_test_case_t base_discover_impl_ver_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the discovery implementation version command*/
	{.testcase_id = "[base]-{BASE_DISCOVER_IMPLEMENTATION_VERSION}-query_implementation_ver-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_DISC_IMPLEMENTATION_VERSION_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
	 {
		{
		.type =T_UINT32_HEX,
		.def = &(return_value_def_t){
			.offset = OFFSET_BYTES_RET(
			struct arm_scmi_base_discover_implementation_version,
			implementation_version),
			.name = "IMPLEMENTATION VERSION"
			}
		},
		T_EOT_T
	}
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	},
};
/*The following data structure captures the test case representation for various
 *test cases identified for
  base protocol focusing on discover protocol list command*/
/*BASE_DISCOVER_LIST_PROTOCOLS - COMMAND TEST LIST*/
arm_scmi_test_case_t base_disc_proto_list_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the base protocol list command*/
	{.testcase_id = "[base]-{BASE_DISCOVER_LIST_PROTOCOLS}-query_protocol_list-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_DISC_PROTO_LIST_MSG_ID,
	 .parameter_discovery_driven = true,
	 .status = SCMI_STATUS_SUCCESS,
	.custom_tester_func = arm_scmi_custom_tester_base_disco_list_protocols
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the discover protocol list command for a negative
	 * scenario. We check if invalid skip count is handled correctly*/
	{.testcase_id = "[base]-{BASE_DISCOVER_LIST_PROTOCOLS}-query_invalid_skipindex-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_DISC_PROTO_LIST_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter =
		OFFSET_INVALID_PARAM(struct arm_scmi_base_discover_list_protocols, skip),
	 .status = SCMI_STATUS_INVALID_PARAMETERS,
	 .custom_tester_func = arm_scmi_custom_tester_base_disco_list_protocols
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
};
/*The following data structure captures the test case representation for various
 *test cases identified for base protocol focusing on discover agent command*/
/*BASE_DISCOVER_AGENT - COMMAND TEST LIST*/
arm_scmi_test_case_t base_disc_agent_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the base protocol discover agent command with
	 * agentid = 0 which should return 'platform<blah>' as response*/
	{.testcase_id = "[base]-{BASE_DISCOVER_AGENT}-query_agent_id0-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_DISC_AGENT_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) {0},
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	.custom_tester_func = arm_scmi_custom_tester_base_disco_agent_msg
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the base protocol discover agent command for all
	 * non-zero agent ids*/
	{.testcase_id = "[base]-{BASE_DISCOVER_AGENT}-query_non_zero_agentid-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_DISC_AGENT_MSG_ID,
	 .parameter_discovery_driven = true,
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	.custom_tester_func = arm_scmi_custom_tester_base_disco_agent_msg
	},
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are testing the base protocol discover agent command for a
	 * negative scenario where we specify an invalid agent id*/
	{.testcase_id = "[base]-{BASE_DISCOVER_AGENT}-query_invalid_agentid-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_DISC_AGENT_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter = OFFSET_INVALID_PARAM(
		struct arm_scmi_base_discover_agent, agent_id),
	 .status = SCMI_STATUS_NOT_FOUND,
	 .custom_tester_func = arm_scmi_custom_tester_base_disco_agent_msg
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
};
/*The following data structure captures the test case representation for various
 * test cases identified for base protocol focusing on notify errors command*/
/*BASE_NOTIFY_ERRORS - COMMAND TEST LIST*/
arm_scmi_test_case_t base_disc_notify_errors_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the base protocol notify error*/
	{.testcase_id = "[base]-{BASE_NOTIFY_ERRORS}-reg_err_notification-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_NOTIFY_ERRORS_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) {((RESERVED << NOTIFY_ENABLE_RESERVED_LOW) |
		(BASE_NOTIFY_ENABLE_SET << NOTIFY_ENABLE))},
	 .status = SCMI_STATUS_SUCCESS
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the post-condition for unsubscribing for notifications*/
	{.testcase_id = "[base]-{BASE_NOTIFY_ERRORS}-postcondition_reg_err_notification-01",
	 .protocol_id = BASE_PROTO_ID,
	 .message_id = BASE_NOTIFY_ERRORS_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) {((RESERVED << NOTIFY_ENABLE_RESERVED_LOW) |
		(BASE_NOTIFY_ENABLE_UNSET << NOTIFY_ENABLE))},
	 .status = SCMI_STATUS_SUCCESS
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
};

/*
 * This array is used to process the test suites within the base_exe file.
 * The test suite index within the array must correspond to the test suite id in
 * the enum scmi_test_base_test_suite_ids.
 */
arm_scmi_test_suite_t arm_scmi_test_base_test_suites[] = {
	{base_discovery_test_suite},
	{base_discover_vendor_test_suite},
	{base_discover_subvendor_test_suite},
	{base_discover_impl_ver_test_suite},
	{base_disc_proto_list_test_suite},
	{base_disc_agent_test_suite},
	{base_disc_notify_errors_test_suite}
	};
const size_t arm_scmi_test_base_test_suites_size =
		NUM_ELEMS(arm_scmi_test_base_test_suites);
/*
 * This structure is used to record the Base Protocol test case results.
 */
struct arm_scmi_protocol_report arm_scmi_base_protocol_report = {
		.protocol_id = BASE_PROTO_ID,
		.test_suite_report_size = NUM_ELEMS(arm_scmi_test_base_test_suites),
		/* The test suite index within the array must correspond to the test
		 * suite id in the enum scmi_test_base_test_suite_ids.
		 */
		.test_suite_report_table = (struct arm_scmi_test_suite_report[]) {
			{
				.test_suite_name = "BASE_DISCOVERY",
				.test_case_count = COUNT_TEST_CASES(base_discovery_test_suite),
				.test_case_report_table = (struct arm_scmi_test_case_report[
					COUNT_TEST_CASES(base_discovery_test_suite)]) {}
			},
			{
				.test_suite_name = "BASE_DISCOVER_VENDOR",
				.test_case_count =
					COUNT_TEST_CASES(base_discover_vendor_test_suite),
				.test_case_report_table = (struct arm_scmi_test_case_report[
					COUNT_TEST_CASES(base_discover_vendor_test_suite)]) {}
			},
			{
				.test_suite_name = "BASE_DISCOVER_SUB_VENDOR",
				.test_case_count =
					COUNT_TEST_CASES(base_discover_subvendor_test_suite),
				.test_case_report_table = (struct arm_scmi_test_case_report[
					COUNT_TEST_CASES(base_discover_subvendor_test_suite)]) {}
			},
			{
				.test_suite_name = "BASE_DISCOVER_IMPLEMENTATION_VERSION",
				.test_case_count =
					COUNT_TEST_CASES(base_discover_impl_ver_test_suite),
				.test_case_report_table = (struct arm_scmi_test_case_report[
					COUNT_TEST_CASES(base_discover_impl_ver_test_suite)]) {}
			},
			{
				.test_suite_name = "BASE_DISCOVER_LIST_PROTOCOLS",
				.test_case_count =
					COUNT_TEST_CASES(base_disc_proto_list_test_suite),
				.test_case_report_table = (struct arm_scmi_test_case_report[
					COUNT_TEST_CASES(base_disc_proto_list_test_suite)]) {}
			},
			{
				.test_suite_name = "BASE_DISCOVER_AGENT",
				.test_case_count = COUNT_TEST_CASES(base_disc_agent_test_suite),
				.test_case_report_table = (struct arm_scmi_test_case_report[
					COUNT_TEST_CASES(base_disc_agent_test_suite)]) {}
			},
			{
				.test_suite_name = "BASE_NOTIFY_ERRORS",
				.test_case_count =
					COUNT_TEST_CASES(base_disc_notify_errors_test_suite),
				.test_case_report_table = (struct arm_scmi_test_case_report[
					COUNT_TEST_CASES(base_disc_notify_errors_test_suite)]) {}
			}
		}
};
