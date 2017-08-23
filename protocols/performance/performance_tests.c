/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <test_agent_exports.h>
#include <performance_common.h>
#include <performance_exports.h>

/*The following data structure captures the test case representation for various test cases identified for
  performance domain management protocol focusing on Discovery commands*/
arm_scmi_test_case_t performance_discovery_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	{.testcase_id = "[performance]-{PROTOCOL_VERSION}-query_protocol_version-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_MGMT_PROTO_VER_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t [])
	 {
		{
		.type =  T_UINT32_HEX,
		.def = &(return_value_def_t){
			.name = "PROTOCOL VERSION",
			.offset = OFFSET_BYTES_RET(struct arm_scmi_performance_protocol_version,
					version)
			}
		},
		T_EOT_T
	 }
	},
	/*----------Start of Test Case Description 02--------------*/
	{.testcase_id =  "[performance]-{PROTOCOL_ATTRIBUTES}-query_protocol_attributes-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_MGMT_PROTO_ATTRIB_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
	 {
		{
		.type = T_BIT_FIELD,
		.value.integer = RESERVED,
		.flags = CHECK,
		.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
					struct arm_scmi_performance_protocol_attributes, attributes),
				.name = "PERFORMANCE ATTRIBUTES_RESERVED",
				.msb = PERFORMANCE_PROT_ATTR_PROT_RESERVED_HIGH,
				.lsb = PERFORMANCE_PROT_ATTR_PROT_RESERVED_LOW
				}
		},
		{
		.type = T_BIT_FIELD,
		.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
					struct arm_scmi_performance_protocol_attributes, attributes),
				.name = "PERFORMANCE POWER VALUE MILLIWATTS",
				.msb = PERFORMANCE_PROT_POWER_VALUES_HIGH,
				.lsb = PERFORMANCE_PROT_POWER_VALUES_LOW
				}
		},
		{
		.type = T_BIT_FIELD,
		.def = &(return_value_def_t){
			.offset = OFFSET_BYTES_RET(
				struct arm_scmi_performance_protocol_attributes, attributes),
			.name = "PERFORMANCE ATTRIBUTES NUMBER PERFORMANCE DOMAINS",
			.msb = PERFORMANCE_PROT_ATTR_NUMBER_DOMAINS_HIGH,
			.lsb = PERFORMANCE_PROT_ATTR_NUMBER_DOMAINS_LOW
			}
		},
		T_EOT_T
	},
	.post_proccesing_func =
		arm_scmi_post_processing_performance_protocol_attributes
	},
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are checking if the mandatory performance domain attribute command is supported by performance domain protocol*/
	{.testcase_id = "[performance]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_performance_domain_attrib_cmd_support-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_MGMT_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { PERF_DOMAIN_ATTRIB_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_performance_protocol_message_attributes,
							attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
			arm_scmi_post_processing_performance_protocol_message_attributes
	},
	/*----------Start of Test Case Description 04--------------*/
	/*Here we are checking if the mandatory performance describe level command
	 * is supported by performance domain protocol
	 * */
	{.testcase_id = "[performance]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_performance_describe_levels_cmd-support-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_MGMT_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { PERF_DESC_LVL_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_performance_protocol_message_attributes,
							attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
		arm_scmi_post_processing_performance_protocol_message_attributes
	},
	/*----------Start of Test Case Description 05--------------*/
	/*Here we are checking if the mandatory performance limits set command is supported by performance domain protocol*/
	{.testcase_id = "[performance]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_performance_set_limits_cmd-support-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_MGMT_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { PERF_LIMIT_SET_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_performance_protocol_message_attributes,
							attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
		arm_scmi_post_processing_performance_protocol_message_attributes
	},
	/*----------Start of Test Case Description 06--------------*/
	/*Here we are checking if the mandatory performance limits get command is supported by performance domain protocol*/
	{.testcase_id = "[performance]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_performance_get_limits_cmd-support-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_MGMT_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { PERF_LIMIT_GET_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_performance_protocol_message_attributes,
							attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
		arm_scmi_post_processing_performance_protocol_message_attributes
	},
	/*----------Start of Test Case Description 07--------------*/
	/*Here we are checking if the mandatory performance level set command is supported by performance domain protocol*/
	{.testcase_id = "[performance]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_performance_level_set_cmd-support-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_MGMT_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { PERF_LVL_SET_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_performance_protocol_message_attributes,
							attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
		arm_scmi_post_processing_performance_protocol_message_attributes
	},
	/*----------Start of Test Case Description 08--------------*/
	/*Here we are checking if the mandatory performance level get command is supported by performance domain protocol*/
	{.testcase_id = "[performance]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_performance_level_get_cmd-support-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_MGMT_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { PERF_LVL_GET_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_performance_protocol_message_attributes,
							attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
		arm_scmi_post_processing_performance_protocol_message_attributes
	},
	/*----------Start of Test Case Description 09--------------*/
	/*Here we are checking if the *optional* performance notify limits command is supported by performance domain protocol*/
	{.testcase_id = "[performance]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_performance_notify_limit_cmd-support-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_MGMT_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { PERF_NOTIFY_LIMIT_MSG_ID },
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_performance_protocol_message_attributes,
							attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
		arm_scmi_post_processing_performance_protocol_message_attributes
	},
	/*----------Start of Test Case Description 10--------------*/
	/*Here we are checking if the *optional* performance notify level command is supported by performance domain protocol*/
	{.testcase_id = "[performance]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_performance_notify_level_cmd_support-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_MGMT_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { PERF_NOTIFY_LVL_MSG_ID },
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_performance_protocol_message_attributes,
							attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
		arm_scmi_post_processing_performance_protocol_message_attributes
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}

};

/*The following data structure captures the test case representation for various test cases identified for
  performance domain management protocol focusing on performance domain attributes command*/
arm_scmi_test_case_t performance_domain_attr_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the performance domain attribute command based on the various performance domains
	  discovered in the previous discovery phase*/
	{.testcase_id = "[performance]-{PERFORMANCE_DOMAIN_ATTRIBUTES}-query_performance_domain_attributes_alldomainid-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_DOMAIN_ATTRIB_MSG_ID,
	 .parameter_discovery_driven = true,
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	 .custom_tester_func = arm_scmi_custom_test_performance_domain_attributes
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the performance domain attribute command for a negative scenario.
	  We check if a non-existent domain id query is handled correctly*/
	{.testcase_id = "[performance]-{PERFORMANCE_DOMAIN_ATTRIBUTES}-query_non_existant_performance_domainid-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_DOMAIN_ATTRIB_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter =
		 OFFSET_INVALID_PARAM(struct arm_scmi_performance_domain_attributes,
			domain_id),
	 .status = SCMI_STATUS_NOT_FOUND,
	 .custom_tester_func = arm_scmi_custom_test_performance_domain_attributes
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	},

};

/*The following data structure captures the test case representation for various test cases identified for
  performance domain management protocol focusing on performance describe levels command*/
arm_scmi_test_case_t performance_describe_levels_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the performance describe levels command based on the various performance domains
	  discovered in the previous discovery phase*/
	{.testcase_id = "[performance]-{PERFORMANCE_DESCRIBE_LEVELS}-query_performance_describe_levels_alldomainid-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_DESC_LVL_MSG_ID,
	 .parameter_discovery_driven = true,
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_test_performance_describe_levels
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the performance describe levels command for a negative scenario.
	  We check if a non-existant domain id query is handled correctly*/
	{.testcase_id = "[performance]-{PERFORMANCE_DESCRIBE_LEVELS}-query_performance_describe_levels_non_existant_domainid-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_DESC_LVL_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter =
		 OFFSET_INVALID_PARAM(struct arm_scmi_performance_describe_levels,
			domain_id),
	 .status = SCMI_STATUS_NOT_FOUND,
	 .custom_tester_func = arm_scmi_custom_test_performance_describe_levels
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
};

/*The following data structure captures the test case representation for various test cases identified for
  performance domain management protocol focusing on performance limits set command*/
/*PERFORMANCE_LIMITS_SET - COMMAND TEST LIST*/
arm_scmi_test_case_t performance_limits_set_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the setting of performance limit for all the various
	 * performance domains discovered in the previous discovery phase. In this
	 * test case we set the min limit as the abs min + 1 higher value we got
	 * from the describe levels query and abs max - 1 lower value we got from
	 * the describe levels query
	 * */
	{.testcase_id = "[performance]-{PERFORMANCE_LIMITS_SET}-performance_limits_set_alldomainid-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_LIMIT_SET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	 .custom_tester_func = arm_scmi_custom_test_performance_limits_set
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the setting of performance limit for a negative scenario.
	  We check if a non-existant domain id input passed is handled correctly
	  */
	{.testcase_id = "[performance]-{PERFORMANCE_LIMITS_SET}-performance_limits_set_invalid_domainid-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_LIMIT_SET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter = OFFSET_INVALID_PARAM(
		struct arm_scmi_performance_limits_set, domain_id),
	 .status = SCMI_STATUS_NOT_FOUND,
	 .custom_tester_func = arm_scmi_custom_test_performance_limits_set
	},
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are testing the setting of performance limit for a negative
	 * scenario. We check if a an out of range max perf level being set is
	 * handled correctly. In this case we attempt to set the max range beyond
	 * the valid max
	 * */
	{.testcase_id = "[performance]-{PERFORMANCE_LIMITS_SET}-performance_limits_set_over_range-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_LIMIT_SET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter = OFFSET_INVALID_PARAM(
		struct arm_scmi_performance_limits_set, range_max),
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	 .custom_tester_func = arm_scmi_custom_test_performance_limits_set
	},
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are testing the setting of performance limit for a negative scenario.
	  We check if a an out of range max perf level being set is handled correctly
	  In this case we attempt to set the max range beyond the valid max*/
	{.testcase_id = "[performance]-{PERFORMANCE_LIMITS_SET}-performance_limits_set_under_range-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_LIMIT_SET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter = OFFSET_INVALID_PARAM(
				struct arm_scmi_performance_limits_set, range_min),
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	 .custom_tester_func = arm_scmi_custom_test_performance_limits_set
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST"
	}

};

/*The following data structure captures the test case representation for various test cases identified for
  performance domain management protocol focusing on performance level get command*/
/*PERFORMANCE_LEVEL_GET - COMMAND TEST LIST*/
arm_scmi_test_case_t performance_limits_get_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	{.testcase_id = "[performance]-{PERFORMANCE_LIMITS_SET}-precondition_performance_limits_get_alldomainid-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_LIMIT_SET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .precondition_indicator = PRECOND_DEFINE,
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_test_performance_limits_set
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the getting of performance level across each domain as
	 * set in the previous test case.
	 */
	{.testcase_id = "[performance]-{PERFORMANCE_LIMITS_GET}-performance_limits_get_all_domainid-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_LIMIT_GET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .precondition_indicator = PRECOND_USE,
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_test_performance_limits_get
	},
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are testing the getting of performance level for a negative
	 * scenario. We check if an invalid domain id passed is handled correctly
	 */
	{.testcase_id = "[performance]-{PERFORMANCE_LIMITS_GET}-performance_limits_get_invalid_domainid-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_LIMIT_GET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter = OFFSET_INVALID_PARAM(
				struct arm_scmi_performance_limits_get, domain_id),
	 .status = SCMI_STATUS_NOT_FOUND,
	 .custom_tester_func = arm_scmi_custom_test_performance_limits_get
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST"
	}
};

/*The following data structure captures the test case representation for various
 * test cases identified for performance domain management protocol focusing on
 * performance level set command
 */
/*PERFORMANCE_LEVEL_SET - COMMAND TEST LIST*/
arm_scmi_test_case_t performance_levels_set_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the setting of performance level for all the various
	 * performance domains discovered in the previous discovery phase. In this
	 * test case we set the perf level as abs min + 1 higher value we got from
	 * the describe levels query
	 */
	{.testcase_id = "[performance]-{PERFORMANCE_LEVEL_SET}-performance_level_set_all_domainid-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_LVL_SET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	 .custom_tester_func = arm_scmi_custom_test_performance_levels_set
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the setting of performance level for a negative
	 * scenario. We check if an invalid domain id passed is handled correctly
	 */
	{.testcase_id = "[performance]-{PERFORMANCE_LEVEL_SET}-performance_level_set_invalid_domainid-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_LVL_SET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter = OFFSET_INVALID_PARAM(
				struct arm_scmi_performance_levels_set, domain_id),
	 .status = SCMI_STATUS_NOT_FOUND,
	 .custom_tester_func = arm_scmi_custom_test_performance_levels_set
	},
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are testing the setting of performance level for a negative
	 * scenario. We check if setting a perf level larger than max limit is
	 * handled correctly
	 */
	{.testcase_id = "[performance]-{PERFORMANCE_LEVEL_SET}-performance_level_set_invalid_level-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_LVL_SET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter = OFFSET_INVALID_PARAM(
				struct arm_scmi_performance_levels_set, performance_level),
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	 .custom_tester_func = arm_scmi_custom_test_performance_levels_set
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST"
	}
};

/*The following data structure captures the test case representation for various test cases identified for
  performance domain management protocol focusing on performance level get command*/
/*PERFORMANCE_LEVEL_GET - COMMAND TEST LIST*/
arm_scmi_test_case_t performance_levels_get_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/* Precondition for get values */
	{.testcase_id = "[performance]-{PERFORMANCE_LEVEL_SET}-precondition_performance_level_get_alldomainid-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_LVL_SET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .precondition_indicator = PRECOND_DEFINE,
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_test_performance_levels_set
	},
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the getting of performance level across each domain as
	 * set in the previous test case.
	 */
	{.testcase_id = "[performance]-{PERFORMANCE_LEVEL_GET}-performance_level_get_alldomainid-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_LVL_GET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .precondition_indicator = PRECOND_USE,
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_test_performance_levels_get
	},
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are testing the getting of performance level for a negative
	 * scenario. We check if an invalid domain id passed is handled correctly.
	 */
	{.testcase_id = "[performance]-{PERFORMANCE_LEVEL_GET}-performance_level_get_invalid_domainid-01",
	 .protocol_id = PERF_MGMT_PROTO_ID,
	 .message_id = PERF_LVL_GET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter = OFFSET_INVALID_PARAM(
				struct arm_scmi_performance_levels_get, domain_id),
	 .status = SCMI_STATUS_NOT_FOUND,
	 .custom_tester_func = arm_scmi_custom_test_performance_levels_get
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST"
	}
};
/*
 * This array is used to process the test suites within the performance_exe file.
 * The test suite index within the array must correspond to the test suite id in
 * the enum scmi_test_power_test_suite_ids.
 */
arm_scmi_test_suite_t arm_scmi_performance_test_suites[] = {
	{performance_discovery_test_suite},
	{performance_domain_attr_test_suite},
	{performance_describe_levels_test_suite},
	{performance_limits_set_test_suite},
	{performance_limits_get_test_suite},
	{performance_levels_set_test_suite},
	{performance_levels_get_test_suite}
	};

const size_t arm_scmi_performance_test_suites_size =
		NUM_ELEMS(arm_scmi_performance_test_suites);

/*
 * Protocol executor function to be called in the test_executor file.
 */
struct arm_scmi_build_protocol_executor arm_scmi_performance_protocol_executor =
{
	.protocol_id = PERF_MGMT_PROTO_ID,
	.protocol_executor = arm_scmi_performance_execute
};

/*
 * This structure is used to record the Power Protocol test case results.
 */
struct arm_scmi_protocol_report arm_scmi_performance_protocol_report = {
	.protocol_id = PERF_MGMT_PROTO_ID,
	.test_suite_report_size = NUM_ELEMS(arm_scmi_performance_test_suites),
	/* The test suite index within the array must correspond to the test
	 * suite id in the enum scmi_test_performance_test_suite_ids.
	 */
	.test_suite_report_table = (struct arm_scmi_test_suite_report[]) {
		{
			.test_suite_name = "PERFORMANCE_DISCOVERY",
			.test_case_count = COUNT_TEST_CASES(performance_discovery_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(performance_discovery_test_suite)]) {}
		},
		{
			.test_suite_name = "PERFORMANCE_DOMAIN_ATTRIBUTES",
			.test_case_count =
				COUNT_TEST_CASES(performance_domain_attr_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(performance_domain_attr_test_suite)]) {}
		},
		{
			.test_suite_name = "PERFORMANCE_DESCRIBE_LEVELS",
			.test_case_count =
				COUNT_TEST_CASES(performance_describe_levels_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(performance_describe_levels_test_suite)]) {}
		},
		{
			.test_suite_name = "PERFORMANCE_LIMITS_SET",
			.test_case_count =
				COUNT_TEST_CASES(performance_limits_set_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(performance_limits_set_test_suite)]) {}
		},
		{
			.test_suite_name = "PERFORMANCE_LIMITS_GET",
			.test_case_count =
				COUNT_TEST_CASES(performance_limits_get_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(performance_limits_get_test_suite)]) {}
		},
		{
			.test_suite_name = "PERFORMANCE_LEVEL_SET",
			.test_case_count =
				COUNT_TEST_CASES(performance_levels_set_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(performance_levels_set_test_suite)]) {}
		},
		{
			.test_suite_name = "PERFORMANCE_LEVEL_GET",
			.test_case_count =
				COUNT_TEST_CASES(performance_levels_get_test_suite),
			.test_case_report_table = (struct arm_scmi_test_case_report[
				COUNT_TEST_CASES(performance_levels_get_test_suite)]) {}
		}
	}
};
