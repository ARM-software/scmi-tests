/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <protocol_common.h>
#include <performance_common.h>
#include <performance_expected.h>

enum {PERF_MIN, PERF_MAX = 3};

/* Variable to hold expected data */
static struct arm_scmi_performance_expected *performance_expected =
		&(struct arm_scmi_performance_expected){};
/* Variable to hold the expected flags mask */
static uint32_t expected_flags_mask;

struct performance_domain {
	uint32_t number_levels;
	uint32_t levels[4];
	uint32_t current_level;
	bool current_level_known;
	uint32_t range_max;
	uint32_t range_min;
	uint32_t attributes;
};

struct performance_protocol_data {
	uint16_t num_performance_domains;
	struct performance_domain domain_runtime_values[MAX_PERFORMANCE_DOMAIN_COUNT];
};

static struct performance_protocol_data performance_protocol_data;

/*!
 * @brief Function used to fill up expected test case return data from
 * expected protocol return data.
 *
 *  The function is called by the test engine just before sending
 *  the command in the test case and testing the received data. It also
 *  sets other hooks depending on the message id. To see the expected_returns
 *  index meaning see 'performance_tests.c' for each specific command.
 *
 *  @param test case Pointer to the test case being executed in the test engine.
 *
 */
static void performance_fill_expected_return_values(
	arm_scmi_test_case_t *test_case)
{
	return_value_desc_t *expected_ret;

	switch (test_case->message_id)
	{
		case PERF_MGMT_PROTO_VER_MSG_ID:
			expected_ret = &test_case->expected_returns[0];
			expected_ret->value.integer = performance_expected->protocol_version;
			expected_ret->flags = arm_scmi_print_or_check(
				PERFORMANCE_PROTOCOL_VERSION, expected_flags_mask);
			break;
		case PERF_MGMT_PROTO_ATTRIB_MSG_ID:
			expected_ret = &test_case->expected_returns[1];
			expected_ret->value.integer =
				performance_expected->power_value_in_milliwatts;
			expected_ret->flags = arm_scmi_print_or_check(
				PERFORMANCE_POWER_VALUE_IN_MW, expected_flags_mask);
			expected_ret = &test_case->expected_returns[2];
			expected_ret->value.integer =
				performance_expected->num_performance_domains;
			expected_ret->flags = arm_scmi_print_or_check(
				PERFORMANCE_NUM_PERFORMANCE_DOMAINS, expected_flags_mask);
			break;
		case PERF_MGMT_PROTO_MSG_ATTRIB_MSG_ID:
			test_case->expected_returns[0].flags = CHECK;
			break;
		case PERF_DOMAIN_ATTRIB_MSG_ID:
		case PERF_DESC_LVL_MSG_ID:
		case PERF_LIMIT_SET_MSG_ID:
		case PERF_LIMIT_GET_MSG_ID:
			break;

	}
}

/*!
 * @brief Setup function for performance domain management tests execution.
 *
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param expected Pointer to the expected values for the performance protocol.
 *
 */
static void setup_performance(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		struct arm_scmi_performance_expected *expected)
{
	uint8_t index;
	struct arm_scmi_test_suite_report *tsr =
			arm_scmi_performance_protocol_report.test_suite_report_table;

	/* If no expected values are provided (i.e. NULL) it is assigned an
	 * empty compound literal in order to avoid if != NULL in every
	 * base_expected value.
	 */
	if (expected == NULL) {
		expected_flags_mask = 0;
	} else {
		performance_expected = expected;
		expected_flags_mask = expected->flags_mask;
	};
	//Initialize runtime data
	performance_protocol_data = (struct performance_protocol_data){};
	arm_scmi_init_protocol_execution_context(protocol_execution_context,
		performance_fill_expected_return_values,
		&arm_scmi_performance_protocol_report);
	arm_scmi_set_test_suite_table(protocol_execution_context,
		arm_scmi_performance_test_suites,
		arm_scmi_performance_test_suites_size);
	for (index = 0; index < arm_scmi_performance_test_suites_size; ++index)
		arm_scmi_performance_test_suites[index].name =
			(tsr + index)->test_suite_name;
	arm_scmi_skip_test_suite(protocol_execution_context,
			TS_PERFORMANCE_DISCOVERY, false);
	arm_scmi_log_protocol_name(&protocol_execution_context->test_stats, "Performance");
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
void arm_scmi_post_processing_performance_protocol_message_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case, struct arm_scmi_message *received)
{
	uint32_t mask;
	bool must_be_supported = true;
	bool must_test_suite;
	int index = -1;
	uint32_t message_id = test_case->parameters[OFFSET_PARAM(
		struct arm_scmi_performance_protocol_message_attributes, message_id)];

	switch (message_id)
	{
		case PERF_DOMAIN_ATTRIB_MSG_ID:
			mask= ALL_BITS_SET;
			index = TS_PERFORMANCE_DOMAIN_ATTRIBUTES;
			break;
		case PERF_DESC_LVL_MSG_ID:
			mask = ALL_BITS_SET;
			index = TS_PERFORMANCE_DESCRIBE_LEVELS;
			break;
		case PERF_LIMIT_SET_MSG_ID:
		case PERF_LIMIT_GET_MSG_ID:
		case PERF_LVL_SET_MSG_ID:
		case PERF_LVL_GET_MSG_ID:
			mask = ALL_BITS_SET;
			break;
		case PERF_NOTIFY_LIMIT_MSG_ID:
			mask = LEFT_SHIFT(PERFORMANCE_LIMIT_NOTIFICATION_SUPPORT);
			must_be_supported =
				performance_expected->performance_limit_notification_support;
			break;
		case PERF_NOTIFY_LVL_MSG_ID:
			mask = LEFT_SHIFT(PERFORMANCE_LEVEL_NOTIFICATION_SUPPORT);
			must_be_supported =
				performance_expected->performance_level_notification_support;
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
 * @brief Function to post-process performance protocol attributes.
 *
 *  The function is called by the test engine after testing the performance
 *  protocol attribute; statistics support and a number of performance domains
 *  are stored and other tests done based in statistics support.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *  @param received Pointer to the received message structure.
 *
 */
void arm_scmi_post_processing_performance_protocol_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case,
		struct arm_scmi_message *received)
{
	uint32_t attributes;
	uint32_t statistics_len;
	struct arm_scmi_protocol_test_stats *test_stats =
		&protocol_execution_context->test_stats;

	attributes = received->payload[OFFSET_RET(
		struct arm_scmi_performance_protocol_attributes, attributes)];
	if (received->status == SCMI_STATUS_SUCCESS) {
		performance_protocol_data.num_performance_domains =
			arm_scmi_get_norm_bits(attributes,
			PERFORMANCE_PROT_ATTR_NUMBER_DOMAINS_HIGH,
			PERFORMANCE_PROT_ATTR_NUMBER_DOMAINS_LOW);
		statistics_len = received->payload[OFFSET_RET(
			struct arm_scmi_performance_protocol_attributes, statistics_len)];
		arm_scmi_check_and_report_dec(test_stats, expected_flags_mask,
			LEFT_SHIFT(PERFORMANCE_STATISTICS_LENGTH),
			performance_expected->statistics_length, statistics_len,
			"STATISTICS_ADDRESS_LEN");
		if (statistics_len > 0) {
			arm_scmi_check_and_report_hex(test_stats, expected_flags_mask,
				LEFT_SHIFT(PERFORMANCE_STATISTICS_ADDRESS_LOW),
				performance_expected->statistics_address_low,
				received->payload[OFFSET_RET(
				struct arm_scmi_performance_protocol_attributes,
				statistics_address_low)], "STATISTICS_ADDRESS_LOW");
			arm_scmi_check_and_report_hex(test_stats, expected_flags_mask,
				LEFT_SHIFT(PERFORMANCE_STATISTICS_ADDRESS_HIGH),
				performance_expected->statistics_address_high,
				received->payload[OFFSET_RET(
				struct arm_scmi_performance_protocol_attributes,
				statistics_address_high)], "STATISTICS_ADDRESS_HIGH");
		}
	}
}

/*!
 * @brief Function to test the performance domain attributes command.
 *
 *  The function is called by the test engine before sending the performance
 *  domain attributes command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_test_performance_domain_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case)
{
	uint32_t domain_id;
	int error_code;
	uint32_t attributes;
	uint32_t protocol_mask = expected_flags_mask;
	uint32_t rcv_u, exp_u;
	uint32_t rate_limit;
	char *domain_name;
	bool domain_expected;
	char *rcv_s, *exp_s;
	struct arm_scmi_protocol_execution_context *prot =
			protocol_execution_context;
	struct arm_scmi_performance_expected *pe = performance_expected;
	enum test_status res;

	/* For all test cases allocate space for parameters */
	test_case->parameter_count = 1;
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
		struct arm_scmi_performance_domain_attributes, domain_id)) {
		/* Negative test case */
		test_case->parameters[
		OFFSET_PARAM(struct arm_scmi_performance_domain_attributes,domain_id)] =
			performance_protocol_data.num_performance_domains + 1;
		error_code = arm_scmi_execute_and_assert_test_case(prot, test_case,
			&received);
	}
	else if (test_case->parameter_discovery_driven) {
		/* Check performance domain id data */
		for (domain_id = 0;
			domain_id < performance_protocol_data.num_performance_domains;
			++domain_id) {
			test_case->parameters[
			OFFSET_PARAM(struct arm_scmi_performance_domain_attributes,
				domain_id)] = domain_id;
			snprintf(description, NUM_ELEMS(description),
				"\n\t\tPERFORMANCE DOMAIN ID %d", domain_id);
			arm_scmi_log(&prot->test_stats, description);
			error_code = arm_scmi_execute_and_assert_test_case(prot, test_case,
				&received);
			if (arm_scmi_skip_return_values(error_code, received.status))
				continue;
			attributes = RETURN_VAL(received,
				struct arm_scmi_performance_domain_attributes, attributes);
			domain_expected = ((pe != NULL) &&
				(domain_id < pe->num_performance_domains));
			/* Check/print set limits */
			res = INFO;
			rcv_u = arm_scmi_get_norm_bits(attributes,
				PERFORMANCE_DOMAIN_ATTR_LIMIT_SET,
				PERFORMANCE_DOMAIN_ATTR_LIMIT_SET);
			snprintf(name, NUM_ELEMS(name), "PERFORMANCE SET LIMITS");
			if ((pe->set_limit_capable != NULL) &&
				(protocol_mask & LEFT_SHIFT(PERFORMANCE_SET_LIMIT_CAPABLE))){
				exp_u = pe->set_limit_capable[domain_id];
				res = (exp_u == rcv_u) ? PASSED:FAILED;
				snprintf(description, NUM_ELEMS(description),
					"Expected: %d, Received: %d", exp_u, rcv_u);
			}
			if ((res == PASSED) || (res == INFO)) {
				snprintf(description, NUM_ELEMS(description), "%d", rcv_u);
			}
			arm_scmi_log_test_result(&prot->test_stats, res, name, description);
			/* Check/print set performance level */
			res = INFO;
			rcv_u = arm_scmi_get_norm_bits(attributes,
				PERFORMANCE_DOMAIN_ATTR_LEVEL_SET,
				PERFORMANCE_DOMAIN_ATTR_LEVEL_SET);
			snprintf(name, NUM_ELEMS(name), "PERFORMANCE SET LEVEL");
			if ((pe->set_performance_level_capable != NULL) &&
				(protocol_mask & LEFT_SHIFT(
				PERFORMANCE_SET_PERFORMANCE_LEVEL_CAPABLE)) && domain_expected){
				exp_u = pe->set_performance_level_capable[domain_id];
				res = (exp_u == rcv_u) ? PASSED:FAILED;
				snprintf(description, NUM_ELEMS(description),
					"Expected: %d, Received: %d", exp_u, rcv_u);
			}
			if ((res == PASSED) || (res == INFO)) {
				snprintf(description, NUM_ELEMS(description), "%d", rcv_u);
			}
			arm_scmi_log_test_result(&prot->test_stats, res, name, description);
			/* Check/print set limit change notification */
			res = INFO;
			rcv_u = arm_scmi_get_norm_bits(attributes,
					PERFORMANCE_DOMAIN_ATTR_LIMIT_NOTIFY,
					PERFORMANCE_DOMAIN_ATTR_LIMIT_NOTIFY);
			snprintf(name, NUM_ELEMS(name), "PERFORMANCE LIMITS CHANGE NOTIFICATION");
			if ((pe->performance_limit_notification_support != NULL) &&
				(protocol_mask &
				LEFT_SHIFT(PERFORMANCE_LIMIT_NOTIFICATION_SUPPORT)) &&
				domain_expected) {
				exp_u = pe->performance_limit_notification_support[domain_id];
				res = (exp_u == rcv_u) ? PASSED:FAILED;
				snprintf(description, NUM_ELEMS(description),
					"Expected: %d, Received: %d", exp_u, rcv_u);
			}
			if ((res == PASSED) || (res == INFO)) {
				snprintf(description, NUM_ELEMS(description), "%d", rcv_u);
			}
			arm_scmi_log_test_result(&prot->test_stats, res, name, description);
			/* Check/print set level change notification */
			res = INFO;
			rcv_u = arm_scmi_get_norm_bits(attributes,
					PERFORMANCE_DOMAIN_ATTR_LEVEL_NOTIFY,
					PERFORMANCE_DOMAIN_ATTR_LEVEL_NOTIFY);
			snprintf(name, NUM_ELEMS(name), "PERFORMANCE LEVEL CHANGE NOTIFICATION");
			if ((pe->performance_level_notification_support != NULL) &&
				(protocol_mask &
				LEFT_SHIFT(PERFORMANCE_LEVEL_NOTIFICATION_SUPPORT)) &&
				domain_expected) {
				exp_u = pe->performance_level_notification_support[domain_id];
				res = (exp_u == rcv_u) ? PASSED:FAILED;
				snprintf(description, NUM_ELEMS(description),
					"Expected: %d, Received: %d", exp_u, rcv_u);
			}
			if ((res == PASSED) || (res == INFO)) {
				snprintf(description, NUM_ELEMS(description), "%d", rcv_u);
			}
			arm_scmi_log_test_result(&prot->test_stats, res, name, description);
			/* Check reserved bits */
			arm_scmi_check_and_report_dec(&prot->test_stats, ALL_BITS_SET,
				CHECK, RESERVED, arm_scmi_get_norm_bits(attributes,
				PERFORMANCE_DOMAIN_ATTR_RESERVED_HIGH,
				PERFORMANCE_DOMAIN_ATTR_RESERVED_LOW), "RESERVED");
			/* used in other commands */
			performance_protocol_data.domain_runtime_values[
			domain_id].attributes = attributes;
			/* Check/print rate limit */
			rate_limit = RETURN_VAL(received,
				struct arm_scmi_performance_domain_attributes, rate_limit);
			/* Check Reserved bits in rate limit */
			rcv_u = arm_scmi_get_norm_bits(rate_limit,
				PERFORMANCE_DOMAIN_ATTR_RATE_RESERVED_HIGH,
				PERFORMANCE_DOMAIN_ATTR_RATE_RESERVED_LOW);
			arm_scmi_check_and_report_dec(&prot->test_stats, ALL_BITS_SET, CHECK,
				RESERVED, rcv_u, "RESERVED/RATE LIMIT");
			/* Check rate limit bits in rate limit */
			res = INFO;
			rcv_u = arm_scmi_get_norm_bits(rate_limit,
				PERFORMANCE_DOMAIN_ATTR_RATE_LIMIT_HIGH,
				PERFORMANCE_DOMAIN_ATTR_RATE_LIMIT_LOW);
			snprintf(name, NUM_ELEMS(name), "RATE LIMIT(microseconds)");
			if ((pe->performance_rate_limit != NULL) &&
				(protocol_mask & LEFT_SHIFT(PERFORMANCE_RATE_LIMIT)) &&
				domain_expected) {
				exp_u = pe->performance_rate_limit[domain_id];
				res = (exp_u == rcv_u) ? PASSED:FAILED;
				snprintf(description, NUM_ELEMS(description),
					"Expected: %d, Received: %d", exp_u, rcv_u);
			}
			if ((res == PASSED) || (res == INFO)) {
				snprintf(description, NUM_ELEMS(description), "%d", rcv_u);
			}
			arm_scmi_log_test_result(&prot->test_stats, res, name, description);
			/* Check/print sustained frequency */
			res = INFO;
			rcv_u = RETURN_VAL(received,
				struct arm_scmi_performance_domain_attributes, sustained_freq);
			snprintf(name, NUM_ELEMS(name), "SUSTAINED FREQUENCY(kHz)");
			if ((pe->performance_sustained_freq != NULL) &&
				(protocol_mask & LEFT_SHIFT(PERFORMANCE_SUSTAINED_FREQ)) &&
				domain_expected) {
				exp_u = pe->performance_sustained_freq[domain_id];
				res = (exp_u == rcv_u) ? PASSED:FAILED;
				snprintf(description, NUM_ELEMS(description),
					"Expected: %d, Received: %d", exp_u, rcv_u);
			}
			if ((res == PASSED) || (res == INFO)) {
				snprintf(description, NUM_ELEMS(description), "%d", rcv_u);
			}
			arm_scmi_log_test_result(&prot->test_stats, res, name, description);
			/* Check/print sustained perf level */
			res = INFO;
			rcv_u = RETURN_VAL(received,
				struct arm_scmi_performance_domain_attributes,
					sustained_perf_level);
			snprintf(name, NUM_ELEMS(name), "SUSTAINED LEVEL");
			if ((pe->performance_sustained_level != NULL) &&
				(protocol_mask & LEFT_SHIFT(PERFORMANCE_SUSTAINED_LEVEL)) &&
				domain_expected) {
				exp_u = pe->performance_sustained_level[domain_id];
				res = (exp_u == rcv_u) ? PASSED:FAILED;
				snprintf(description, NUM_ELEMS(description),
					"Expected: %d, Received: %d", exp_u, rcv_u);
			}
			if ((res == PASSED) || (res == INFO)) {
				snprintf(description, NUM_ELEMS(description), "%d", rcv_u);
			}
			arm_scmi_log_test_result(&prot->test_stats, res, name, description);
			/* Check/print name */
			res = INFO;
			domain_name = RETURN_STR(received,
				struct arm_scmi_performance_domain_attributes, name);
			rcv_s = domain_name;
			snprintf(name, NUM_ELEMS(name), "DOMAIN NAME");
			if ((pe->performance_domain_names != NULL) &&
				(protocol_mask & LEFT_SHIFT(PERFORMANCE_DOMAIN_NAMES)) &&
				domain_expected) {
				exp_s = pe->performance_domain_names[domain_id];
				res = (strcmp(rcv_s,exp_s) == 0) ? PASSED:FAILED;
				snprintf(description, NUM_ELEMS(description),
					"Expected: %s, Received: %s", exp_s, rcv_s);
			}
			if ((res == INFO) || (res == PASSED)){
				snprintf(description, NUM_ELEMS(description), "'%s'", rcv_s);
			}
			arm_scmi_log_test_result(&prot->test_stats, res, name, description);
		}
	}
}

/*!
 * @brief Function to test the performance describe levels command.
 *
 *  The function is called by the test engine before sending the
 *  performance describe levels command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_test_performance_describe_levels(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case)
{
	uint32_t domain_id;
	int error_code;
	enum test_status res;
	uint32_t num_levels;
	uint32_t num_perf_levels, num_rem_perf_levels;
	uint32_t level_index = 0, n;
	uint32_t rcv_u, exp_u;
	struct arm_scmi_perf_levels *perf_levels;
	uint32_t rcv_num_levels;
	struct arm_scmi_protocol_test_stats *test_stats =
		&protocol_execution_context->test_stats;
	struct arm_scmi_performance_expected *pe = performance_expected;
	bool domain_expected;
	bool level_expected;
	uint32_t *levels;
	struct performance_protocol_data *ptr_data = &performance_protocol_data;

	/* For all test cases allocate space for parameters */
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
		struct arm_scmi_performance_describe_levels, domain_id)) {
		/* Negative test cases */
		test_case->parameter_count = 2;
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_performance_describe_levels, domain_id)] =
			ptr_data->num_performance_domains + 1;
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_performance_describe_levels, level_index)] = 0;
		error_code = arm_scmi_execute_and_assert_test_case(
			protocol_execution_context, test_case, &received);
	} else {
		test_case->parameter_count = 2;
		/* Discovering performance levels */
		for (domain_id = 0;
			domain_id < ptr_data->num_performance_domains; ++domain_id) {
			/* For every performance domain id */
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_performance_describe_levels, domain_id)] =
				domain_id;
			rcv_num_levels = 0;
			level_index = 0;
			/* find their levels */
			do {
				test_case->parameters[OFFSET_PARAM(
					struct arm_scmi_performance_describe_levels, level_index)] =
					level_index;
				snprintf(description, NUM_ELEMS(description),
					"\n\t\tDOMAIN ID %d", domain_id);
				arm_scmi_log(&protocol_execution_context->test_stats,
					description);
				error_code = arm_scmi_execute_and_assert_test_case(
					protocol_execution_context, test_case, &received);
				if ((error_code != NO_ERROR) ||
					(received.status != SCMI_STATUS_SUCCESS))
					break;
				domain_expected = ((pe != NULL) &&
					(domain_id < pe->num_performance_domains));
				/* Get num_levels */
				num_levels = RETURN_VAL(received,
					struct arm_scmi_performance_describe_levels, num_levels);
				num_rem_perf_levels = arm_scmi_get_norm_bits(num_levels,
					PERFORMANCE_DESC_LEVELS_NUM_LEVELS_REMAINING_HIGH,
					PERFORMANCE_DESC_LEVELS_NUM_LEVELS_REMAINING_LOW);
				num_perf_levels = arm_scmi_get_norm_bits(num_levels,
					PERFORMANCE_DESC_LEVELS_NUM_LEVELS_HIGH,
					PERFORMANCE_DESC_LEVELS_NUM_LEVELS_LOW);
				/* Print number of remaining performance levels */
				arm_scmi_check_and_report_dec(test_stats, ALL_BITS_SET, PRINT, 0,
				num_rem_perf_levels, "NUMBER OF REMAINING PERFORMANCE LEVELS");
				/* Check Reserved bits */
				rcv_u = arm_scmi_get_norm_bits(num_levels,
					PERFORMANCE_DESC_LEVELS_ATTR_RESERVED_HIGH,
					PERFORMANCE_DESC_LEVELS_ATTR_RESERVED_LOW);
				arm_scmi_check_and_report_dec(test_stats, ALL_BITS_SET, CHECK,
					RESERVED, rcv_u, "RESERVED");
				/* Check/print number of performance levels */
				rcv_u = num_perf_levels;
				rcv_num_levels += num_perf_levels;
				ptr_data->domain_runtime_values[domain_id].number_levels =
				rcv_num_levels;
				res = INFO;
				snprintf(name, NUM_ELEMS(name), "NUMBER OF PERFORMANCE LEVELS");
				if ((expected_flags_mask &
						LEFT_SHIFT(PERFORMANCE_NUM_PERFORMANCE_LEVELS)) &&
						(num_rem_perf_levels == 0) && domain_expected) {
					exp_u = pe->num_performance_levels[domain_id];
					res = rcv_u == exp_u ? PASSED:FAILED;
					snprintf(description, NUM_ELEMS(description),
						"Expected: %d, Received: %d", exp_u, rcv_u);
				}
				if ((res == PASSED) || (res == INFO)) {
					snprintf(description, NUM_ELEMS(description), "%d", rcv_u);
				}
				arm_scmi_log_test_result(test_stats, res, name, description);
				/* For every level find their values */
				perf_levels = (struct arm_scmi_perf_levels *)
					(&received.payload[OFFSET_RET(
					struct arm_scmi_performance_describe_levels, perf_levels)]);
				levels = ptr_data->domain_runtime_values[domain_id].levels;
				for (n = 0; n < num_perf_levels; ++n) {
					snprintf(description, NUM_ELEMS(description),
						"\n\t\tDOMAIN ID %d, LEVEL %d", domain_id, n);
					arm_scmi_log(test_stats, description);
					level_expected = (domain_expected &&
					(pe->num_performance_levels != NULL) &&
					((level_index + n) < pe->num_performance_levels[domain_id]));
					rcv_u = perf_levels[n].performance_level_value;
					/* Store performance describe levels */
					if ((level_index + n) == 0) {
						levels[PERF_MIN] = levels[PERF_MIN + 1] =
						levels[PERF_MAX - 1] = levels[PERF_MAX] = rcv_u;
					} else if ((level_index + n) == 1) {
						levels[PERF_MIN + 1] = levels[PERF_MAX] = rcv_u;
					}
					levels[PERF_MAX - 1] = levels[PERF_MAX];
					levels[PERF_MAX] = rcv_u;
					/* Check/print Performance level value */
					res = INFO;
					snprintf(name, NUM_ELEMS(name), "PERFORMANCE LEVEL VALUE");
					if ((pe->performance_level_value != NULL) &&
						(expected_flags_mask &
						LEFT_SHIFT(PERFORMANCE_LEVEL_VALUE)) && level_expected)
					{
						exp_u =
						pe->performance_level_value[domain_id][level_index + n];
						res = rcv_u == exp_u ? PASSED:FAILED;
						snprintf(description, NUM_ELEMS(description),
							"Expected: %d, Received: %d", exp_u, rcv_u);
					}
					if ((res == PASSED) || (res == INFO)) {
						snprintf(description, NUM_ELEMS(description), "%d",
						rcv_u);
					}
					arm_scmi_log_test_result(test_stats, res, name, description);
					/* Check/print Power cost */
					res = INFO;
					snprintf(name, NUM_ELEMS(name), "PERFORMANCE LEVEL POWER COST");
					rcv_u = perf_levels[n].power_cost;
					if ((pe->performance_level_power_cost != NULL) &&
						(expected_flags_mask &
						LEFT_SHIFT(PERFORMANCE_LEVEL_POWER_COST)) &&
						level_expected) {
						exp_u = pe->performance_level_power_cost[
						domain_id][level_index + n];
						res = rcv_u == exp_u ? PASSED:FAILED;
						snprintf(description, NUM_ELEMS(description),
							"Expected: %d, Received: %d", exp_u, rcv_u);
					}
					if ((res == PASSED) || (res == INFO)) {
						snprintf(description, NUM_ELEMS(description), "%d", rcv_u);
					}
					arm_scmi_log_test_result(test_stats, res, name, description);
					/* Check Reserved bits */
					rcv_u = arm_scmi_get_norm_bits(perf_levels[n].attributtes,
							PERFORMANCE_DESC_LEVELS_NUM_LEVELS_RESERVED_HIGH,
							PERFORMANCE_DESC_LEVELS_NUM_LEVELS_RESERVED_LOW);
					arm_scmi_check_and_report_dec(test_stats, ALL_BITS_SET, CHECK,
						RESERVED, rcv_u, "LEVEL RESERVED");
					/* Check/print worst latency from attributes */
					res = INFO;
					snprintf(name, NUM_ELEMS(name), "WORST-CASE LATENCY(microseconds)");
					rcv_u = arm_scmi_get_norm_bits(perf_levels[n].attributtes,
							PERFORMANCE_DESC_LEVELS_ATTR_TRANSITION_HIGH,
							PERFORMANCE_DESC_LEVELS_ATTR_TRANSITION_LOW);
					if ((pe->performance_level_worst_latency != NULL) &&
						(expected_flags_mask &
						LEFT_SHIFT(PERFORMANCE_LEVEL_WORST_LATENCY)) &&
						level_expected) {
						exp_u = pe->performance_level_worst_latency[
							domain_id][level_index + n];
						res = rcv_u == exp_u ? PASSED:FAILED;
						snprintf(description, NUM_ELEMS(description),
							"Expected: %d, Received: %d", exp_u, rcv_u);
					}
					if ((res == PASSED) || (res == INFO)) {
						snprintf(description, NUM_ELEMS(description), "%d", rcv_u);
					}
					arm_scmi_log_test_result(test_stats, res, name, description);
				}
				level_index += num_perf_levels;
			} while (num_rem_perf_levels);
		}
	}
}

/*!
 * @brief Function to test the performance limits set command.
 *
 *  The function is called by the test engine before sending the performance
 *  limits set command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_test_performance_limits_set(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case)
{
	uint32_t domain_id;
	struct performance_protocol_data *ptr_data = &performance_protocol_data;

	/* For all test cases allocate space for parameters */
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	test_case->parameter_count = 3;

	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
			struct arm_scmi_performance_limits_set, domain_id)) {
		/* Negative test case */
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_performance_limits_set, domain_id)] =
			ptr_data->num_performance_domains + 1;
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_performance_limits_set, range_max)] =
			ptr_data->domain_runtime_values[0].levels[PERF_MAX - 1];
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_performance_limits_set, range_min)] =
			ptr_data->domain_runtime_values[0].levels[PERF_MIN + 1];
		arm_scmi_execute_and_assert_test_case(protocol_execution_context,
			test_case, &received);
	} else if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
			struct arm_scmi_performance_limits_set, range_max)) {
		/* Negative test case */
		/* For every domain id */
		for (domain_id = 0; domain_id < ptr_data->num_performance_domains;
			++domain_id) {
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_performance_limits_set, domain_id)] =
				domain_id;
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_performance_limits_set, range_max)] =
				ptr_data->domain_runtime_values[domain_id].levels[PERF_MAX] + 1;
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_performance_limits_set, range_min)] =
				ptr_data->domain_runtime_values[domain_id].levels[PERF_MIN];
			test_case->status = arm_scmi_get_norm_bits(
					ptr_data->domain_runtime_values[domain_id].attributes,
					PERFORMANCE_DOMAIN_ATTR_LIMIT_SET,
					PERFORMANCE_DOMAIN_ATTR_LIMIT_SET) == 1 ?
					SCMI_STATUS_OUT_OF_RANGE: SCMI_STATUS_DENIED;
			snprintf(description, NUM_ELEMS(description), "\n\t\tDOMAIN ID %d",
				domain_id);
			arm_scmi_log(&protocol_execution_context->test_stats, description);
			arm_scmi_execute_and_assert_test_case(protocol_execution_context,
				test_case, &received);
		}
	} else if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
			struct arm_scmi_performance_limits_set, range_min)) {
		/* Negative test case */
		/* For every domain id */
		for (domain_id = 0;
			domain_id < ptr_data->num_performance_domains; ++domain_id) {
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_performance_limits_set, domain_id)] =
				domain_id;
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_performance_limits_set, range_max)] =
				ptr_data->domain_runtime_values[domain_id].levels[PERF_MAX];
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_performance_limits_set, range_min)] =
				ptr_data->domain_runtime_values[domain_id].levels[PERF_MIN] - 1;
			test_case->status = arm_scmi_get_norm_bits(
					ptr_data->domain_runtime_values[domain_id].attributes,
					PERFORMANCE_DOMAIN_ATTR_LIMIT_SET,
					PERFORMANCE_DOMAIN_ATTR_LIMIT_SET) == 1 ?
					SCMI_STATUS_OUT_OF_RANGE: SCMI_STATUS_DENIED;
			snprintf(description, NUM_ELEMS(description), "\n\t\tDOMAIN ID %d",
				domain_id);
			arm_scmi_log(&protocol_execution_context->test_stats, description);
			arm_scmi_execute_and_assert_test_case(
				protocol_execution_context, test_case, &received);
		}
	} else {
		/* Checking valid performance level value set limits */
		/* For every domain id */
		for (domain_id = 0;
			domain_id < ptr_data->num_performance_domains; ++domain_id) {
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_performance_limits_set, domain_id)] = domain_id;
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_performance_limits_set, range_max)] =
				ptr_data->domain_runtime_values[domain_id].levels[PERF_MAX - 1];
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_performance_limits_set, range_min)] =
				ptr_data->domain_runtime_values[domain_id].levels[PERF_MIN + 1];
			test_case->status = arm_scmi_get_norm_bits(
					ptr_data->domain_runtime_values[domain_id].attributes,
					PERFORMANCE_DOMAIN_ATTR_LIMIT_SET,
					PERFORMANCE_DOMAIN_ATTR_LIMIT_SET) == 1 ?
					SCMI_STATUS_SUCCESS: SCMI_STATUS_DENIED;
			snprintf(description, NUM_ELEMS(description), "\n\t\tDOMAIN ID %d",
				domain_id);
			arm_scmi_log(&protocol_execution_context->test_stats, description);
			arm_scmi_execute_and_assert_test_case(protocol_execution_context,
				test_case, &received);
			/* Update limits */
			if (received.status == SCMI_STATUS_SUCCESS) {
				ptr_data->domain_runtime_values[domain_id].range_min =
					test_case->parameters[OFFSET_PARAM(
						struct arm_scmi_performance_limits_set, range_min)];
				ptr_data->domain_runtime_values[domain_id].range_max =
					test_case->parameters[OFFSET_PARAM(
						struct arm_scmi_performance_limits_set, range_max)];
			}
		}
	}
}

/*!
 * @brief Function to test the performance limits get command.
 *
 *  The function is called by the test engine before sending the performance
 *  limits get command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_test_performance_limits_get(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case)
{
	uint32_t domain_id;
	int error_code;
	uint32_t rcv_u;
	struct performance_protocol_data *ptr_data = &performance_protocol_data;
	int check_or_print;

	/* For all test cases allocate space for parameters */
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	test_case->parameter_count = 1;
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
			struct arm_scmi_performance_limits_get, domain_id)) {
		/* Negative test case */
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_performance_limits_get, domain_id)] =
			ptr_data->num_performance_domains + 1;
		arm_scmi_execute_and_assert_test_case(
			protocol_execution_context, test_case, &received);
	} else {
		/* Check if it is a precondition test */
		if ((test_case->precondition_indicator == PRECOND_USE) &&
		((protocol_execution_context->precondition_data.result != CONFORMANT) ||
		(protocol_execution_context->precondition_data.status !=
			SCMI_STATUS_SUCCESS))) {
			arm_scmi_log_test_result(&protocol_execution_context->test_stats,
				SKIP, "PRECONDITION STATUS", "PRECONDITION NON-CONFORMANT");
			return;
		}
		/* Check precondition status */
		check_or_print = (test_case->precondition_indicator == PRECOND_USE) ?
			CHECK:PRINT;
		/* Checking valid performance level value get limits */
		/* For every domain id */
		for (domain_id = 0; domain_id < ptr_data->num_performance_domains;
			++domain_id) {
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_performance_limits_get, domain_id)] = domain_id;
			snprintf(description, NUM_ELEMS(description), "\n\t\tDOMAIN ID %d",
				domain_id);
			arm_scmi_log(&protocol_execution_context->test_stats, description);
			error_code = arm_scmi_execute_and_assert_test_case(
				protocol_execution_context, test_case, &received);
			if (arm_scmi_skip_return_values(error_code, received.status))
				continue;
			/* Check/Print range_max */
			rcv_u = RETURN_VAL(received, struct arm_scmi_performance_limits_get,
				range_max);
			arm_scmi_check_and_report_dec(
				&protocol_execution_context->test_stats,
				ALL_BITS_SET, check_or_print, ptr_data->domain_runtime_values[
				domain_id].range_max, rcv_u, "RANGE MAX");
			/* Check/Print range_min */
			rcv_u = RETURN_VAL(received, struct arm_scmi_performance_limits_get,
				range_min);
			arm_scmi_check_and_report_dec(
				&protocol_execution_context->test_stats,
				ALL_BITS_SET, check_or_print, ptr_data->domain_runtime_values[
				domain_id].range_min, rcv_u, "RANGE MIN");
		}
	}
}

/*!
 * @brief Function to test the performance levels set command.
 *
 *  The function is called by the test engine before sending the performance
 *  levels set command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_test_performance_levels_set(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case)
{
	uint32_t domain_id;
	struct performance_protocol_data *ptr_data = &performance_protocol_data;

	/* For all test cases allocate space for parameters */
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	test_case->parameter_count = 2;

	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
			struct arm_scmi_performance_levels_set, domain_id)) {
		/* Negative test case */
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_performance_levels_set, domain_id)] =
				ptr_data->num_performance_domains + 1;
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_performance_levels_set, performance_level)] =
				ptr_data->domain_runtime_values[0].range_max;
		arm_scmi_execute_and_assert_test_case(protocol_execution_context,
			test_case, &received);
	} else if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
			struct arm_scmi_performance_levels_set, performance_level)) {
		/* Negative test case */
		/* For every domain id */
		for (domain_id = 0;
			domain_id < ptr_data->num_performance_domains;
			++domain_id) {
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_performance_levels_set, domain_id)] =
				domain_id;
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_performance_levels_set, performance_level)] =
						ptr_data->domain_runtime_values[domain_id].range_max + 1;
			test_case->status = arm_scmi_get_norm_bits(
					ptr_data->domain_runtime_values[domain_id].attributes,
					PERFORMANCE_DOMAIN_ATTR_LEVEL_SET,
					PERFORMANCE_DOMAIN_ATTR_LEVEL_SET) == 1 ?
					SCMI_STATUS_OUT_OF_RANGE: SCMI_STATUS_DENIED;
			snprintf(description, NUM_ELEMS(description), "\n\t\tDOMAIN ID %d",
				domain_id);
			arm_scmi_log(&protocol_execution_context->test_stats, description);
			arm_scmi_execute_and_assert_test_case(protocol_execution_context,
				test_case, &received);
		}
	} else {
		/* Checking valid performance level */
		/* For every domain id */
		for (domain_id = 0;
			domain_id < ptr_data->num_performance_domains; ++domain_id) {
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_performance_levels_set, domain_id)] = domain_id;
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_performance_levels_set, performance_level)] =
					ptr_data->domain_runtime_values[domain_id].range_max;
			test_case->status = arm_scmi_get_norm_bits(
					ptr_data->domain_runtime_values[domain_id].attributes,
					PERFORMANCE_DOMAIN_ATTR_LEVEL_SET,
					PERFORMANCE_DOMAIN_ATTR_LEVEL_SET) == 1 ?
					SCMI_STATUS_SUCCESS: SCMI_STATUS_DENIED;
			snprintf(description, NUM_ELEMS(description), "\n\t\tDOMAIN ID %d",
				domain_id);
			arm_scmi_log(&protocol_execution_context->test_stats, description);
			arm_scmi_execute_and_assert_test_case(protocol_execution_context,
				test_case, &received);
			/* Update current level */
			if (received.status == SCMI_STATUS_SUCCESS) {
				ptr_data->domain_runtime_values[domain_id].current_level =
					test_case->parameters[OFFSET_PARAM(
					struct arm_scmi_performance_levels_set, performance_level)];
				ptr_data->domain_runtime_values[domain_id].current_level_known =
				true;
			}
		}
	}
}

/*!
 * @brief Function to test the performance levels get command.
 *
 *  The function is called by the test engine before sending the performance
 *  levels get command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_test_performance_levels_get(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case)
{
	uint32_t domain_id;
	int error_code;
	uint32_t rcv_u;
	struct performance_protocol_data *ptr_data = &performance_protocol_data;
	struct arm_scmi_protocol_test_stats *test_stats =
		&protocol_execution_context->test_stats;
	int check_or_print;

	/* For all test cases allocate space for parameters */
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	test_case->parameter_count = 1;
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
			struct arm_scmi_performance_levels_get, domain_id)) {
		/* Negative test case */
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_performance_levels_get, domain_id)] =
			ptr_data->num_performance_domains + 1;
		arm_scmi_execute_and_assert_test_case(
			protocol_execution_context, test_case, &received);
	} else {
		/* Check if it is a precondition test */
		if ((test_case->precondition_indicator == PRECOND_USE) &&
		((protocol_execution_context->precondition_data.result != CONFORMANT) ||
		(protocol_execution_context->precondition_data.status !=
			SCMI_STATUS_SUCCESS))) {
			arm_scmi_log_test_result(test_stats, SKIP, "PRECONDITION STATUS",
				"PRECONDITION NON-CONFORMANT");
			return;
		}
		/* Check precondition status */
		check_or_print = (test_case->precondition_indicator == PRECOND_USE) ?
			CHECK:PRINT;
		/* Checking valid performance level value */
		/* For every domain id */
		for (domain_id = 0; domain_id < ptr_data->num_performance_domains;
			++domain_id) {
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_performance_levels_get, domain_id)] = domain_id;
			snprintf(description, NUM_ELEMS(description), "\n\t\tDOMAIN ID %d",
				domain_id);
			arm_scmi_log(&protocol_execution_context->test_stats, description);
			error_code = arm_scmi_execute_and_assert_test_case(
				protocol_execution_context, test_case, &received);
			if (arm_scmi_skip_return_values(error_code, received.status))
				continue;
			/* Check/print performance_level */
			rcv_u = RETURN_VAL(received, struct arm_scmi_performance_levels_get,
				performance_level);
			arm_scmi_check_and_report_dec(test_stats, ALL_BITS_SET,
				check_or_print,
				ptr_data->domain_runtime_values[domain_id].current_level,
				rcv_u,
				"PERFORMANCE LEVEL");
		}
	}
}

/*!
 * @brief Executor function that runs the test suites/cases for the performance
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
void arm_scmi_performance_execute(const void *expected,
		struct arm_scmi_protocol_report **protocol_report)
{
	static struct arm_scmi_protocol_execution_context protocol_execution_context;

	*protocol_report = &arm_scmi_performance_protocol_report;
	setup_performance(&protocol_execution_context,
			(struct arm_scmi_performance_expected *) expected);
	arm_scmi_run(&protocol_execution_context, &received);
}
