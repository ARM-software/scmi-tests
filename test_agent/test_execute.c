/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <test_execute.h>

/* Global variables defined once and used across other compilation units */
char description[LOG_OUTPUT_STR_SIZE] = "";
char name[LOG_OUTPUT_STR_SIZE] = "";
/* Global variable defined once for Platform message storage and used across
 * other compilation units
 */
struct arm_scmi_message received;

/* Static array to return all returned values for all test cases */
static uint32_t return_buffer[MAX_RETURNS_SIZE];

/*!
 * @brief Return the string equivalent of the given SCMI code.
 *
 *  @param code SCMI error code.
 *  @returns String equivalence for given SCMI error code.
 *
 */
char *arm_scmi_status_code_str(int code)
{
	static char code_str[LOG_OUTPUT_STR_SIZE];

	switch(code){
		case SCMI_STATUS_SUCCESS:
			return "SCMI_STATUS_SUCCESS";
			break;
		case SCMI_STATUS_NOT_FOUND:
			return "SCMI_STATUS_NOT_FOUND";
			break;
		case SCMI_STATUS_NOT_SUPPORTED:
			return "SCMI_STATUS_NOT_SUPPORTED";
			break;
		case SCMI_STATUS_INVALID_PARAMETERS:
			return "SCMI_STATUS_INVALID_PARAMETERS";
			break;
		case SCMI_STATUS_DENIED:
			return "SCMI_STATUS_DENIED";
			break;
		case SCMI_STATUS_OUT_OF_RANGE:
			return "SCMI_STATUS_OUT_OF_RANGE";
			break;
		case SCMI_STATUS_HARDWARE_ERROR:
			return "SCMI_STATUS_HARDWARE_ERROR";
			break;
		case SCMI_STATUS_COMMS_ERROR:
			return "SCMI_STATUS_COMMS_ERROR";
			break;
		case SCMI_STATUS_GENERIC_ERROR:
			return "SCMI_STATUS_GENERIC_ERROR";
			break;
		case SCMI_STATUS_PROTOCOL_ERROR:
			return "SCMI_STATUS_PROTOCOL_ERROR";
			break;
		case SCMI_STATUS_BUSY:
			return "SCMI_STATUS_BUSY";
			break;
		default:
			snprintf(code_str,  NUM_ELEMS(description),
			"UNKNOWN SCMI STATUS CODE(%d)", code);
			return code_str;
			break;
	}
}

/*!
 * @brief Get only the test case name from the test case id.
 *
 * @details The test case id is conformed by [protocol name]-{Command name}-
 * {test case name}
 *
 *  @param test_case_id Pointer to the full test case id.
 *  @returns Pointer to test case name only.
 *
 */
static const char *get_test_case_name(const char const *test_case_id)
{
	const char *p = test_case_id;
	uint8_t occurrences = 2;

	assert(test_case_id != NULL);
	while ((*p != '\0') && (occurrences > 0))
		if (*(p++) == '-')
			occurrences--;
	return p;
}

/*!
 * @brief Check the expected returned values if they are defined.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param expected Pointer expected returned values.
 *  @param received Pointer to the received message.
 *
 */
static void check_expected_returned_values(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		return_value_desc_t *expected,
		struct arm_scmi_message *received)
{
	return_value_def_t *ret_def;
	uint8_t *rcv;
	uint8_t index;
	enum test_status return_status = INFO;
	uint32_t exp_u, rcv_u;

	index = 0;
	while ((expected != NULL) && (expected[index].type != T_EOT)) {
		name[0] = '\0';
		description[0] = '\0';
		ret_def = expected[index].def;
		rcv = (uint8_t *)received->payload + ret_def->offset;
		return_status = INFO;
		snprintf(name,  NUM_ELEMS(name), "RETURN@0x%0x '%s'", ret_def->offset,
			ret_def->name);
		switch (expected[index].type) {
			case T_UINT32_HEX:
				exp_u = expected[index].value.integer;
				rcv_u = *((uint32_t *)rcv);
				if (expected[index].flags == CHECK)
					return_status = (exp_u == rcv_u) ? PASSED:FAILED;
				if (return_status == FAILED)
					snprintf(description,  NUM_ELEMS(description),
					"Expected: 0x%08x, Received: 0x%08x", exp_u, rcv_u);
				else if ((return_status == INFO) || (return_status == PASSED))
					snprintf(description,  NUM_ELEMS(description),
					"0x%08x", rcv_u);
				break;
			case T_STRING:
				if (expected[index].flags == CHECK)
					return_status = strcmp(expected[index].value.string,
						(char *)rcv) ? FAILED:PASSED;
				if (return_status == FAILED)
					snprintf(description,  NUM_ELEMS(description),
					"Expected: '%s', Received: '%s'",
					expected[index].value.string, (char *)rcv);
				else if ((return_status == INFO) || (return_status == PASSED))
					snprintf(description,  NUM_ELEMS(description), "'%s'",
					(char *)rcv);
				break;
			case T_BIT_FIELD:
				exp_u = expected[index].value.integer;
				rcv_u = arm_scmi_get_norm_bits(*((uint32_t *)rcv),
						expected[index].def->msb, expected[index].def->lsb);
				if (expected[index].flags == CHECK)
					return_status = (exp_u == rcv_u) ? PASSED:FAILED;
				if (return_status == FAILED)
					snprintf(description,  NUM_ELEMS(description),
					"Expected: %d, Received: %d", exp_u, rcv_u);
				else if ((return_status == INFO) || (return_status == PASSED))
					snprintf(description,  NUM_ELEMS(description), "%d", rcv_u);
				break;
			default:
				/* Do nothing */
				break;
		}
		arm_scmi_log_test_result(
			&protocol_execution_context->test_stats, return_status, name,
			description);
		++index;
	}
}


/*!
 * @brief Get next test suite from the protocol execution context.
 *
 * This function increases the internal pointer to test suites array and gets
 * the next test suite to be tested if the suite is supported and there are more
 * suites to test. In any other case returns NULL.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @return Pointer to next test suite or NULL.
 *
 */
static struct arm_scmi_test_suite *next_test_suite(
		struct arm_scmi_protocol_execution_context *protocol_execution_context)
{
	uint8_t test_suite_index = ++protocol_execution_context->test_suite_index;
	struct arm_scmi_test_suite *test_suite =
			&protocol_execution_context->test_suites[test_suite_index];

	while ((test_suite_index < protocol_execution_context->num_test_suites) &&
			test_suite->skip) {
		test_suite =
				&protocol_execution_context->test_suites[++test_suite_index];
	}
	protocol_execution_context->test_suite_index = test_suite_index;
	if (test_suite_index < protocol_execution_context->num_test_suites) {
		protocol_execution_context->test_case_counter = -1;
		return test_suite;
	} else
		return NULL;
}

/*!
 * @brief Get next test case from the protocol execution context.
 *
 * This function increases the internal pointer to test cases and gets
 * the next test case within an array of test cases if the test case is
 * not EOT and there are more test cases to test. In any other case
 * returns NULL.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @return Pointer to next test suite (base array of test cases) or NULL.
 *
 */
static arm_scmi_test_case_t *next_test_case(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_suite)
{
	arm_scmi_test_case_t *test_case;

	if (test_suite == NULL) return NULL;
	protocol_execution_context->test_case_counter++;
	test_case = &test_suite[protocol_execution_context->test_case_counter];
	if (strcmp(test_case->testcase_id, EOT) != 0)
		return test_case;
	else
		return NULL;
}

/*!
 * @brief Build and send a synchronous command to the platform transport
 * interface, then wait for the platform response.
 *
 *  Sends a synchronous agent's command to the platform from test case data and
 *  returns with the platform response.
 *
 *  @param test_case Pointer to the test case definition.
 *  @param response Pointer to storage for the message response.
 *  @return Error code or 0 if no error.
 *
 */
static int send_receive_synchronous(arm_scmi_test_case_t *test_case,
		struct arm_scmi_message *response)
{
	int error_code;
	uint32_t header =
		GET_BITS(RESERVED, RESERVED_HIGH, RESERVED_LOW) |
		GET_BITS(TOKEN, TOKEN_HIGH, TOKEN_LOW) |
		GET_BITS(test_case->protocol_id, PROTOCOL_ID_HIGH, PROTOCOL_ID_LOW) |
		GET_BITS(COMMAND, MESSAGE_TYPE_HIGH, MESSAGE_TYPE_LOW) |
		GET_BITS(test_case->message_id, MESSAGE_ID_HIGH, MESSAGE_ID_LOW);

	uint32_t header_rcv;

	/* Implemented by the platform */
	error_code = arm_scmi_send_message(header, test_case->parameter_count,
			test_case->parameters, &header_rcv, &response->status,
			&response->payload_size, return_buffer);
	if (error_code == NO_ERROR) {
		/* conversion to local structure */
		response->reserved = arm_scmi_get_norm_bits(header_rcv, RESERVED_HIGH,
				RESERVED_LOW);
		response->token = arm_scmi_get_norm_bits(header_rcv,
				TOKEN_HIGH, TOKEN_LOW);
		response->protocol_id = arm_scmi_get_norm_bits(header_rcv,
				PROTOCOL_ID_HIGH, PROTOCOL_ID_LOW);
		response->message_type = arm_scmi_get_norm_bits(header_rcv,
				MESSAGE_TYPE_HIGH, MESSAGE_TYPE_LOW);
		response->message_id = arm_scmi_get_norm_bits(header_rcv,
				MESSAGE_ID_HIGH, MESSAGE_ID_LOW);
		response->payload = return_buffer;
	}
	return error_code;
}

/*!
 * @brief Build and send an asynchronous command to the platform transport
 * interface, then wait for the delayed response to the command.
 *
 *  Sends an asynchronous agent's command to the platform from test case data
 *  and returns with the platform response.
 *
 *  @param test_case Pointer to the test case definition.
 *  @param response_sync Pointer to the synchronous message response.
 *  @param response_async Pointer to the asynchronous message response.
 *  @param message_ready Pointer to flag to indicate if async message is ready.
 *
 */
static int send_receive_async(arm_scmi_test_case_t *test_case,
		struct arm_scmi_message *response_sync,
		struct arm_scmi_message *response_async,
		bool *message_ready)
{
	uint32_t header_async;
	int32_t status_async;
	int error_code;

	error_code = send_receive_synchronous(test_case, response_sync);
	if ((error_code != NO_ERROR) &&
			(response_sync->status == SCMI_STATUS_SUCCESS))
	{
		arm_scmi_wait_for_response(&header_async,
				&status_async,
				&response_async->payload_size,
				return_buffer,
				message_ready,
			test_case->timeout_in_sec);
		response_async->reserved = arm_scmi_get_norm_bits(header_async,
				RESERVED_HIGH, RESERVED_LOW);
		response_async->token =
				arm_scmi_get_norm_bits(header_async, TOKEN_HIGH, TOKEN_LOW);
		response_async->protocol_id =
				arm_scmi_get_norm_bits(header_async, PROTOCOL_ID_HIGH,
				PROTOCOL_ID_LOW);
		response_async->message_type =
				arm_scmi_get_norm_bits(header_async, MESSAGE_TYPE_HIGH,
				MESSAGE_TYPE_LOW);
		response_async->message_id =
				arm_scmi_get_norm_bits(header_async, MESSAGE_ID_HIGH,
				MESSAGE_ID_LOW);
		response_async->payload = return_buffer;
	}
	return error_code;
}

/*!
 * @brief Get the bits from a range of positions within an integer of type
 * uint32_t.
 *
 *  This function is used for extracting the data bits from an upper and lower
 *  range from an integer of type uint32_t. The bits are then shifted to the
 *  right to return the normalised value.
 *
 *  @param data Integer of type uin32_t from which the bits are extracted.
 *  @param bit_upper Upper bit position.
 *  @param bit_lower Lower bit position.
 *
 *  @return The normalised unsigned value from the bits range.
 *
 */
uint32_t arm_scmi_get_norm_bits(uint32_t data, uint8_t bit_upper,
		uint8_t bit_lower)
{
	uint32_t mask;
	uint32_t bits;
	mask = ((1 << (bit_upper - bit_lower + 1)) - 1) << bit_lower;
	bits = (data & mask) >> bit_lower;
	return bits;
}

/*!
 * @brief Initialize a protocol execution context.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param get_return_values_func Pointer to the function filling the expected
 *  return values.
 *  @param protocol_report_table Pointer to protocol table where the test
 *  results for each test case are stored.
 *
 */
void arm_scmi_init_protocol_execution_context(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		get_return_values_func_t get_return_values_func,
		struct arm_scmi_protocol_report *protocol_report_table)
{
	protocol_execution_context->num_test_suites = 0;
	protocol_execution_context->test_suite_index = -1;
	protocol_execution_context->test_case_counter = -1;
	protocol_execution_context->test_stats =
			(struct arm_scmi_protocol_test_stats){};
	protocol_execution_context->get_return_values_func = get_return_values_func;
	protocol_execution_context->protocol_report_table = protocol_report_table;
}

/*!
 * @brief Define the list of test suites for a protocol.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_suites Pointer to table of test suites.
 *  @param len Number of test suites.
 *
 */
void arm_scmi_set_test_suite_table(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		struct arm_scmi_test_suite *test_suites, size_t len)
{
	assert(len <= MAX_TEST_SUITES);
	protocol_execution_context->test_suites = test_suites;
	protocol_execution_context->num_test_suites = len;
}

/*!
 * @brief Ask for a test suite to be skipped, not executed.
 *
 *  @param protocol_execution_context Protocol protocol_execution_context.
 *  @param test_suite_index Index of the test suite in the protocol test suite
 *  array.
 *  @param value Boolean value to be set.
 *
 */
void arm_scmi_skip_test_suite(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		int test_suite_index, bool value)
{
	assert(test_suite_index < MAX_TEST_SUITES);
	protocol_execution_context->test_suites[test_suite_index].skip = value;
	protocol_execution_context->protocol_report_table->
			test_suite_report_table[test_suite_index].skip = value;
}

/*!
 * @brief Check if a test suite has to be skipped or executed.
 *
 *  @param protocol_execution_context Pointer to protocol execution context.
 *  @param test_suite_index Index of the test suite in the protocol test suite
 *  array.
 *  @return Boolean value at the implemented field in the structure.
 *
 */
bool arm_scmi_is_test_suite_skipped(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		int test_suite_index)
{
	assert(test_suite_index < MAX_TEST_SUITES);
	return protocol_execution_context->test_suites[test_suite_index].skip;
}

/*!
 * @brief Check end of test suite.
 *
 * Returns true if the end of the current test suite has been reached..
 *
 *  @param protocol_execution_context Protocol protocol_execution_context.
 *  @return Boolean value.
 *
 */
bool arm_scmi_check_end_of_test_suite(struct arm_scmi_protocol_execution_context
		*protocol_execution_context)
{
	return protocol_execution_context->test_suite_index >=
			protocol_execution_context->num_test_suites;
}

/*!
 * @brief Check if the header of a platform response is valid.
 *
 *  @param test_case Pointer to test case definition.
 *  @param received Pointer to message structure response.
 *
 */
enum test_status arm_scmi_assert_header(arm_scmi_test_case_t *test_case,
		struct arm_scmi_message *received)
{
	int res = ((received->reserved == RESERVED) &&
			(received->token == TOKEN) &&
			(received->protocol_id == test_case->protocol_id) &&
			(received->message_id == test_case->message_id) &&
			(received->message_type == COMMAND)
					) ? PASSED: FAILED;

	return res;
}

/*!
 * @brief Check if the status of a platform response is as expected.
 *  @param test_case Pointer to test case definition.
 *  @param received Pointer to message structure response.
 *
 */
enum test_status arm_scmi_assert_status(arm_scmi_test_case_t *test_case,
		struct arm_scmi_message *received)
{
	int res = (test_case->status == received->status) ? PASSED: FAILED;

	return res;
}

/*!
 * @brief Send a synchronous command and assert header and status returned
 * values from a platform message.
 *
 *  @param protocol_execution_context Protocol protocol_execution_context.
 *  @param test_case Pointer to test case definition.
 *  @param response Pointer to structure containing platform response.
 *  @returns Error code.
 *
 * Asserts:
 *      'protocol_execution_context' is not NULL.
 *
 */
int arm_scmi_execute_and_assert_test_case(
	struct arm_scmi_protocol_execution_context *protocol_execution_context,
	arm_scmi_test_case_t *test_case, struct arm_scmi_message *response)
{
	enum test_status res;
	int error_code;
	uint32_t recv_header;
	int i;

	assert( protocol_execution_context != NULL );
	error_code = send_receive_synchronous(test_case, response);
	if (test_case->precondition_indicator == PRECOND_DEFINE) {
		protocol_execution_context->precondition_data.status = response->status;
	}
	snprintf(description,  NUM_ELEMS(description),
		"\n\t\t\tMESSAGE_ID = 0x%02x", test_case->message_id);
	arm_scmi_log(&protocol_execution_context->test_stats,
		description);
	if (test_case->parameter_count > 0) {
		snprintf(description,  NUM_ELEMS(description),
			"\n\t\t\tPARAMETERS uin32_t[%d] = |", test_case->parameter_count);
		for (i = 0; i < test_case->parameter_count; ++i)
			sprintf(description + strlen(description), "%d|",
					test_case->parameters[i]);
		arm_scmi_log(&protocol_execution_context->test_stats,
			description);
	}
	if (error_code == NO_ERROR) {
		res = arm_scmi_assert_header(test_case, response);
		recv_header = ((response->reserved << RESERVED_LOW) |
				(response->token << TOKEN_LOW) |
				(response->protocol_id << PROTOCOL_ID_LOW) |
				(response->message_type << MESSAGE_TYPE_LOW) |
				(response->message_id << MESSAGE_ID_LOW));
		if (res == FAILED) {
			snprintf(description, NUM_ELEMS(description),
			"Expected: 0x%08x, Received: 0x%x",
			(RESERVED << RESERVED_LOW) | (TOKEN << TOKEN_LOW) |
			(test_case->protocol_id << PROTOCOL_ID_LOW) |
			(COMMAND << MESSAGE_TYPE_LOW) |
			(test_case->message_id << MESSAGE_ID_LOW), recv_header);
		} else {
			snprintf(description, NUM_ELEMS(description), "0x%08x", recv_header);
		}
		arm_scmi_log_test_result(&protocol_execution_context->test_stats, res,
			"CHECK HEADER", description);
		if (test_case->status != SCMI_STATUS_NOT_SPECIFIED) {
			description[0] = '\0';
			res = arm_scmi_assert_status(test_case, response);
			if (res == FAILED) {
				snprintf(description,  NUM_ELEMS(description),
				"Expected: %s Received: %s",
				arm_scmi_status_code_str(test_case->status),
				arm_scmi_status_code_str(response->status));
				arm_scmi_log_test_result(&protocol_execution_context->test_stats,
					res, "CHECK STATUS", description);
			} else {
				snprintf(description,  NUM_ELEMS(description), "%s",
				arm_scmi_status_code_str(response->status));
				arm_scmi_log_test_result(&protocol_execution_context->test_stats,
					res, "CHECK STATUS", description);
			}
		} else {
			arm_scmi_info_status(protocol_execution_context, response);
		}
	} else {
		arm_scmi_log_test_result(&protocol_execution_context->test_stats, ERROR,
				"CHECK TRANSPORT STATUS", "Error on transport layer.");
	}
	return error_code;
}

/*!
 * @brief Report the status from a platform message.
 *
 *  The function reports the status value from the returned message.
 *
 *  @param protocol_execution_context Protocol protocol_execution_context.
 *  @param received Pointer to structure containing platform response.
 *
 *  Asserts:
 *      'protocol_execution_context' is not NULL.
 *
 */
void arm_scmi_info_status(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		struct arm_scmi_message *received)
{
	int32_t received_status;

	assert( protocol_execution_context != NULL );
	received_status = received->status;
	snprintf(description, NUM_ELEMS(description), "%s", arm_scmi_status_code_str(
			received_status));
	arm_scmi_log_test_result(&protocol_execution_context->test_stats, INFO,
			"INFO STATUS", description);
}

/*!
 * @brief Run the test suites of a protocol.
 *
 *  The function sends and receives commands, call hooks, execute
 *  tests and log output. It is used for generic manipulation
 *  of common test cases but also provides customisation using hooks
 *  (pointers to functions) for every generic step.
 *
 *  @param protocol_execution_context Protocol protocol_execution_context.
 *  @param received Pointer to the received message.
 *
 */
void arm_scmi_run(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		struct arm_scmi_message *received)
{
	struct arm_scmi_test_suite *test_suite;
	arm_scmi_test_case_t *test_case;
	get_return_values_func_t get_return_values_func = NULL;
	return_value_desc_t *expected;
	struct arm_scmi_protocol_execution_context *context =
		protocol_execution_context;

	int error_code;
	int test_status;
	struct arm_scmi_protocol_report *table = context->protocol_report_table;
	/* Loop through all test cases for all test suites */
	while ((test_suite = next_test_suite(context)) != NULL) {
		arm_scmi_log_test_suite_start(&context->test_stats, test_suite->name);
		/* Initialize precondition test status per test suite */
		context->precondition_data.result = UNDEFINED;
		while ((test_case = next_test_case(context,
				test_suite->test_case_table)) != NULL) {
			arm_scmi_log_test_case_start(&context->test_stats,
					test_case->testcase_id);
			/* Set the return values function either by test case (build)
			 * or run time (protocol_execution_context)
			 */
			get_return_values_func =
				(test_case->get_return_values_func != NULL) ?
						test_case->get_return_values_func: // by test case
						context->get_return_values_func;
			/* Call the function to fill the expected return values and flags
			 * for the current test case*/
			if (get_return_values_func != NULL)
				get_return_values_func(test_case);
			/* Check if the test case has a custom test function */
			if (test_case->custom_tester_func != NULL) {
				test_case->custom_tester_func(context, test_case);
			} else {
				/* If not then send the command to the platform and get the
				 * response, i.e. Generic testing.
				 */
				error_code = arm_scmi_execute_and_assert_test_case(context,
					test_case, received);
				if (error_code != NO_ERROR)
					continue; /* In case of error skip this test case tests */
				/* Check if they are expected returns */
				if (received->status == SCMI_STATUS_SUCCESS) {
					expected = test_case->expected_returns;
					/* Check if they are expected returns */
					check_expected_returned_values(context, expected, received);
				}
			}
			/* After custom or generic testing, check if they are
			 * post-processing tasks, i.e more tests or custom after test
			 * processes.
			 */
			if (test_case->post_proccesing_func != NULL)
				test_case->post_proccesing_func(context, test_case, received);
			/* Now report the results back to the agent handler */
			test_status = arm_scmi_get_current_test_case_status(
					&context->test_stats);
			table->test_suite_report_table[
			context->test_suite_index].test_case_report_table[
						context->test_case_counter] =
					(struct arm_scmi_test_case_report){
							.test_case_name =
								get_test_case_name(test_case->testcase_id),
							.test_status = test_status
						};
			/* Check if it is a precondition set */
			if (test_case->precondition_indicator == PRECOND_DEFINE) {
				context->precondition_data.result = test_status;
			}
			/* Signal end of test case to the test engine */
			arm_scmi_log_test_case_end(&context->test_stats);
		}
		/* Signal end of test suite to the test engine */
		arm_scmi_log_test_suite_end(&context->test_stats);
	}
}
