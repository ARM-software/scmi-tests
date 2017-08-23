/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <protocol_common.h>
#include <power_common.h>
#include <power_expected.h>

/* Variable to hold expected data */
static struct arm_scmi_power_expected *power_expected =
		&(struct arm_scmi_power_expected){};
/* Variable to hold the expected flags mask */
static uint32_t expected_flags_mask;

struct power_protocol_data {
	uint16_t num_power_domains;
	uint32_t attributes[MAX_POWER_DOMAIN_COUNT];
};

static struct power_protocol_data power_protocol_data;

/*!
 * @brief Function used to fill up expected test case return data from
 * expected protocol return data.
 *
 *  The function is called by the test engine just before sending
 *  the command in the test case and testing the received data. It also
 *  sets other hooks depending on the message id. To see the expected_returns
 *  index meaning see 'power_tests.c' for each specific command.
 *
 *  @param test case Pointer to the test case being executed in the test engine.
 *
 */
static void power_fill_expected_return_values(arm_scmi_test_case_t *test_case)
{
	return_value_desc_t *expected_ret;

	switch (test_case->message_id)
	{
		case PWR_PROTO_VER_MSG_ID:
			expected_ret = &test_case->expected_returns[0];
			expected_ret->value.integer = power_expected->protocol_version;
			expected_ret->flags = arm_scmi_print_or_check(POWER_PROTOCOL_VERSION,
					expected_flags_mask);
			break;
		case PWR_PROTO_ATTR_MSG_ID:
			expected_ret = &test_case->expected_returns[1];
			expected_ret->value.integer = power_expected->num_power_domains;
			expected_ret->flags = arm_scmi_print_or_check(NUM_POWER_DOMAINS,
					expected_flags_mask);
			break;
		case PWR_PROTO_MSG_ATTR_MSG_ID:
			test_case->expected_returns[0].flags = CHECK;
			break;
		case PWR_DOMAIN_ATTRIB_MSG_ID:
			break;
		case PWR_STATE_SET_MSG_ID:
			break;
		case PWR_STATE_GET_MSG_ID:
			break;
		case PWR_STATE_NOTIFY_MSG_ID:
			break;
	}
}

/*!
 * @brief Setup function for power domain management tests execution.
 *
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param expected Pointer to the expected values for the power protocol.
 *
 */
static void setup_power(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		struct arm_scmi_power_expected *expected)
{
	uint8_t index;
	struct arm_scmi_test_suite_report *tsr =
			arm_scmi_power_protocol_report.test_suite_report_table;

	/* If no expected values are provided (i.e. NULL) it is assigned an
	 * empty compound literal in order to avoid if != NULL in every
	 * base_expected value.
	 */
	if (expected == NULL) {
		expected_flags_mask = 0;
	} else {
		power_expected = expected;
		expected_flags_mask = expected->flags_mask;
	};
	arm_scmi_init_protocol_execution_context(protocol_execution_context,
		power_fill_expected_return_values, &arm_scmi_power_protocol_report);
	arm_scmi_set_test_suite_table(protocol_execution_context,
		arm_scmi_power_test_suites, arm_scmi_power_test_suites_size);
	for (index = 0; index < arm_scmi_power_test_suites_size; ++index)
		arm_scmi_power_test_suites[index].name = (tsr + index)->test_suite_name;
	arm_scmi_skip_test_suite(protocol_execution_context, TS_POWER_DISCOVERY,
			false);
	arm_scmi_log_protocol_name(&protocol_execution_context->test_stats, "Power");
}

/*!
 * @brief Function used to check if a command is supported.
 *
 *  The function is called by the test engine after testing the
 *  PROTOCOL_MESSAGE_ATTRIBUTES message id in order to check if a test suite
 *  must be skipped.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the test case being executed in the test engine.
 *  @param received Pointer to the received message structure.
 *
 */
void arm_scmi_post_processing_power_protocol_message_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case, struct arm_scmi_message *received)
{
	uint32_t mask;
	bool must_be_supported = true;
	bool must_test_suite;
	int index;
	uint32_t message_id = test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_power_protocol_message_attributes, message_id)];

	switch (message_id)
	{
		case PWR_DOMAIN_ATTRIB_MSG_ID:
			mask= ALL_BITS_SET;
			index = TS_POWER_DOMAIN_ATTRIBUTES;
			break;
		case PWR_STATE_SET_MSG_ID:
			mask =  ALL_BITS_SET;
			index = TS_POWER_STATE_SET;
			break;
		case PWR_STATE_GET_MSG_ID:
			mask = ALL_BITS_SET;
			index = TS_POWER_STATE_GET;
			break;
		case PWR_STATE_NOTIFY_MSG_ID:
			mask = LEFT_SHIFT(POWER_STATE_NOTIFY_CMD_SUPPORTED);
			index = TS_POWER_STATE_NOTIFY;
			must_be_supported = power_expected->power_state_notify_cmd_supported;
			break;
	}
	must_test_suite = arm_scmi_check_suite_skipped(
			&protocol_execution_context->test_stats, mask,
			expected_flags_mask, must_be_supported, message_id,
			received->status);
	if (index > 0)
		arm_scmi_skip_test_suite(protocol_execution_context, index,
		!must_test_suite);
}

/*!
 * @brief Function to post-process power protocol attributes.
 *
 *  The function is called by the test engine after testing the power protocol
 *  attribute; statistics support and a number of power domains are stored and
 *  other tests done based in statistics support.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *  @param received Pointer to the received message structure.
 *
 */
void arm_scmi_post_processing_power_protocol_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case,
		struct arm_scmi_message *received)
{
	uint32_t attributes;
	uint32_t statistics_len;
	struct arm_scmi_protocol_test_stats *test_stats =
		&protocol_execution_context->test_stats;

	attributes = received->payload[OFFSET_RET(
			struct arm_scmi_power_protocol_attributes, attributes)];
	if (received->status == SCMI_STATUS_SUCCESS) {
		power_protocol_data.num_power_domains = arm_scmi_get_norm_bits(
			attributes, POWER_PROT_ATTR_NUMBER_DOMAINS_HIGH,
			POWER_PROT_ATTR_NUMBER_DOMAINS_LOW);
		statistics_len = received->payload[OFFSET_RET(
			struct arm_scmi_power_protocol_attributes, statistics_len)];
		if (statistics_len > 0) /*test additional return values*/
		{
			arm_scmi_check_and_report_hex(test_stats, expected_flags_mask,
				LEFT_SHIFT(STATISTICS_ADDRESS_LOW),
				power_expected->statistics_address_low,
				received->payload[OFFSET_RET(
				struct arm_scmi_power_protocol_attributes,
				statistics_address_low)], "STATISTICS_ADDRESS_LOW");
			arm_scmi_check_and_report_hex(test_stats, expected_flags_mask,
				LEFT_SHIFT(STATISTICS_ADDRESS_HIGH),
				power_expected->statistics_address_high,
				received->payload[OFFSET_RET(
				struct arm_scmi_power_protocol_attributes,
				statistics_address_high)], "STATISTICS_ADDRESS_HIGH");
		}
		arm_scmi_check_and_report_dec(test_stats, expected_flags_mask,
			LEFT_SHIFT(STATISTICS_LEN), power_expected->statistics_length,
			received->payload[OFFSET_RET(
			struct arm_scmi_power_protocol_attributes, statistics_len)],
			"STATISTICS_LEN");
	}
}

/*!
 * @brief Function to test the power domain attributes command.
 *
 *  The function is called by the test engine before sending the power domain
 *  attributes command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_test_power_domain_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case)
{
	uint32_t domain_id;
	int error_code;
	uint32_t attributes;
	uint32_t protocol_mask = expected_flags_mask;
	uint32_t rcv_u, exp_u;
	char *domain_name;
	char *rcv_s, *exp_s;
	struct arm_scmi_protocol_execution_context *prot =
			protocol_execution_context;
	enum test_status res;

	/* For all test cases allocate space for parameters */
	test_case->parameter_count = 1;
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
			struct arm_scmi_power_domain_attributes, domain_id) ) {
		/* Negative test case */
		test_case->parameters[
		OFFSET_PARAM(struct arm_scmi_power_domain_attributes, domain_id)] =
			power_protocol_data.num_power_domains + 1;
		error_code = arm_scmi_execute_and_assert_test_case(prot, test_case,
			&received);
	}
	else if (test_case->parameter_discovery_driven) {
		/* Check power domain id data */
		for (domain_id = 0; domain_id < power_protocol_data.num_power_domains;
				++domain_id) {
			test_case->parameters[
			OFFSET_PARAM(struct arm_scmi_power_domain_attributes, domain_id)] =
				domain_id;
			snprintf(description, NUM_ELEMS(description),
				"\n\t\tPOWER DOMAIN ID %d", domain_id);
			arm_scmi_log(&prot->test_stats, description);
			error_code = arm_scmi_execute_and_assert_test_case(prot, test_case,
				&received);
			if (arm_scmi_skip_return_values(error_code, received.status))
				break;
			attributes = RETURN_VAL(received,
				struct arm_scmi_power_domain_attributes, attributes);
			/* Check/print Power state change notifications support */
			res = INFO;
			rcv_u = arm_scmi_get_norm_bits(attributes,
				POWER_DOMAIN_ATTR_STATE_NOTIFICATIONS_HIGH,
				POWER_DOMAIN_ATTR_STATE_NOTIFICATIONS_LOW);
			snprintf(name, NUM_ELEMS(name),
				"POWER STATE CHANGE NOTIFICATIONS SUPPORT");
			if ((power_expected->power_state_notification_support != NULL) &&
			   (protocol_mask & LEFT_SHIFT(POWER_STATE_NOTIFICATION_SUPPORT))) {
				exp_u = power_expected->power_state_notification_support[
						domain_id];
				res = (exp_u == rcv_u) ? PASSED:FAILED;
				snprintf(description, NUM_ELEMS(description),
					"Expected: %d, Received: %d", exp_u, rcv_u);
			}
			if ((res == PASSED) || (res == INFO)) {
				snprintf(description, NUM_ELEMS(description), "%d", rcv_u);
			}
			arm_scmi_log_test_result(&prot->test_stats, res, name, description);
			/* Check/print Power state asynchronous support */
			res = INFO;
			rcv_u = arm_scmi_get_norm_bits(attributes,
				POWER_DOMAIN_ATTR_ASYNC_HIGH, POWER_DOMAIN_ATTR_ASYNC_LOW);
			snprintf(name, NUM_ELEMS(name), "POWER ASYNCHRONOUS SUPPORT");
			if ((power_expected->power_asynchronous_support != NULL) &&
			(protocol_mask & LEFT_SHIFT(POWER_ASYNCHRONOUS_SUPPORT))) {
				exp_u = power_expected->power_asynchronous_support[domain_id];
				res = (exp_u == rcv_u) ? PASSED:FAILED;
				snprintf(description, NUM_ELEMS(description),
					"Expected: %d, Received: %d", exp_u, rcv_u);
			}
			if ((res == PASSED) || (res == INFO)) {
				snprintf(description, NUM_ELEMS(description), "%d", rcv_u);
			}
			arm_scmi_log_test_result(&prot->test_stats, res, name, description);
			/* Check/print Power state synchronous support */
			res = INFO;
			rcv_u = arm_scmi_get_norm_bits(attributes,
				POWER_DOMAIN_ATTR_SYNC_HIGH, POWER_DOMAIN_ATTR_SYNC_LOW);
			snprintf(name, NUM_ELEMS(name), "POWER SYNCHRONOUS SUPPORT");
			if ((power_expected->power_synchronous_support != NULL) &&
				(protocol_mask & LEFT_SHIFT(POWER_SYNCHRONOUS_SUPPORT))) {
				exp_u = power_expected->power_synchronous_support[domain_id];
				res = (exp_u == rcv_u) ? PASSED:FAILED;
				snprintf(description, NUM_ELEMS(description),
					"Expected: %d, Received: %d", exp_u, rcv_u);
			}
			if ((res == PASSED) || (res == INFO)) {
				snprintf(description, NUM_ELEMS(description), "%d", rcv_u);
			}
			arm_scmi_log_test_result(&prot->test_stats, res, name, description);
			/* Check Reserved bits */
			arm_scmi_check_and_report_dec(&prot->test_stats, ALL_BITS_SET,
				CHECK, RESERVED, arm_scmi_get_norm_bits(attributes,
				POWER_DOMAIN_ATTR_RESERVED_HIGH, POWER_DOMAIN_ATTR_RESERVED_LOW),
				"RESERVED");
			/* used in other commands */
			power_protocol_data.attributes[domain_id] = attributes;
			/* Check/print name */
			res = INFO;
			domain_name = RETURN_STR(received,
				struct arm_scmi_power_domain_attributes, name);
			rcv_s = domain_name;
			snprintf(name, NUM_ELEMS(name), "DOMAIN NAME");
			if ((power_expected->power_domain_names != NULL) &&
				(protocol_mask & LEFT_SHIFT(POWER_DOMAIN_NAMES))) {
				exp_s = power_expected->power_domain_names[domain_id];
				res = (strcmp(rcv_s, exp_s) == 0) ? PASSED:FAILED;
				snprintf(description, NUM_ELEMS(description),
				"Expected: %s, Received: %s", exp_s, rcv_s);
			}
			if ((res == PASSED) || (res == INFO)) {
				snprintf(description, NUM_ELEMS(description), "'%s'", rcv_s);
			}
			arm_scmi_log_test_result(&prot->test_stats, INFO, name, description);
		}
	}
}

/*!
 * @brief Function to test the power state set command.
 *
 *  The function is called by the test engine before sending the power state set
 *  command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_test_power_state_set(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case)
{
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
		struct arm_scmi_power_state_set, domain_id) ) {
		/* Negative test cases */
		test_case->parameter_count = 3;
		test_case->parameters[
			OFFSET_PARAM(struct arm_scmi_power_state_set, flags)] = 0x0;
		test_case->parameters[
			OFFSET_PARAM(struct arm_scmi_power_state_set, domain_id)] = 0;
		test_case->parameters[
			OFFSET_PARAM(struct arm_scmi_power_state_set, power_state)] = 0;
		/* Invalid parameter value */
		switch (test_case->invalid_parameter) {
			case OFFSET_INVALID_PARAM(struct arm_scmi_power_state_set,
				domain_id):
				test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_power_state_set, domain_id)] =
					power_protocol_data.num_power_domains;
				break;
		}
		arm_scmi_execute_and_assert_test_case(protocol_execution_context,
			test_case, &received);
	}
}

/*!
 * @brief Function to test the power state get command.
 *
 *  The function is called by the test engine before sending the
 *  power state get command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_test_power_state_get(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case)
{
	uint32_t domain_id;
	int error_code;
	uint32_t power_state;

	/* For all test cases allocate space for parameters */
	test_case->parameter_count = 1;
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
		struct arm_scmi_power_state_get, domain_id) ) {
		/* Negative test case */
		/* Invalid domain id value */
				test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_power_state_get, domain_id)] =
					power_protocol_data.num_power_domains;
		error_code = arm_scmi_execute_and_assert_test_case(
			protocol_execution_context, test_case, &received);
	}
	else if (test_case->parameter_discovery_driven) {
		/* Check power states*/
		for (domain_id = 0; domain_id < power_protocol_data.num_power_domains;
				++domain_id) {
			/* Power domains */
			test_case->parameters[OFFSET_PARAM(struct arm_scmi_power_state_get,
				domain_id)] = domain_id;
			snprintf(description, NUM_ELEMS(description),
				"\n\t\tPOWER DOMAIN ID %d", domain_id);
			arm_scmi_log(&protocol_execution_context->test_stats, description);
			error_code = arm_scmi_execute_and_assert_test_case(
				protocol_execution_context, test_case, &received);
			if (arm_scmi_skip_return_values(error_code, received.status))
				break;
			power_state = RETURN_VAL(received, struct arm_scmi_power_state_get,
				power_state);
			arm_scmi_check_and_report_hex(&protocol_execution_context->test_stats,
				ALL_BITS_SET, PRINT, 0, power_state, "POWER STATE");
		}
	}
}

/*!
 * @brief Function to test the power state notify command.
 *
 *  The function is called by the test engine before sending the power state
 *  notify command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context..
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_test_power_state_notify(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case)
{
	uint32_t domain_id;

	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	if (test_case->invalid_parameter != NO_INVALID_PARAM ) {
		/* Negative test case */
		test_case->parameter_count = 1;
		/* Invalid domain id value */
				test_case->parameters[OFFSET_PARAM(
						struct arm_scmi_power_state_notify, domain_id)] =
							power_protocol_data.num_power_domains;
		arm_scmi_execute_and_assert_test_case(protocol_execution_context,
				test_case, &received);
	}
	else {
		/* Check power state notify */
		test_case->parameter_count = 2;
		/* Power domains */
		for (domain_id = 0; domain_id < power_protocol_data.num_power_domains;
				++domain_id)
		{
			/* Check if power state change notifications are supported */
			if (!arm_scmi_get_norm_bits(power_protocol_data.attributes[domain_id],
					POWER_DOMAIN_ATTR_STATE_NOTIFICATIONS_HIGH,
					POWER_DOMAIN_ATTR_STATE_NOTIFICATIONS_LOW))
							continue;
			test_case->parameters[OFFSET_PARAM(struct arm_scmi_power_state_notify,
					domain_id)] = domain_id;
			snprintf(description, NUM_ELEMS(description),
				"\n\t\tPOWER DOMAIN ID: %d, NOTIFY ENABLE: %d", domain_id,
					test_case->parameters[OFFSET_PARAM(
						struct arm_scmi_power_state_notify, notify_enable)]);
			arm_scmi_log(&protocol_execution_context->test_stats, description);
			arm_scmi_execute_and_assert_test_case(protocol_execution_context,
				test_case, &received);
		}
	}
}

/*!
 * @brief Executor function that runs the test suites/cases for the power
 * management protocol.
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
void arm_scmi_power_execute(const void *expected,
		struct arm_scmi_protocol_report **protocol_report)
{
	static struct arm_scmi_protocol_execution_context protocol_execution_context;

	*protocol_report = &arm_scmi_power_protocol_report;
	setup_power(&protocol_execution_context,
			(struct arm_scmi_power_expected *) expected);
	arm_scmi_run(&protocol_execution_context, &received);
}
