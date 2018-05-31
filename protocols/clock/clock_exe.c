/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <protocol_common.h>
#include <clock_common.h>
#include <clock_expected.h>

/* Variable to hold expected data */
static struct arm_scmi_clock_expected *clock_expected =
		&(struct arm_scmi_clock_expected){};
/* Variable to hold the expected flags mask */
static uint32_t expected_flags_mask;

enum {MIN_CLOCK, MAX_CLOCK = 3};

struct clock_device {
	struct arm_scmi_clock_rate rates[4];
	struct arm_scmi_clock_rate current_rate;
	bool rate_set;
	uint32_t enabled; /* Enabled or disabled */
	bool enabled_set;
};

struct clock_protocol_data {
	uint32_t num_clocks;
	uint32_t max_num_pending_async_rate_chgs_supported;
	uint8_t format;
	struct clock_device clocks[MAX_NUM_CLOCK];
};

/* Global variable to hold run-time base protocol data */
static struct clock_protocol_data clock_protocol_data;

/*!
 * @brief Function used to fill up expected test case return data from
 * expected protocol return data.
 *
 *  The function is called by the test engine just before sending
 *  the command in the test case and testing the received data. It also
 *  sets other hooks depending on the message id. To see the expected_returns
 *  index meaning see 'clock_tests.c' for each specific command.
 *
 *  @param test case Pointer to the test case being executed in the test engine.
 *
 */
static void clock_fill_expected_return_values(arm_scmi_test_case_t *test_case)
{
	return_value_desc_t *expected_ret;

	switch (test_case->message_id)
	{
		case CLK_PROTO_VER_MSG_ID:
			expected_ret = &test_case->expected_returns[0];
			expected_ret->value.integer = clock_expected->protocol_version;
			expected_ret->flags = arm_scmi_print_or_check(
				CLOCK_PROTOCOL_VERSION, expected_flags_mask);
			break;
		case CLK_PROTO_ATTRIB_MSG_ID:
			expected_ret = &test_case->expected_returns[1];
			expected_ret->value.integer =
				clock_expected->max_num_pending_async_rate_chg_supported;
			expected_ret->flags = arm_scmi_print_or_check(
				MAX_NUM_PENDING_ASYNC_RATE_CHG_SUPPORTED, expected_flags_mask);
			expected_ret = &test_case->expected_returns[2];
			expected_ret->value.integer =
				clock_expected->number_clocks;
			expected_ret->flags = arm_scmi_print_or_check(NUMBER_CLOCKS,
				expected_flags_mask);
			break;
		case CLK_PROTO_MSG_ATTRIB_MSG_ID:
			test_case->expected_returns[0].flags = CHECK;
			break;
		case CLK_DESC_RATE_MSG_ID:
			/* Custom tester */
			break;
		case CLK_RATE_SET_MSG_ID:
			/* Custom tester */
			break;
		case CLK_RATE_GET_MSG_ID:
			/* Custom tester */
			break;
		case CLK_CONFIG_SET_MSG_ID:
			/* Custom tester */
			break;
	}
}

/*!
 * @brief Setup function for clock domain management tests execution.
 *
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param expected Pointer to the expected values for the performance protocol.
 *
 */
static void setup_clock(
	struct arm_scmi_protocol_execution_context *protocol_execution_context,
	struct arm_scmi_clock_expected *expected)
{
	int index;
	struct arm_scmi_test_suite_report *tsr =
			arm_scmi_clock_protocol_report.test_suite_report_table;

	/* If no expected values are provided (i.e. NULL) it is assigned an
	 * empty compound literal in order to avoid if != NULL in every
	 * base_expected value.
	 */
	if (expected == NULL) {
		expected_flags_mask = 0;
	} else {
		clock_expected = expected;
		expected_flags_mask = expected->flags_mask;
	};
	arm_scmi_init_protocol_execution_context(protocol_execution_context,
		clock_fill_expected_return_values,
		&arm_scmi_clock_protocol_report);
	arm_scmi_set_test_suite_table(protocol_execution_context,
		arm_scmi_clock_test_suites,
		arm_scmi_clock_test_suites_size);
	for (index = 0; index < arm_scmi_clock_test_suites_size; ++index)
		arm_scmi_clock_test_suites[index].name = (tsr + index)->test_suite_name;
	arm_scmi_skip_test_suite(protocol_execution_context, TS_CLOCK_DISCOVERY,
		false);
	arm_scmi_log_protocol_name(&protocol_execution_context->test_stats, "Clock");
}

/*!
 * @brief Function used to check if a command is supported.
 *
 *  The function is called by the test engine executor after testing the
 *  PROTOCOL_MESSAGE_ATTRIBUTES message id in order to set a test suite
 *  if the command is supported
 *
 *  @param testcase Parameter by reference to the test case being executed in
 *  the test engine executor.
 *  @param expected Parameter by reference where the protocol expected returned
 *  data is stored.
 *  @param received Parameter by reference to the received message structure.
 *
 */
void arm_scmi_post_processing_clock_protocol_message_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case, struct arm_scmi_message *received)
{
	uint32_t mask;
	int index = -1;
	bool must_be_supported = true;
	bool must_test_suite = true;
	uint32_t message_id = test_case->parameters[OFFSET_PARAM(
		struct arm_scmi_clock_protocol_message_attributes, message_id)];

	switch (message_id)
	{
		case CLK_ATTRIB_MSG_ID:
			mask = ALL_BITS_SET;
			index = TS_CLOCK_ATTRIBUTE;
			break;
		case CLK_DESC_RATE_MSG_ID:
			mask= ALL_BITS_SET;
			index = TS_CLOCK_DESC_RATE;
			break;
		case CLK_RATE_SET_MSG_ID:
			mask = ALL_BITS_SET;
			index = TS_CLOCK_RATE_SET;
			break;
		case CLK_RATE_GET_MSG_ID:
			mask = ALL_BITS_SET;
			index = TS_CLOCK_RATE_GET;
			break;
		case CLK_CONFIG_SET_MSG_ID:
			mask = ALL_BITS_SET;
			index = TS_CLOCK_CONFIG_SET;
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
 * @brief Function to test the clock describe rates command.
 *
 *  The function is called by the test engine before sending the
 *  clock describe rates command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_tester_clock_describe_rates(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case)
{
	uint8_t clock_id;
	uint32_t num_rates_flags;
	uint8_t num_rates = 0;
	uint32_t rcv_u, exp_u;
	uint32_t rate_index = 0;
	uint32_t remaining_rates;
	struct arm_scmi_clock_rate *rcv_rates, *rates;
	size_t nr;
	struct arm_scmi_protocol_test_stats *test_stats =
			&protocol_execution_context->test_stats;
	uint64_t op1, step_size;
	struct arm_scmi_clock_rate **discrete;
	int error_code;

	/* For all test cases allocate space for parameters */
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	test_case->parameter_count = 2;
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
			 struct arm_scmi_clock_describe_rates, clock_id)) {
		/* Negative test case*/
		test_case->parameters[OFFSET_PARAM(struct arm_scmi_clock_describe_rates,
			clock_id)] = clock_protocol_data.num_clocks;
		test_case->parameters[OFFSET_PARAM(struct arm_scmi_clock_describe_rates,
			rate_index)] = 0;
		arm_scmi_execute_and_assert_test_case(protocol_execution_context,
			test_case, &received);
	} else {
		/* Positive test case */
		/* For every clock id */
			for (clock_id = 0; clock_id < clock_protocol_data.num_clocks;
				++clock_id) {
			snprintf(description, NUM_ELEMS(description), "\n\t\tCLOCK ID %d",
				clock_id);
			arm_scmi_log(test_stats, description);
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_clock_describe_rates, clock_id)] = clock_id;
			rate_index = 0;
			/* Assign pointer to clock protocol data rates */
			rates = clock_protocol_data.clocks[clock_id].rates;
			do {
			/* start with the first rate in the range */
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_clock_describe_rates, rate_index)] = rate_index;
			error_code = arm_scmi_execute_and_assert_test_case(
					protocol_execution_context, test_case, &received);
			if (arm_scmi_skip_return_values(error_code, received.status))
				break;
			/* Get num_rates_flags return value */
			num_rates_flags = RETURN_VAL(received,
				struct arm_scmi_clock_describe_rates, num_rates_flags);
			/* Print Number of remaining rates */
			remaining_rates = arm_scmi_get_norm_bits(num_rates_flags,
				CLK_DESCRATE_NUM_RATES_REMAINING_HIGH,
				CLK_DESCRATE_NUM_RATES_REMAINING_LOW);
			arm_scmi_check_and_report_dec(test_stats, ALL_BITS_SET, PRINT, 0,
				remaining_rates, "NUMBER OF REMAINING RATES");
			/* Check Reserved bits */
			rcv_u = arm_scmi_get_norm_bits(num_rates_flags,
				CLK_DESCRATE_RESERVED_HIGH, CLK_DESCRATE_RESERVED_LOW);
			arm_scmi_check_and_report_dec(test_stats, ALL_BITS_SET, CHECK,
				RESERVED, rcv_u, "RESERVED");
			/* Check/Print format bits */
			uint32_t format = arm_scmi_get_norm_bits(num_rates_flags,
				CLK_DESCRATE_RETURN_FORMAT_HIGH, CLK_DESCRATE_RETURN_FORMAT_LOW);
			clock_protocol_data.format = format;
			if (clock_expected->rate_format_supported) {
				arm_scmi_check_and_report_dec(test_stats, expected_flags_mask,
					LEFT_SHIFT(RATE_FORMAT_SUPPORTED),
					clock_expected->rate_format_supported[clock_id],
                                        format, "FORMAT");
			} else {
				arm_scmi_check_and_report_dec(test_stats, expected_flags_mask,
					LEFT_SHIFT(RATE_FORMAT_SUPPORTED), 0, format, "FORMAT");
			}
			/* Check/print number of rates */
			num_rates = arm_scmi_get_norm_bits(num_rates_flags,
				CLK_DESCRATE_NUM_RATES_RETURNED_HIGH,
				CLK_DESCRATE_NUM_RATES_RETURNED_LOW);
			exp_u = clock_expected->num_rates == NULL ? 0:
				clock_expected->num_rates[clock_id];
			arm_scmi_check_and_report_dec(test_stats, expected_flags_mask,
				LEFT_SHIFT(CLOCK_NUM_RATES), exp_u, num_rates, "NUMBER OF RATES");
			/* Check/print rate array */
			rcv_rates = (struct arm_scmi_clock_rate *) &received.payload[
				OFFSET_RET(struct arm_scmi_clock_describe_rates, rates)];
			if (format == THREE_TUPLE_RATE_FORMAT) {
				/* Check/print 3-tuple values */
				/* Check/print LOWEST physical rate */
				exp_u = clock_expected->low_clock_rate == NULL ? 0:
						clock_expected->low_clock_rate[clock_id].lower;
				arm_scmi_check_and_report_dec(test_stats, expected_flags_mask,
					LEFT_SHIFT(THREE_TUPLE_CLOCK_RATES), exp_u,
					rcv_rates[LOWEST_RATE].lower, "LOWEST RATE {LOWER}");
				exp_u = clock_expected->low_clock_rate == NULL ? 0:
						clock_expected->low_clock_rate[clock_id].upper;
				arm_scmi_check_and_report_dec(test_stats, expected_flags_mask,
					LEFT_SHIFT(THREE_TUPLE_CLOCK_RATES), exp_u,
					rcv_rates[LOWEST_RATE].upper, "LOWEST RATE {UPPER}");
				/* Check/print HIGHEST physical rate */
				exp_u = clock_expected->high_clock_rate == NULL ? 0:
						clock_expected->high_clock_rate[clock_id].lower;
				arm_scmi_check_and_report_dec(
					test_stats, expected_flags_mask,
					LEFT_SHIFT(THREE_TUPLE_CLOCK_RATES), exp_u,
					rcv_rates[HIGHEST_RATE].lower, "HIGHEST RATE {LOWER}");
				exp_u = clock_expected->high_clock_rate == NULL ? 0:
						clock_expected->high_clock_rate[clock_id].upper;
				arm_scmi_check_and_report_dec(test_stats, expected_flags_mask,
					LEFT_SHIFT(THREE_TUPLE_CLOCK_RATES), exp_u,
					rcv_rates[HIGHEST_RATE].upper, "HIGHEST RATE {UPPER}");
				/* Check/print STEP size */
				exp_u = clock_expected->step_clock_rate == NULL ? 0:
						clock_expected->step_clock_rate[clock_id].lower;
				arm_scmi_check_and_report_dec(test_stats, expected_flags_mask,
					LEFT_SHIFT(THREE_TUPLE_CLOCK_RATES), exp_u,
					rcv_rates[STEP_SIZE].lower, "STEP SIZE {LOWER}");
				exp_u = clock_expected->step_clock_rate == NULL ? 0:
						clock_expected->step_clock_rate[clock_id].upper;
				arm_scmi_check_and_report_dec(test_stats, expected_flags_mask,
					LEFT_SHIFT(THREE_TUPLE_CLOCK_RATES), exp_u,
					rcv_rates[STEP_SIZE].upper, "STEP SIZE {UPPER}");
				/* Store clock rate values */
				rates[MIN_CLOCK] = rcv_rates[LOWEST_RATE];
				op1 = (((uint64_t)rcv_rates[LOWEST_RATE].upper) << 32) |
					rcv_rates[LOWEST_RATE].lower;
				step_size = (((uint64_t)rcv_rates[STEP_SIZE].upper) << 32) |
						rcv_rates[STEP_SIZE].lower;
				op1 += step_size;
				rates[MIN_CLOCK + 1] = (struct arm_scmi_clock_rate){
					(uint32_t) op1, (uint32_t) (op1 >> 32)};
				op1 = (((uint64_t)rcv_rates[HIGHEST_RATE].upper) << 32) |
					rcv_rates[HIGHEST_RATE].lower;
				op1 -= step_size;
				rates[MAX_CLOCK - 1] = (struct arm_scmi_clock_rate){
					(uint32_t) op1, (uint32_t) (op1 >> 32)};
				rates[MAX_CLOCK] = rcv_rates[HIGHEST_RATE];

			} else {
				/* Discrete clock rates */
				discrete = clock_expected->discrete_clock_rates;
				for (nr = 0; nr < num_rates; ++nr) {
					exp_u = clock_expected->discrete_clock_rates == NULL ? 0:
						discrete[clock_id][nr + rate_index].lower;
					arm_scmi_check_and_report_dec(test_stats,
						expected_flags_mask, LEFT_SHIFT(DISCRETE_CLOCK_RATES),
						exp_u, rcv_rates->lower, "LOWER WORD");
					exp_u = clock_expected->discrete_clock_rates == NULL ? 0:
						discrete[clock_id][nr + rate_index].upper;
					arm_scmi_check_and_report_dec(test_stats,
						expected_flags_mask, LEFT_SHIFT(DISCRETE_CLOCK_RATES),
						exp_u, rcv_rates->upper, "UPPER WORD");
					/* Store discrete clock rates */
					if ((rate_index + nr) == 0) {
						rates[MIN_CLOCK] = rates[MIN_CLOCK + 1] =
						rates[MAX_CLOCK - 1] = rates[MAX_CLOCK] = *rcv_rates;
					} else if ((rate_index + nr) == 1) {
						rates[MIN_CLOCK + 1] = rates[MAX_CLOCK - 1] =
						rates[MAX_CLOCK] = *rcv_rates;
					}
					rates[MAX_CLOCK - 1] = rates[MAX_CLOCK];
					rates[MAX_CLOCK] = *rcv_rates;
					rcv_rates += 1;
				}
			}
			rate_index += num_rates;
		} while (remaining_rates > 0);
	}
	}
}

/*!
 * @brief Function to test the clock attributes command.
 *
 *  The function is called by the test engine before sending the clock
 *  attributes command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_tester_clock_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case)
{
	uint32_t clock_id;
	uint32_t rcv_u;
	char *clock_name_rcv;
	char *clock_name_exp;
	uint32_t attributes;
	int error_code;

	/* For all test cases allocate space for parameters */
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	test_case->parameter_count = 1;
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
			struct arm_scmi_clock_attributes, clock_id)) {
		/* Negative test case */
		test_case->parameters[OFFSET_PARAM(struct arm_scmi_clock_attributes,
				clock_id)] = clock_protocol_data.num_clocks;
		arm_scmi_execute_and_assert_test_case(protocol_execution_context,
			test_case, &received);
	} else {
		/* Test with valid parameter */
		for (clock_id = 0; clock_id < clock_protocol_data.num_clocks;
				++clock_id) {
			snprintf(description, NUM_ELEMS(description), "\n\t\tCLOCK ID %d",
				clock_id);
			arm_scmi_log(&protocol_execution_context->test_stats, description);
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_clock_attributes, clock_id)] = clock_id;
			error_code = arm_scmi_execute_and_assert_test_case(
					protocol_execution_context, test_case, &received);
			if (arm_scmi_skip_return_values(error_code, received.status))
				continue;
			attributes = RETURN_VAL(received, struct arm_scmi_clock_attributes,
				attributes);
			/* Check Reserved attributes */
			rcv_u = arm_scmi_get_norm_bits(attributes,
				CLK_ATTRIBUTES_RESERVED_HIGH, CLK_ATTRIBUTES_RESERVED_LOW);
			arm_scmi_check_and_report_dec(&protocol_execution_context->test_stats,
				ALL_BITS_SET, CHECK, RESERVED, rcv_u, "RESERVED");
			/* Check/Print enabled/disabled clock */
			rcv_u = arm_scmi_get_norm_bits(attributes, CLK_ATTRIBUTES_ENABLED,
				CLK_ATTRIBUTES_ENABLED);
			arm_scmi_check_and_report_dec(&protocol_execution_context->test_stats,
				ALL_BITS_SET, clock_protocol_data.clocks[clock_id].enabled_set ?
				CHECK: PRINT, clock_protocol_data.clocks[clock_id].enabled,
				rcv_u, "ENABLED/DISABLED");
			/* Check/print clock names */
			clock_name_rcv = RETURN_STR(received,
				struct arm_scmi_clock_attributes, clock_name);
			clock_name_exp = (clock_expected->clock_name == NULL) ? NULL:
					(char *)clock_expected->clock_name[clock_id];
			arm_scmi_check_and_report_str(
				&protocol_execution_context->test_stats,
				expected_flags_mask, LEFT_SHIFT(CLOCK_NAMES), clock_name_exp,
				clock_name_rcv, "CLOCK NAME");
		}
	}
}

/*!
 * @brief Function to test the clock rate set command.
 *
 *  The function is called by the test engine before sending the clock
 *  rate set command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_tester_clock_rate_set(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case)
{
	uint32_t clock_id;
	struct clock_device *clock;

	/* For all test cases allocate space for parameters */
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	test_case->parameter_count = 4;
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
			struct arm_scmi_clock_rate_set, clock_id)) {
		/* Negative test case*/
		test_case->parameters[OFFSET_PARAM(struct arm_scmi_clock_rate_set,
			flags)] = 0;
		test_case->parameters[OFFSET_PARAM(struct arm_scmi_clock_rate_set,
			clock_id)] = clock_protocol_data.num_clocks;
		/* use any valid rate below */
		clock = &clock_protocol_data.clocks[0];
		test_case->parameters[OFFSET_PARAM(struct arm_scmi_clock_rate_set,
			rate)] = clock->rates[MIN_CLOCK + 1].lower;
		test_case->parameters[OFFSET_PARAM(struct arm_scmi_clock_rate_set,
			rate ) + 1] = clock->rates[MIN_CLOCK + 1].upper;
		arm_scmi_execute_and_assert_test_case(protocol_execution_context,
			test_case, &received);
	} else {
		/* Check if there is a precondition */
		if ((test_case->precondition_indicator == PRECOND_USE) &&
		((protocol_execution_context->precondition_data.result != CONFORMANT) ||
		(protocol_execution_context->precondition_data.status !=
			SCMI_STATUS_SUCCESS))) {
			arm_scmi_log_test_result(&protocol_execution_context->test_stats,
				SKIP, "PRECONDITION STATUS", "PRECONDITION NON-CONFORMANT");
			return;
		}
		/* Test with valid parameter */
		test_case->parameters[
		OFFSET_PARAM(struct arm_scmi_clock_rate_set, flags)] = 0;
		/* Loop through all clocks visible to this agent */
		for (clock_id = 0; clock_id < clock_protocol_data.num_clocks;
				++clock_id) {
			clock = &clock_protocol_data.clocks[clock_id];
			snprintf(description, NUM_ELEMS(description), "\n\t\tCLOCK ID %d",
				clock_id);
			arm_scmi_log(&protocol_execution_context->test_stats, description);
			test_case->parameters[OFFSET_PARAM(struct arm_scmi_clock_rate_set,
				clock_id)] = clock_id;
			test_case->parameters[OFFSET_PARAM(struct arm_scmi_clock_rate_set,
				rate)] = clock->rates[MIN_CLOCK + 1].lower;
			test_case->parameters[OFFSET_PARAM(struct arm_scmi_clock_rate_set,
				rate) + 1] = clock->rates[MIN_CLOCK + 1].upper;
			arm_scmi_execute_and_assert_test_case(protocol_execution_context,
				test_case, &received);
			/* Update current level */
			if (received.status == SCMI_STATUS_SUCCESS) {
				clock->current_rate.lower = test_case->parameters[OFFSET_PARAM(
					struct arm_scmi_clock_rate_set, rate)];
				clock->current_rate.upper = test_case->parameters[OFFSET_PARAM(
					struct arm_scmi_clock_rate_set, rate) + 1];
				clock->rate_set = true;
			}
		}
	}
}

/*!
 * @brief Function to test the clock rate set command.
 *
 *  The function is called by the test engine before sending the clock
 *  rate set command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_tester_clock_rate_get(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case)
{
	uint32_t clock_id;
	uint32_t *clock_rate;
	struct arm_scmi_protocol_test_stats *test_stats =
		&protocol_execution_context->test_stats;
	int error_code;
	int check_or_print;

	/* For all test cases allocate space for parameters */
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	test_case->parameter_count = 1;
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
		struct arm_scmi_clock_rate_get, clock_id)) {
		/* Negative test case*/
		test_case->parameters[OFFSET_PARAM(struct arm_scmi_clock_rate_get,
			clock_id)] = clock_protocol_data.num_clocks;
		arm_scmi_execute_and_assert_test_case(protocol_execution_context,
			test_case, &received);
	} else {
		/* Check if there is a precondition */
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
		/* Test with valid parameter */
		for (clock_id = 0; clock_id < clock_protocol_data.num_clocks;
				++clock_id) {
			snprintf(description, NUM_ELEMS(description), "\n\t\tCLOCK ID %d",
				clock_id);
			arm_scmi_log(test_stats, description);
			test_case->parameters[OFFSET_PARAM(struct arm_scmi_clock_rate_get,
				clock_id)] = clock_id;
			arm_scmi_execute_and_assert_test_case(protocol_execution_context,
				test_case, &received);
			error_code = arm_scmi_execute_and_assert_test_case(
					protocol_execution_context, test_case, &received);
			if (arm_scmi_skip_return_values(error_code, received.status))
				continue;
			/* Check/Print clock rate */
			clock_rate = &(received.payload[OFFSET_RET(
				struct arm_scmi_clock_rate_get, rate)]);
			arm_scmi_check_and_report_hex(&protocol_execution_context->test_stats,
				ALL_BITS_SET, check_or_print, clock_protocol_data.clocks[
				clock_id].current_rate.lower, clock_rate[0], "CLOCK RATE LOWER");
			arm_scmi_check_and_report_hex(&protocol_execution_context->test_stats,
				ALL_BITS_SET, check_or_print, clock_protocol_data.clocks[
				clock_id].current_rate.upper, clock_rate[1], "CLOCK RATE UPPER");
		}
	}
}

/*!
 * @brief Function to fill protocol attributes.
 *
 *  The function is called by the test engine executor after testing the protocol
 *  attributes and fill the returned or expected data for global values of
 *  number of clocks and max number of pending asynchronous clock rate changes.
 *
 *  @param protocol_exe_handler Parameter by reference to the protocol handler.
 *  @param testcase Parameter by reference to the current test case.
 *  @param received Parameter by reference to platform response.
 *
 */
void arm_scmi_post_processing_clock_protocol_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case,
		struct arm_scmi_message *received)
{
	uint32_t attributes;

	attributes = received->payload[OFFSET_RET(
		struct arm_scmi_clock_protocol_attributes, attributes)];
	if (received->status == SCMI_STATUS_SUCCESS) {
		clock_protocol_data.num_clocks = arm_scmi_get_norm_bits(attributes,
			CLK_NUMBER_CLOCKS_HIGH, CLK_NUMBER_CLOCKS_LOW);
		clock_protocol_data.max_num_pending_async_rate_chgs_supported =
	(expected_flags_mask & LEFT_SHIFT(MAX_NUM_PENDING_ASYNC_RATE_CHG_SUPPORTED)) ?
			clock_expected->max_num_pending_async_rate_chg_supported:
	arm_scmi_get_norm_bits(attributes, CLK_MAX_NUM_PENDING_ASYNC_RATE_CHGS_SPRT_HIGH,
			CLK_MAX_NUM_PENDING_ASYNC_RATE_CHGS_SPRT_LOW);
	}
}

/*!
 * @brief Function to test the clock rate set command.
 *
 *  The function is called by the test engine before sending the clock
 *  rate set command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_tester_clock_config_set(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case)
{
	uint32_t clock_id;
	struct clock_device *clock;

	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
		struct arm_scmi_clock_config_set, clock_id)) {
		test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
		test_case->parameter_count = 2;
		/* Negative test case*/
		/* For all test cases allocate space for parameters */
		test_case->parameters[OFFSET_PARAM(struct arm_scmi_clock_config_set,
			clock_id)] = clock_protocol_data.num_clocks;
		/* use any valid attributes below */
		test_case->parameters[OFFSET_PARAM(struct arm_scmi_clock_config_set,
			attributes)] = 0;
		arm_scmi_execute_and_assert_test_case(protocol_execution_context,
			test_case, &received);
	} else {
		/* Test with valid parameter */
		/* Loop through all clocks visible to this agent */
		for (clock_id = 0; clock_id < clock_protocol_data.num_clocks;
				++clock_id) {
			clock = &clock_protocol_data.clocks[clock_id];
			snprintf(description, NUM_ELEMS(description), "\n\t\tCLOCK ID %d",
				clock_id);
			arm_scmi_log(&protocol_execution_context->test_stats, description);
			test_case->parameters[OFFSET_PARAM(struct arm_scmi_clock_config_set,
				clock_id)] = clock_id;
			/* The attributes parameter is set at build time */
			arm_scmi_execute_and_assert_test_case(protocol_execution_context,
				test_case, &received);
			/* Update current level */
			if (received.status == SCMI_STATUS_SUCCESS) {
				clock->enabled = arm_scmi_get_norm_bits(test_case->parameters[
					OFFSET_PARAM(struct arm_scmi_clock_config_set, attributes)],
					CLK_CONFIGSET_ATTR_ENABLED, CLK_CONFIGSET_ATTR_ENABLED);
				clock->enabled_set = true;
			}
		}
	}
}

/*!
 * @brief Executor function that runs the test suites/cases for the clock
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
void arm_scmi_clock_execute(const void *expected,
		struct arm_scmi_protocol_report **protocol_report)
{
	static struct arm_scmi_protocol_execution_context protocol_execution_context;

	*protocol_report = &arm_scmi_clock_protocol_report;
	setup_clock(&protocol_execution_context,
			(struct arm_scmi_clock_expected *) expected);
	arm_scmi_run(&protocol_execution_context, &received);
}
