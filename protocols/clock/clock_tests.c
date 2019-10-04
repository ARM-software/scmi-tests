/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <clock_common.h>
#include <clock_exports.h>
#include <test_agent_exports.h>

/*The following data structure captures the test case representation for various
 * test cases identified for
  top level protocol discovery commands*/
/*CLOCK PROTOCOL - DISCOVERY TEST LIST*/
arm_scmi_test_case_t clock_discovery_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	{.testcase_id = "[clock]-{PROTOCOL_VERSION}-query_protocol_version-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_PROTO_VER_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t [])
	 {
		{
		.type =  T_UINT32_HEX,
		.def = &(return_value_def_t){
			.name = "PROTOCOL VERSION",
			.offset = OFFSET_BYTES_RET(struct arm_scmi_clock_protocol_version,
					version)
			}
		},
		T_EOT_T
	 }
	},
	/*----------Start of Test Case Description 02--------------*/
	{.testcase_id =  "[clock]-{PROTOCOL_ATTRIBUTES}-query_protocol_attributes-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_PROTO_ATTRIB_MSG_ID,
	 .status = SCMI_STATUS_SUCCESS,
	.expected_returns = (return_value_desc_t[])
	 {
		{
		.type = T_BIT_FIELD,
		.value.integer = RESERVED,
		.flags = CHECK,
		.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(struct arm_scmi_clock_attributes,
					attributes),
				.name = "CLOCK ATTRIBUTES_RESERVED",
				.msb = CLK_RESERVED_HIGH,
				.lsb = CLK_RESERVED_LOW
				}
		},
		{
		.type = T_BIT_FIELD,
		.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
					struct arm_scmi_clock_protocol_attributes, attributes),
				.name = "CLOCK ATTRIBUTES_MAX_NUM_PENDING_ASYNC_RATE_CHGS_SUPPORTED",
				.msb = CLK_MAX_NUM_PENDING_ASYNC_RATE_CHGS_SPRT_HIGH,
				.lsb = CLK_MAX_NUM_PENDING_ASYNC_RATE_CHGS_SPRT_LOW
				}
		},
		{
		.type = T_BIT_FIELD,
		.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
					struct arm_scmi_clock_protocol_attributes, attributes),
			.name = "CLOCK ATTRIBUTES_NUMBER_CLOCKS",
			.msb = CLK_NUMBER_CLOCKS_HIGH,
			.lsb = CLK_NUMBER_CLOCKS_LOW
			}
		},
		T_EOT_T
	},
	 .post_proccesing_func = arm_scmi_post_processing_clock_protocol_attributes
	},
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are checking if the mandatory CLOCK_ATTRIBUTES command is
	 * supported by base protocol*/
	{.testcase_id = "[clock]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_clock_attribute_cmd_support-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { CLK_ATTRIB_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
						struct arm_scmi_clock_protocol_message_attributes,
						attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
		arm_scmi_post_processing_clock_protocol_message_attributes
	},
	/*----------Start of Test Case Description 04--------------*/
	/*Here we are checking if the mandatory CLOCK_DESCRIBE_RATES is
	 * supported by base protocol*/
	{.testcase_id =  "[clock]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_clock_describe_rates_cmd_support-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { CLK_DESC_RATE_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
				struct arm_scmi_clock_protocol_message_attributes, attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
		arm_scmi_post_processing_clock_protocol_message_attributes
	},
	/*----------Start of Test Case Description 05--------------*/
	/*Here we are checking if the mandatory CLOCK_RATES_SET is
	 * supported by base protocol*/
	{.testcase_id = "[clock]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_clock_rate_set_cmd_support-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { CLK_RATE_SET_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
						struct arm_scmi_clock_protocol_message_attributes,
						attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
		arm_scmi_post_processing_clock_protocol_message_attributes
	},
	/*----------Start of Test Case Description 06--------------*/
	/*Here we are checking if the mandatory CLOCK_RATES_GET is
	 * supported by base protocol*/
	{.testcase_id = "[clock]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_clock_rate_get_cmd_support-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { CLK_RATE_GET_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
						struct arm_scmi_clock_protocol_message_attributes,
						attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
			T_EOT_T
		 },
	.post_proccesing_func =
		arm_scmi_post_processing_clock_protocol_message_attributes
	},
	/*----------Start of Test Case Description 07--------------*/
	/*Here we are checking if the mandatory CLOCK_CONFIG_SET is
	 * supported*/
	{.testcase_id = "[clock]-{PROTOCOL_MESSAGE_ATTRIBUTES}-query_clock_config_set_cmd_support-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_PROTO_MSG_ATTRIB_MSG_ID,
	 .parameter_count = 1,
	 .parameters = (uint32_t[]) { CLK_CONFIG_SET_MSG_ID },
	 .status = SCMI_STATUS_SUCCESS,
	 .expected_returns = (return_value_desc_t[])
		 {
			{
			.type =  T_UINT32_HEX,
			.value.integer = 0x0,
			.def = &(return_value_def_t){
				.offset = OFFSET_BYTES_RET(
					struct arm_scmi_clock_protocol_message_attributes,
					attributes),
					.name = "MESSAGE ATTRIBUTES"
				}
			},
		T_EOT_T
		},
	.post_proccesing_func =
		arm_scmi_post_processing_clock_protocol_message_attributes
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
};

 /*The following data structure captures the test case representation for
  * various test cases identified for clock management protocol focusing on
  * clock attributes command
  */
 /*CLOCK_ATTRIBUTES - COMMAND TEST LIST*/
 arm_scmi_test_case_t clock_attributes_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the discovery of clock attributes for all supported
	 * clock ids. Do note that some clocks could be present but not accessible
	 * to the calling agent
	 */
	{.testcase_id = "[clock]-{CLOCK_ATTRIBUTES}-query_clock_attributes_allclockid-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_ATTRIB_MSG_ID,
	 .parameter_discovery_driven = true,
	 .status = SCMI_STATUS_NOT_SPECIFIED,
	 .custom_tester_func = arm_scmi_custom_tester_clock_attributes
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the clock attribute command for a negative scenario.
	  We check if a non-existant clock id query is handled correctly*/
	{.testcase_id = "[clock]-{CLOCK_ATTRIBUTES}-query_non_existant_clockid-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_ATTRIB_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter =
		 OFFSET_INVALID_PARAM(struct arm_scmi_clock_attributes, clock_id),
	 .status = SCMI_STATUS_NOT_FOUND,
	 .custom_tester_func = arm_scmi_custom_tester_clock_attributes
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
 };

 /*The following data structure captures the test case representation for
  * various test cases identified for clock management protocol focusing on
  * clock describe rates command.
  */
 /*CLOCK_DESCRIBE_RATES - COMMAND TEST LIST*/
 arm_scmi_test_case_t clock_describe_rates_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the clock describe levels command based on the various
	 * clocks discovered in the previous discovery phase. Note that not all
	 * clocks are 'accessible' to every agent.
	 */
	{.testcase_id = "[clock]-{CLOCK_DESCRIBE_RATES}-query_allowed_clock_rates-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_DESC_RATE_MSG_ID,
	 .parameter_discovery_driven = true,
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_tester_clock_describe_rates
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the clock describe levels command for a negative scenario.
	  We check if a non-existant clock id query is handled correctly*/
	{.testcase_id = "[clock]-{CLOCK_DESCRIBE_RATES}-query_non_existant_clockid-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_DESC_RATE_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter = OFFSET_INVALID_PARAM(
			 struct arm_scmi_clock_describe_rates, clock_id),
	 .status = SCMI_STATUS_NOT_FOUND,
	 .custom_tester_func = arm_scmi_custom_tester_clock_describe_rates
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
 };

 /*The following data structure captures the test case representation for
  * various test cases identified for clock management protocol focusing on
  * clock rate set command.
  */
 /*CLOCK_RATE_SET - COMMAND TEST LIST*/
 arm_scmi_test_case_t clock_rate_set_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/* This is a pre-condition test case to enable all clocks before setting
	 * them.
	  */
	{.testcase_id = "[clock]-{CLOCK_CONFIG_SET}-precondition_clock_rate_set_allowed_clocks-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_CONFIG_SET_MSG_ID,
	 .parameter_count = 2,
	 /* Here the first parameter is only a placeholder, i.e. ignored
	  * the custom tester function will discover the first parameter at run time
	  * and using the second parameter given by the user.
	  */
	 .parameters = (uint32_t[]) {0, CLOCK_ENABLED},
	 .precondition_indicator = PRECOND_DEFINE,
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_tester_clock_config_set
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the clock rate set command based on the various clocks
	  discovered in the previous discovery phase accessible to this agent. We
	  set abs min + 1 higher in the supported rates array. This will be issued
	  as a Sync command.
	  */
	{.testcase_id = "[clock]-{CLOCK_RATE_SET}-clock_rate_set_allowed_clocks-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_RATE_SET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .precondition_indicator = PRECOND_USE,
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_tester_clock_rate_set
	},

	/*----------Start of Test Case Description 03--------------*/
	/*Here we are testing the clock rate set command for a negative scenario.
	  We check if a non-existant clock id query is handled correctly*/
	{.testcase_id =  "[clock]-{CLOCK_RATE_SET}-clock_rate_set_non_existant_clockid-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_RATE_SET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter = OFFSET_INVALID_PARAM(
		struct arm_scmi_clock_rate_set, clock_id),
	 .status = SCMI_STATUS_NOT_FOUND,
	 .custom_tester_func = arm_scmi_custom_tester_clock_rate_set
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
 };

 /*The following data structure captures the test case representation for
  * various test cases identified for clock management protocol focusing on
  * clock rate get command.
  */
 /*CLOCK_RATE_GET - COMMAND TEST LIST*/
 arm_scmi_test_case_t clock_rate_get_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*This is a pre-condition test case. Here we are testing the setting of
	 * clock rate for all the various clocks discovered in the previous
	 * discovery phase. In this test case we set the clock rates as the
	 * abs min + 1 higher value we got from the describe rates query. As this
	 * is a pre-requisite test case, we should issue this as a Sync command so
	 * we are sure the changes is in effect.
	 */
	{.testcase_id = "[clock]-{CLOCK_RATE_SET}-precondition_clock_rate_get_allowed_clocks-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_RATE_SET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .precondition_indicator = PRECOND_DEFINE,
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_tester_clock_rate_set
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the getting of clock rate across all accessible clocks
	 * as set in the previous test case.
	 */
	{.testcase_id = "[clock]-{CLOCK_RATE_GET}-clock_rate_get_allowed_clocks-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_RATE_GET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .precondition_indicator = PRECOND_USE,
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_tester_clock_rate_get
	},
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are testing the clock rate get command for a negative scenario.
	  We check if a non-existant clock id query is handled correctly*/
	{.testcase_id = "[clock]-{CLOCK_RATE_GET}-clock_rate_get_non_existant_clockid-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_RATE_GET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter = OFFSET_INVALID_PARAM(struct arm_scmi_clock_rate_get,
			 clock_id),
	 .status = SCMI_STATUS_NOT_FOUND,
	 .custom_tester_func = arm_scmi_custom_tester_clock_rate_get
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
 };

 /*The following data structure captures the test case representation for
  * various test cases identified for clock management protocol focusing on
  * clock rate set command.
  */
 /*CLOCK_CONFIG_SET - COMMAND TEST LIST*/
 arm_scmi_test_case_t clock_config_set_test_suite[] = {
	/*----------Start of Test Case Description 01--------------*/
	/*Here we are testing the clock config set command based on the various
	 * clocks discovered in the previous discovery phase accessible to this
	 * agent.
	 * */
	{.testcase_id = "[clock]-{CLOCK_CONFIG_SET}-clock_config_set_enabled_allowed_clocks-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_CONFIG_SET_MSG_ID,
	 .parameter_count = 2,
	 .parameters = (uint32_t[]) {0, CLOCK_ENABLED},
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_tester_clock_config_set
	},
	/*Here we are testing the post-condition for clock config set (enabled)
	 * command based on the various clocks discovered in the previous discovery
	 * phase accessible to this agent.
	 * */
	{.testcase_id = "[clock]-{CLOCK_CONFIG_SET}-postcondition_clock_config_set_enabled_allowed_clocks-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_ATTRIB_MSG_ID,
	 .parameter_discovery_driven = true,
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_tester_clock_attributes
	},
	/*----------Start of Test Case Description 02--------------*/
	/*Here we are testing the clock config set command based (disabled) on the
	 * various clocks discovered in the previous discovery phase accessible to
	 * this agent.
	 * */
	{.testcase_id = "[clock]-{CLOCK_CONFIG_SET}-clock_config_set_disabled_allowed_clocks-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_CONFIG_SET_MSG_ID,
	 .parameter_count = 2,
	 .parameters = (uint32_t[]) {0, CLOCK_DISABLED},
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_tester_clock_config_set
	},
	/*Here we are testing the post-condition for clock config set command based
	 * on the various clocks discovered in the previous discovery phase
	 * accessible to this agent.
	 * */
	{.testcase_id = "[clock]-{CLOCK_CONFIG_SET}-postcondition_clock_config_set_disabled_allowed_clocks-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_ATTRIB_MSG_ID,
	 .parameter_discovery_driven = true,
	 .status = SCMI_STATUS_SUCCESS,
	 .custom_tester_func = arm_scmi_custom_tester_clock_attributes
	},
	/*----------Start of Test Case Description 03--------------*/
	/*Here we are testing the clock config set command for a negative scenario.
	  We check if a non-existant clock id query is handled correctly*/
	{.testcase_id =  "[clock]-{CLOCK_CONFIG_SET}-clock_config_set_non_existant_clockid-01",
	 .protocol_id = CLK_PROTO_ID,
	 .message_id = CLK_CONFIG_SET_MSG_ID,
	 .parameter_discovery_driven = true,
	 .invalid_parameter = OFFSET_INVALID_PARAM(struct arm_scmi_clock_config_set,
		clock_id),
	 .status = SCMI_STATUS_NOT_FOUND,
	 .custom_tester_func = arm_scmi_custom_tester_clock_config_set
	},
	/*----------End of test case list--------------*/
	{.testcase_id = "END_OF_TEST_LIST",
	}
 };

 /*
  * This array is used to process the test suites within the clock_exe file.
  * The test suite index within the array must correspond to the test suite id in
  * the enum scmi_test_clock_test_suite_ids.
  */
 arm_scmi_test_suite_t arm_scmi_clock_test_suites[] = {
 	{clock_discovery_test_suite},
 	{clock_attributes_test_suite},
 	{clock_describe_rates_test_suite},
 	{clock_rate_set_test_suite},
 	{clock_rate_get_test_suite},
	{clock_config_set_test_suite}
 	};

 const size_t arm_scmi_clock_test_suites_size =
	NUM_ELEMS(arm_scmi_clock_test_suites);

 /*
  * Protocol executor function to be called in the test_executor file.
  */
 struct arm_scmi_build_protocol_executor arm_scmi_clock_protocol_executor =
 {
 	.protocol_id = CLK_PROTO_ID,
 	.protocol_executor = arm_scmi_clock_execute
 };

 /*
  * This structure is used to record the Clock Protocol test case results.
  */
 struct arm_scmi_protocol_report arm_scmi_clock_protocol_report = {
 	.protocol_id = CLK_PROTO_ID,
 	.test_suite_report_size = NUM_ELEMS(arm_scmi_clock_test_suites),
 	/* The test suite index within the array must correspond to the test
 	 * suite id in the enum scmi_test_clock_test_suite_ids.
 	 */
 	.test_suite_report_table = (struct arm_scmi_test_suite_report[]) {
 		{
 			.test_suite_name = "CLOCK_DISCOVERY",
 			.test_case_count = COUNT_TEST_CASES(clock_discovery_test_suite),
 			.test_case_report_table = (struct arm_scmi_test_case_report[
 				COUNT_TEST_CASES(clock_discovery_test_suite)]) {}
 		},
 		{
 			.test_suite_name = "CLOCK_ATTRIBUTES",
 			.test_case_count =
 				COUNT_TEST_CASES(clock_attributes_test_suite),
 			.test_case_report_table = (struct arm_scmi_test_case_report[
 				COUNT_TEST_CASES(clock_attributes_test_suite)]) {}
 		},
 		{
 			.test_suite_name = "CLOCK_DESCRIBE_RATES",
 			.test_case_count =
 				COUNT_TEST_CASES(clock_describe_rates_test_suite),
 			.test_case_report_table = (struct arm_scmi_test_case_report[
 				COUNT_TEST_CASES(clock_describe_rates_test_suite)]) {}
 		},
 		{
 			.test_suite_name = "CLOCK_RATE_SET",
 			.test_case_count =
 				COUNT_TEST_CASES(clock_rate_set_test_suite),
 			.test_case_report_table = (struct arm_scmi_test_case_report[
 				COUNT_TEST_CASES(clock_rate_set_test_suite)]) {}
 		},
 		{
 			.test_suite_name = "CLOCK_RATE_GET",
 			.test_case_count =
 				COUNT_TEST_CASES(clock_rate_get_test_suite),
 			.test_case_report_table = (struct arm_scmi_test_case_report[
 				COUNT_TEST_CASES(clock_rate_get_test_suite)]) {}
 		},
 		{
 			.test_suite_name = "CLOCK_CONFIG_SET",
 			.test_case_count =
 				COUNT_TEST_CASES(clock_config_set_test_suite),
 			.test_case_report_table = (struct arm_scmi_test_case_report[
 				COUNT_TEST_CASES(clock_config_set_test_suite)]) {}
 		}
 	}
 };
