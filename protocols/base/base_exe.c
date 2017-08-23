/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <protocol_common.h>
#include <base_expected.h>
#include <base_common.h>
#include <base_exports.h>

struct arm_scmi_base_global_data arm_scmi_base_protocol_data;
static struct arm_scmi_base_expected *base_expected =
	&(struct arm_scmi_base_expected){};
static uint32_t expected_flags_mask;

/*!
 * @brief Auxiliary function to test if the agent id 0 name starts with the
 * the given word.
 *
 * @param expected_start_word Agent id 0 expected start word.
 * @param returned Returned Agent id 0 name.
 *
 * @returns PASSED if starts with the word, FAILED otherwise.
 *
 */
static enum test_status check_agent_name_starts(const char *expected_word,
	const char *returned)
{
   if(strncmp(returned, expected_word, strlen(expected_word)) == 0)
	   return PASSED;
   return FAILED;
}

/*!
 * @brief Function used to fill up expected test case return values from
 * expected protocol return values.
 *
 *  The function is called by the test engine just before sending
 *  the command in the test case and testing the received data. It also
 *  sets other hooks depending on the message id. To see the expected_returns
 *  index meaning see 'base_tests.c' for each specific command.
 *
 *  @param test_case Pointer to the test case being executed in the test engine.
 *
 */
static void base_fill_expected_return_values(arm_scmi_test_case_t *test_case)
{
	return_value_desc_t *expected_ret;

	switch (test_case->message_id)
	{
		case BASE_PROTO_VERSION_MSG_ID:
			expected_ret = &test_case->expected_returns[0];
			expected_ret->value.integer = base_expected->protocol_version;
			expected_ret->flags = arm_scmi_print_or_check(BASE_PROTOCOL_VERSION,
							expected_flags_mask);
			break;
		case BASE_PROTO_ATTR_MSG_ID:
			expected_ret = &test_case->expected_returns[1];
			expected_ret->value.integer = base_expected->num_agents;
			expected_ret->flags = arm_scmi_print_or_check(NUMBER_AGENTS,
						expected_flags_mask);
			expected_ret = &test_case->expected_returns[2];
			expected_ret->value.integer =
						base_expected->num_protocols_implemented;
			expected_ret->flags =
						arm_scmi_print_or_check(NUM_PROTOCOLS_IMPLEMENTED,
								expected_flags_mask) ;
			break;
		case BASE_PROTO_MSG_ATTR_MSG_ID:
			expected_ret = &test_case->expected_returns[0];
			expected_ret->flags = CHECK;
			break;
		case BASE_DISC_VENDOR_MSG_ID:
			expected_ret = &test_case->expected_returns[0];
			expected_ret->value.string = base_expected->vendor_identifier;
			expected_ret->flags = arm_scmi_print_or_check(VENDOR_IDENTIFIER,
					expected_flags_mask);
			break;
		case BASE_DISC_SUBVENDOR_MSG_ID:
			expected_ret = &test_case->expected_returns[0];
			expected_ret->value.string = base_expected->subvendor_identifier;
			expected_ret->flags = arm_scmi_print_or_check(SUBVENDOR_IDENTIFIER,
									expected_flags_mask);
			break;
		case BASE_DISC_IMPLEMENTATION_VERSION_MSG_ID:
			expected_ret = &test_case->expected_returns[0];
			expected_ret->value.integer = base_expected->implementation_version;
			expected_ret->flags = arm_scmi_print_or_check(
					IMPLEMENTATION_VERSION, expected_flags_mask);
			break;
		case BASE_DISC_PROTO_LIST_MSG_ID:
			/* Custom tester */
			break;
		case BASE_DISC_AGENT_MSG_ID:
			/* Custom tester */
			break;
		case BASE_NOTIFY_ERRORS_MSG_ID:
			/* No return values */
			break;
	}
}

/*!
 * @brief Setup the execution of the base protocol tests..
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param expected Pointer to the expected returned values.
 *
 */
static void setup_base(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		struct arm_scmi_base_expected *expected)
{
	uint8_t index;
	struct arm_scmi_test_suite_report *tsr =
			arm_scmi_base_protocol_report.test_suite_report_table;
	/* If no expected values are provided (i.e. NULL) it is assigned an
	 * empty compound literal in order to avoid if != NULL in every
	 * base_expected value.
	 */
	if (expected == NULL) {
		expected_flags_mask = 0;
	} else {
		base_expected = expected;
		expected_flags_mask = expected->flags_mask;
	};
	arm_scmi_init_protocol_execution_context(protocol_execution_context,
			base_fill_expected_return_values, &arm_scmi_base_protocol_report);
	arm_scmi_set_test_suite_table(protocol_execution_context,
			arm_scmi_test_base_test_suites, arm_scmi_test_base_test_suites_size);
	for (index = 0; index < arm_scmi_test_base_test_suites_size; ++index)
		arm_scmi_test_base_test_suites[index].name = (tsr + index)->test_suite_name;
	arm_scmi_skip_test_suite(protocol_execution_context, TS_BASE_DISCOVERY,
			false);
	arm_scmi_log_protocol_name(&protocol_execution_context->test_stats, "Base");
}

/*!
 * @brief Check if a command is supported.
 *
 *  The function is called by the test engine after testing the
 *  PROTOCOL_MESSAGE_ATTRIBUTES message id in order to set a test suite
 *  if the command is supported
 *
 *  @param protocol_execution_context Reference protocol execution test context.
 *  @param test_case Pointer to the current test case.
 *  @param received Pointer to the received message structure.
 *
 */
void arm_scmi_post_processing_base_protocol_message_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case, struct arm_scmi_message *received)
{
	uint32_t mask;
	int index;
	bool must_be_supported = true;
	bool must_test_suite = true;
	uint32_t message_id = test_case->parameters[
		OFFSET_PARAM(struct arm_scmi_base_protocol_message_attributes,
				message_id)];

	switch (message_id) {
		case BASE_DISC_SUBVENDOR_MSG_ID:
			mask = LEFT_SHIFT(SUBVENDOR_CMD_SUPPORTED);
			index = TS_BASE_SUBVENDOR;
			must_be_supported = base_expected->subvendor_cmd_supported;
			break;
		case BASE_DISC_AGENT_MSG_ID:
			mask = LEFT_SHIFT(DISCOVER_AGENT_CMD_SUPPORTED);
			index = TS_BASE_AGENT;
			must_be_supported = base_expected->discover_agent_cmd_supported;
			break;
		case BASE_NOTIFY_ERRORS_MSG_ID:
			mask = LEFT_SHIFT(ERROR_NOTIFICATION_SUPPORTED);
			index = TS_BASE_NOTIFY_ERRORS;
			must_be_supported = base_expected->notify_error_cmd_supported;
			break;
		case BASE_DISC_VENDOR_MSG_ID:
			mask = ALL_BITS_SET;
			index = TS_BASE_VENDOR;
			break;
		case BASE_DISC_IMPLEMENTATION_VERSION_MSG_ID:
			mask = ALL_BITS_SET;
			index = TS_BASE_IMPLEMENTATION;
			break;
		case BASE_DISC_PROTO_LIST_MSG_ID:
			mask = ALL_BITS_SET;
			index = TS_BASE_PROTOCOLS;
			break;
	}
	must_test_suite = arm_scmi_check_suite_skipped(
			&protocol_execution_context->test_stats, mask,
			expected_flags_mask, must_be_supported, message_id,
			received->status);
	arm_scmi_skip_test_suite(protocol_execution_context, index,
			!must_test_suite);
}

/*!
 * @brief Function used to discover agent names.
 *
 *  The function is called by the test engine just before sending
 *  the command and testing the received data.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_tester_base_disco_agent_msg(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case)
{
	uint32_t agent_id;
	int error_code;
	enum test_status res;
	char *agent_rcv, *agent_exp;

	/* For all test cases allocate space for parameters */
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
			struct arm_scmi_base_discover_agent, agent_id) ) {
		/* Negative test case */
		test_case->parameter_count = 1;
		test_case->parameters[
		OFFSET_PARAM(struct arm_scmi_base_discover_agent, agent_id)] =
				arm_scmi_base_protocol_data.num_agents + 1;
		error_code = arm_scmi_execute_and_assert_test_case(
				protocol_execution_context, test_case, &received);
	} else if (!test_case->parameter_discovery_driven) {
		/* Agent id 0 */
		test_case->parameter_count = 1;
		test_case->parameters[
		OFFSET_PARAM(struct arm_scmi_base_discover_agent, agent_id)] = 0;
		error_code = arm_scmi_execute_and_assert_test_case(
				protocol_execution_context, test_case, &received);
		if ((error_code != NO_ERROR) || (received.status < 0))
			return;
		res = check_agent_name_starts("platform",
			RETURN_STR(received, struct arm_scmi_base_discover_agent, name));
		snprintf(description, NUM_ELEMS(description), "%s", ((res == PASSED) ?
			RETURN_STR(received, struct arm_scmi_base_discover_agent, name):
			"Agent name does not start with 'platform'"));
		arm_scmi_log_test_result(&protocol_execution_context->test_stats, res,
			"AGENT NAME", description);
	} else {
		/* Look for agent names using agent id's */
		test_case->parameter_count = 1;
		for (agent_id = 1; agent_id <= arm_scmi_base_protocol_data.num_agents;
				++agent_id)
		{
			test_case->parameters[
			OFFSET_PARAM(struct arm_scmi_base_discover_agent, agent_id)] =
					agent_id;
			error_code = arm_scmi_execute_and_assert_test_case(
					protocol_execution_context, test_case, &received);
			/* The status must have positive numbers (including zero) to be
			 * successful because identify the caller agent id.
			 */
			if ((error_code != NO_ERROR) || (received.status < 0))
				continue;
			agent_rcv = RETURN_STR(received,
				struct arm_scmi_base_discover_agent, name);
			res = INFO;
			if ((LEFT_SHIFT(AGENT_NAMES) & expected_flags_mask))
			{
				agent_exp = base_expected->agent_names[agent_id - 1];
				res = (strcmp(agent_rcv, agent_exp) == 0) ? PASSED: FAILED;
			}
			if (res == FAILED) {
				snprintf(description, NUM_ELEMS(description),
						"Expected: %s, Received: %s", agent_exp, agent_rcv);
			}
			else if ((res == PASSED) || (res == INFO)) {
				snprintf(description, NUM_ELEMS(description), "'%s'", agent_rcv);
			}
			snprintf(name, NUM_ELEMS(name), "AGENT NAME");
			arm_scmi_log_test_result(&protocol_execution_context->test_stats,
				res, name, description);
		}
	}
}

/*!
 * @brief Function to fill protocol attributes.
 *
 *  The function is called by the test engine after testing the protocol
 *  attributes. It fills the returned or expected data for global values of
 *  number of agents and protocols implemented.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *  @param received Pointer to platform response message.
 *
 */
void arm_scmi_post_processing_base_protocol_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case,
		struct arm_scmi_message *received)
{
	uint32_t attributes;

	attributes = received->payload[OFFSET_RET(
			struct arm_scmi_base_protocol_attributes, attributes)];
	if (received->status == SCMI_STATUS_SUCCESS) {
		arm_scmi_base_protocol_data.num_agents =
			arm_scmi_get_norm_bits(attributes, BASE_NUMBER_AGENTS_HIGH,
			BASE_NUMBER_AGENTS_LOW);
		arm_scmi_base_protocol_data.num_protocols_implemented =
			arm_scmi_get_norm_bits(attributes, BASE_NUMBER_PROTOCOLS_HIGH,
			BASE_NUMBER_PROTOCOLS_LOW);
	}
}

/*!
 * @brief Function used to discover the list of supported protocols.
 *
 *  The function is called by the test engine just before sending the command
 *  and testing the received data.
 *
 *  @param protocol_execution_context Pointer to the protocol execution test
 *  context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_tester_base_disco_list_protocols(
	struct arm_scmi_protocol_execution_context *protocol_execution_context,
	arm_scmi_test_case_t *test_case)
{
	size_t offset;
	int error_code;
	uint32_t *skip;
	uint8_t index;
	uint8_t check;
	enum test_status res;
	uint32_t protocol_id;
	uint8_t *rcv_protocols;
	uint32_t num_protocols;
	uint32_t total_protocols =
		arm_scmi_base_protocol_data.num_protocols_implemented;

	/* For all test cases allocate space for parameters */
	test_case->parameter_count = 1;
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
			struct arm_scmi_base_discover_list_protocols, skip)) {
		/* Negative test case */
		offset = OFFSET_FROM_INVALID(test_case->invalid_parameter);
		test_case->parameters[offset] = total_protocols + 1;
		error_code = arm_scmi_execute_and_assert_test_case(
				protocol_execution_context, test_case, &received);
	} else if (test_case->parameter_discovery_driven) {
		/* Discover protocol list */
		offset = OFFSET_PARAM(struct arm_scmi_base_discover_list_protocols,
				skip);
		/* Obtain protocol identifiers using spec algorithm */
		skip = &test_case->parameters[offset];
		*skip = 0;
		do {
			error_code = arm_scmi_execute_and_assert_test_case(
					protocol_execution_context, test_case, &received);
			if ((error_code != NO_ERROR ) ||
					(received.status != SCMI_STATUS_SUCCESS))
				break;
			num_protocols = RETURN_VAL(received,
				struct arm_scmi_base_discover_list_protocols, num_protocols);
			for (index = 0; index < num_protocols; ++index)
				*skip += 1;
		} while (*skip < total_protocols);
		if ((error_code != NO_ERROR ) ||
				(received.status != SCMI_STATUS_SUCCESS))
			return;
		/* Now checks the expected values */
		res = (num_protocols == total_protocols);
		if (res == FAILED)
			snprintf(description, NUM_ELEMS(description),
			"Expected: %d, Received: %d", num_protocols, total_protocols);
		else
			snprintf(description, NUM_ELEMS(description), "%d", total_protocols);
		arm_scmi_log_test_result(&protocol_execution_context->test_stats, res,
			"NUMBER OF PROTOCOLS", description);
		check = (LEFT_SHIFT(SUPPORTED_PROTOCOLS) & expected_flags_mask) != 0
				? CHECK: PRINT;
		 rcv_protocols = (uint8_t *)(&received.payload[OFFSET_RET(
				struct arm_scmi_base_discover_list_protocols, protocols)]);
		for (index = 0; index < num_protocols; ++index) {
			res = INFO;
			protocol_id = rcv_protocols[index];
			if (check == CHECK) {
				res = (protocol_id == base_expected->supported_protocols[index])
					? PASSED:FAILED;
			}
			if (res == FAILED) {
				snprintf(description, NUM_ELEMS(description),
					"Expected: %d, Received: %d",
					base_expected->supported_protocols[index], protocol_id);
			} else if ((res == PASSED) || (res == INFO)) {
				snprintf(description, NUM_ELEMS(description), "%d", protocol_id);
			}
			arm_scmi_base_protocol_data.protocols[index] = protocol_id;
			arm_scmi_log_test_result(&protocol_execution_context->test_stats,
				res, "PROTOCOL ID", description);
		}
	}
}

/*!
 * @brief Executor function that runs the test suites/cases
 * for the base protocol.
 *
 *  @details The function iterates through the test suites and test
 *  cases defined for the base protocol (in the header file)
 *  and fills the global variable with information about
 *  protocols and agents discovered from the platform.
 *
 *  @param expected Pointer to the expected return values structure.
 *  @param protocol_report Pointer to a table of protocol pointers.
 *
 */
void arm_scmi_base_execute(const void *expected,
		struct arm_scmi_protocol_report **protocol_report)
{
	static struct arm_scmi_protocol_execution_context protocol_execution_context;

	*protocol_report = &arm_scmi_base_protocol_report;
	setup_base(&protocol_execution_context,
			(struct arm_scmi_base_expected *)expected);
	arm_scmi_run(&protocol_execution_context, &received);
}

