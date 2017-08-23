/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <test_agent_exports.h>
#include <power_common.h>
#include <power_exports.h>

/*The following data structure captures the test case representation for various
 * test cases identified for the power domain management protocol discovery
 * commands
 * */

/*POWER DOMAIN MANAGEMENT PROTOCOL - DISCOVERY TEST LIST*/
arm_scmi_test_case_t power_discovery_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	{.testcase_id = "[power]-{PROTOCOL_VERSION}-query_protocol_version-01",
	 .protocol_id = PWR_PROTO_ID,
	 .message_id = PWR_PROTO_VER_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t [])
		 {
			{
			.type =  T_UINT32_HEX,
			.def = &(return_value_def_t){
				.name = "PROTOCOL VERSION",
				.offset = OFFSET_BYTES_RET(
						struct arm_scmi_power_protocol_version, version)
				}
			},
			T_EOT_T
		 }
	},
	/*----------Start of Test Case Description 02--------------*/
	{.testcase_id = "[power]-{PROTOCOL_ATTRIBUTES}-query_protocol_attributes-01",
	 .protocol_id = PWR_PROTO_ID,
	 .message_id = PWR_PROTO_ATTR_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
	 {
		{
		.type = T_BIT_FIELD,
		.value.integer = RESERVED,
		.flags = CHECK,
		.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
					struct arm_scmi_power_protocol_attributes, attributes),
				.name = "POWER ATTRIBUTES_RESERVED",
				.msb = POWER_PROT_ATTR_PROT_RESERVED_HIGH,
				.lsb = POWER_PROT_ATTR_PROT_RESERVED_LOW
				}
		},
		{
		.type = T_BIT_FIELD,
		.def = &(return_value_def_t){
			.offset = OFFSET_BYTES_RET(
				struct arm_scmi_power_protocol_attributes, attributes),
			.name = "POWER ATTRIBUTES NUMBER POWER DOMAINS",
			.msb = POWER_PROT_ATTR_NUMBER_DOMAINS_HIGH,
			.lsb = POWER_PROT_ATTR_NUMBER_DOMAINS_LOW
			}
		},
		T_EOT_T
	},
	 .post_proccesing_func = arm_scmi_post_processing_power_protocol_attributes
	},
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are checking if the mandatory power domain attribute command is
	 * supported by power domain mgmt protocol
	 * */
	{.testcase_id = "[power]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_power_domain_attrib_cmd_support-01",
	 .protocol_id = PWR_PROTO_ID,
	 .message_id = PWR_PROTO_MSG_ATTR_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { PWR_DOMAIN_ATTRIB_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
	 {
		{
		.type =  T_UINT32_HEX,
		.value.integer = 0x0,
		.def = &(return_value_def_t){
			.offset = OFFSET_BYTES_RET(
			struct arm_scmi_power_protocol_message_attributes,
						attributes),
				.name = "MESSAGE ATTRIBUTES"
			}
		},
		T_EOT_T
	 },
	.post_proccesing_func =
			arm_scmi_post_processing_power_protocol_message_attributes
	},
	/*----------Start of Test Case Description 04--------------*/
	/*Here we are checking if the mandatory power state set command is supported
	 * by power domain mgmt protocol
	 * */
	{.testcase_id = "[power]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_power_state_set_cmd_support-01",
	 .protocol_id = PWR_PROTO_ID,
	 .message_id = PWR_PROTO_MSG_ATTR_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { PWR_STATE_SET_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_power_protocol_message_attributes,
							attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
			arm_scmi_post_processing_power_protocol_message_attributes
	},
	/*----------Start of Test Case Description 05--------------*/
	/*Here we are checking if the mandatory power state get command is supported
	 * by power domain mgmt protocol
	 * */
	{.testcase_id = "[power]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_power_state_get_cmd_support-01",
	 .protocol_id = PWR_PROTO_ID,
	 .message_id = PWR_PROTO_MSG_ATTR_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { PWR_STATE_GET_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_power_protocol_message_attributes,
							attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
			arm_scmi_post_processing_power_protocol_message_attributes
	},
	/*----------Start of Test Case Description 06--------------*/
	/*Here we are checking if the *optional* power state notify command is
	 * supported by power domain mgmt protocol
	 * */
	{.testcase_id = "[power]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_power_state_notify_cmd_support-01",
	 .protocol_id = PWR_PROTO_ID,
	 .message_id = PWR_PROTO_MSG_ATTR_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { PWR_STATE_NOTIFY_MSG_ID },
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_power_protocol_message_attributes,
							attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
			arm_scmi_post_processing_power_protocol_message_attributes
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
};

/*The following data structure captures the test case representation for various
 * test cases identified for power domain management protocol focusing on power
 * domain attribute command.
 */
/*POWER_DOMAIN_ATTRIBUTE - COMMAND TEST LIST*/
arm_scmi_test_case_t power_domain_attr_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the discovery of power domain attributes for all
	 * supported power domain ids.
	 */
	{.testcase_id = "[power]-{POWER_DOMAIN_ATTRIBUTES}-query_power_domain_attributes_alldomainid-01",
	 .protocol_id = PWR_PROTO_ID,
	 .message_id = PWR_DOMAIN_ATTRIB_MSG_ID,
	 .parameter_discovery_driven = true,
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_test_power_domain_attributes
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the power domain attribute command for a negative
	 * scenario. We check if a non-existant power domain id query is handled
	 * correctly
	 * */
	{.testcase_id = "[power]-{POWER_DOMAIN_ATTRIBUTES}-query_non_existant_powerdomainid-01",
	 .protocol_id = PWR_PROTO_ID,
	 .message_id = PWR_DOMAIN_ATTRIB_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter = OFFSET_INVALID_PARAM(
		struct arm_scmi_power_domain_attributes, domain_id),
	 .status = SCMI_STATUS_NOT_FOUND,
	 .custom_tester_func = arm_scmi_custom_test_power_domain_attributes
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	},

};

/*The following data structure captures the test case representation for various
 * test cases identified for power domain management protocol focusing on power
 * state set command
 * */
/*POWER_STATE_SET - COMMAND TEST LIST*/
arm_scmi_test_case_t power_state_set_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the power state set command for a negative scenario.
	  We check if a non-existant power domain id is handled correctly*/
	{.testcase_id = "[power]-{POWER_STATE_SET}-power_state_set_non_existant_domainid-01",
	 .protocol_id = PWR_PROTO_ID,
	 .message_id = PWR_STATE_SET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter =
		OFFSET_INVALID_PARAM(struct arm_scmi_power_state_set, domain_id),
	 .status = SCMI_STATUS_NOT_FOUND,
	 .custom_tester_func = arm_scmi_custom_test_power_state_set
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	},

};

/*The following data structure captures the test case representation for various
 * test cases identified for power domain management protocol focusing on power
 * state get command
 * */
/*POWER_STATE_GET - COMMAND TEST LIST*/
arm_scmi_test_case_t power_state_get_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the getting of power state for all supported power
	 * domain ids.
	 * */
	{.testcase_id = "[power]-{POWER_STATE_GET}-power_state_get_allpowerdomainid-01",
	 .protocol_id = PWR_PROTO_ID,
	 .message_id = PWR_STATE_GET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_test_power_state_get
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the power state get command for a negative scenario.
	  We check if an invalid power domain id is handled correctly.
	  */
	{.testcase_id = "[power]-{POWER_STATE_GET}-power_state_get_invalid_powerdomainid-01",
	 .protocol_id = PWR_PROTO_ID,
	 .message_id = PWR_STATE_GET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter =
		OFFSET_INVALID_PARAM(struct arm_scmi_power_state_get, domain_id),
	 .status = SCMI_STATUS_NOT_FOUND,
	 .custom_tester_func = arm_scmi_custom_test_power_state_get
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	},

};

/*The following data structure captures the test case representation for various
 * test cases identified for power domain management protocol focusing on power
 * state notify command
 * */
/*POWER_STATE_NOTIFY - COMMAND TEST LIST*/
arm_scmi_test_case_t power_state_notify_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/* Here we are testing the power state notify command for all supported
	 * power domain ids.
	 */
	{.testcase_id = "[power]-{POWER_STATE_NOTIFY}-register_power_state_notify_allpowerdomainid-01",
	 .protocol_id = PWR_PROTO_ID,
	 .message_id = PWR_STATE_NOTIFY_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) {PWR_NOTIFY_ENABLE_SET},
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_test_power_state_notify
	},
	/*----------Start of Test Case Description 02--------------*/
	/* Here we are testing the post conditon for registering for power state
	 * notify command for all supported power domain ids.
	 */
	{.testcase_id = "[power]-{POWER_STATE_NOTIFY}-postcondition_register_power_state_notify_allpowerdomainid-01",
	 .protocol_id = PWR_PROTO_ID,
	 .message_id = PWR_STATE_NOTIFY_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) {PWR_NOTIFY_ENABLE_UNSET},
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_test_power_state_notify
	},
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are testing the power state notify command for a negative scenario.
	  We check if an invalid power domain id is handled correctly.
	  */
	{.testcase_id = "[power]-{POWER_STATE_NOTIFY}-register_power_state_notify_invalid_powerdomainid-01",
	 .protocol_id = PWR_PROTO_ID,
	 .message_id = PWR_STATE_NOTIFY_MSG_ID,
	 .invalid_parameter =
			 OFFSET_INVALID_PARAM(struct arm_scmi_power_state_notify, domain_id),
	 .status = SCMI_STATUS_NOT_FOUND,
	 .custom_tester_func = arm_scmi_custom_test_power_state_notify
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	},
};

/*
 * This array is used to process the test suites within the power_exe file.
 * The test suite index within the array must correspond to the test suite id in
 * the enum scmi_test_power_test_suite_ids.
 */
arm_scmi_test_suite_t arm_scmi_power_test_suites[] = {
	{power_discovery_test_suite},
	{power_domain_attr_test_suite},
	{power_state_set_test_suite},
	{power_state_get_test_suite},
	{power_state_notify_test_suite}
	};

const size_t arm_scmi_power_test_suites_size =
		NUM_ELEMS(arm_scmi_power_test_suites);

/*
 * Protocol executor function to be called in the test_executor file.
 */
struct arm_scmi_build_protocol_executor arm_scmi_power_protocol_executor = {
	.protocol_id = PWR_PROTO_ID,
	.protocol_executor = arm_scmi_power_execute
};
/*
 * This structure is used to record the Power Protocol test case results.
 */
struct arm_scmi_protocol_report arm_scmi_power_protocol_report = {
	.protocol_id = PWR_PROTO_ID,
	.test_suite_report_size = NUM_ELEMS(arm_scmi_power_test_suites),
	/* The test suite index within the array must correspond to the test
	 * suite id in the enum scmi_test_power_test_suite_ids.
	 */
	.test_suite_report_table = (struct arm_scmi_test_suite_report[]) {
		{
			.test_suite_name = "POWER_DISCOVERY",
			.test_case_count = COUNT_TEST_CASES(power_discovery_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(power_discovery_test_suite)]) {}
		},
		{
			.test_suite_name = "POWER_DOMAIN_ATTRIBUTES",
			.test_case_count =
				COUNT_TEST_CASES(power_domain_attr_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(power_domain_attr_test_suite)]) {}
		},
		{
			.test_suite_name = "POWER_STATE_SET",
			.test_case_count =
				COUNT_TEST_CASES(power_state_set_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(power_state_set_test_suite)]) {}
		},
		{
			.test_suite_name = "POWER_STATE_GET",
			.test_case_count =
				COUNT_TEST_CASES(power_state_get_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(power_state_get_test_suite)]) {}
		},
		{
			.test_suite_name = "POWER_STATE_NOTIFY",
			.test_case_count =
				COUNT_TEST_CASES(power_state_notify_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(power_state_notify_test_suite)]) {}
		}
	}
};
