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
#include <test_helper.h>


/*!
 * @brief Helper function to compare and report test status for two uint32_t.
 *
 *  @param test_report Pointer to the test report definition.
 *  @param protocol_flags Protocol expected flags.
 *  @param expected_bit Expected bit for the expected value.
 *  @param expected_value Expected uint32_t value.
 *  @param received_value Received uint32_t value.
 *  @param name Name of the test.
 *  @param format Format used to print the result.
 *
 */
void arm_scmi_check_and_report(struct arm_scmi_protocol_test_stats *test_report,
		uint32_t protocol_flags, uint32_t expected_bit, uint32_t expected_value,
		uint32_t received_value, const char *name, int format)
{
	enum test_status res;
	char *format_str;
	char *format_str1;

	if (format == T_UINT32_HEX) {
		format_str1 = "0x%08x";
		format_str = "Expected: 0x%08x, Received: 0x%08x";
	} else {
		format_str1 = "%d";
		format_str = "Expected: %d, Received: %d";
	}
	res = INFO;
	if (expected_bit & protocol_flags) {
		res = (expected_value == received_value) ? PASSED:FAILED;
		snprintf(description,  NUM_ELEMS(description), format_str,
		expected_value, received_value);
	}
	if ((res == INFO) || (res == PASSED)) {
		snprintf(description,  NUM_ELEMS(description), format_str1,
		received_value);
	}
	arm_scmi_log_test_result(test_report, res, name, description);
}

/*!
 * @brief Helper function to compare and report test status for two uint32_t
 * with decimal output.
 *
 *  @param test_report Pointer to the test report definition.
 *  @param protocol_flags Protocol expected flags.
 *  @param expected_bit Expected bit for the expected value.
 *  @param expected_value Expected uint32_t value.
 *  @param received_value Received uint32_t value.
 *  @param name Name of the test.
 *
 */
void arm_scmi_check_and_report_dec(
		struct arm_scmi_protocol_test_stats *test_report,
		uint32_t protocol_flags, uint32_t expected_bit, uint32_t expected_value,
		uint32_t received_value, const char *name)
{
	arm_scmi_check_and_report(test_report, protocol_flags, expected_bit,
		expected_value, received_value, name, T_UINT32_DEC);
}

/*!
 * @brief Helper function to compare and report test status for two uint32_t
 * with hexadecimal output.
 *
 *  @param test_report Pointer to the test report definition.
 *  @param protocol_flags Protocol expected flags.
 *  @param expected_bit Expected bit for the expected value.
 *  @param expected_value Expected uint32_t value.
 *  @param received_value Received uint32_t value.
 *  @param name Name of the test.
 *
 */
void arm_scmi_check_and_report_hex(
		struct arm_scmi_protocol_test_stats *test_report,
		uint32_t protocol_flags, uint32_t expected_bit, uint32_t expected_value,
		uint32_t received_value, const char *name)
{
	arm_scmi_check_and_report(test_report, protocol_flags, expected_bit,
		expected_value, received_value, name, T_UINT32_HEX);
}

/*!
 * @brief Helper function to compare and report test status for two strings.
 *
 *  @param test_report Pointer to the test report definition.
 *  @param protocol_flags Protocol expected flags.
 *  @param expected_bit Expected bit for the expected value.
 *  @param expected_str Expected string value.
 *  @param received_str Received string value.
 *  @param name Name of the test.
 *
 */
void arm_scmi_check_and_report_str(
		struct arm_scmi_protocol_test_stats *test_report,
		uint32_t protocol_flags, uint32_t expected_bit, char *expected_str,
		char *received_str, const char *name)
{
	enum test_status res;

	res = INFO;
	if (expected_bit & protocol_flags) {
		res = strcmp(expected_str, received_str) == 0 ? PASSED:FAILED;
		snprintf(description,  NUM_ELEMS(description),
		"Expected: '%s', Received: '%s'", expected_str, received_str);
	}
	if ((res == PASSED) || (res == INFO)) {
		snprintf(description,  NUM_ELEMS(description), "'%s'", received_str);
	}
	arm_scmi_log_test_result(test_report, res, name, description);
}

/*!
 * @brief Helper function to check the status of a message id for the
 * discovery test suite.
 *
 * 	Based on the command flags, protocol flags and if the suite must be
 * 	supported this function evaluates the status of the received value and
 * 	checks if the suite belonging to that message id must be skipped.
 *
 *  @param test_report Pointer to the test report definition.
 *  @param command_flags Command expected flags.
 *  @param protocol_flags Protocol expected flags.
 *  @param supported True if the test suite must be supported.
 *  @param message_id Message id of the command.
 *  @param status Received status of the command.
 *  @return True if the suite must be skipped; false otherwise.
 *
 */
bool arm_scmi_check_suite_skipped(
		struct arm_scmi_protocol_test_stats *test_report,
		uint32_t command_flags, uint32_t protocol_flags, bool supported,
		uint32_t message_id, uint32_t status)
{
	enum test_status result;
	uint32_t status_expected = supported ? SCMI_STATUS_SUCCESS:
			SCMI_STATUS_NOT_FOUND;
	bool must_test = true;

	description[0] = '\0';
	/* checking if it is implemented in the platform */
	switch (status) {
		case SCMI_STATUS_SUCCESS: /* valid status command responses*/
		case SCMI_STATUS_NOT_FOUND:
			if ((command_flags == ALL_BITS_SET) ||
					(command_flags & protocol_flags) != 0) {
				result = status_expected == status ? PASSED:FAILED;
				must_test = ((result == PASSED) && supported);
			}
			else if ((command_flags & protocol_flags) == 0) {
				result = INFO;
				must_test = status == SCMI_STATUS_SUCCESS;
			}
			break;
		default: /* unexpected status response */
			result = ERROR;
			must_test = false;
			break;
	}
	snprintf(name,  NUM_ELEMS(name), "CHECK STATUS@'message_id' = 0x%04x",
		message_id);
	if (result == FAILED)
		snprintf(description,  NUM_ELEMS(description),
		"Expected: %s, Received: %s", arm_scmi_status_code_str(status_expected),
		arm_scmi_status_code_str(status));
	else if (result == ERROR)
		snprintf(description,  NUM_ELEMS(description),
		"Unexpected status response: %s", arm_scmi_status_code_str(status));
	else if ((result == INFO) || (result == PASSED))
		snprintf(description,  NUM_ELEMS(description), "%s",
			arm_scmi_status_code_str(status));
	arm_scmi_log_test_result(test_report, result, name, description);
	return must_test;
}

/*!
 * @brief Helper function check if a returned value must be printed or checked.
 *
 *  @param expected_bit_index Bit index for the expected flag.
 *  @param protocol_flags Protocol expected flags.
 *  @returns CHECK if the expected bit is set PRINT otherwise.
 *
 */
int arm_scmi_print_or_check(uint32_t expected_bit_index,
		uint32_t protocol_flags)
{
	return (((LEFT_SHIFT(expected_bit_index) & protocol_flags) != 0) ?
			CHECK: PRINT);
}

/*!
 * @brief Helper function used to check if the returned values from the platform
 * must be skipped for test or print.
 *
 *  @param error_code Error code of the last command sent to the platform.
 *  @param status Status of the last command sent to the platform.
 *  @returns True if either error_code or status are not compliant.
 *
 */
bool arm_scmi_skip_return_values(int error_code, int32_t status)
{
	return ((error_code != NO_ERROR) || (status != SCMI_STATUS_SUCCESS));
}
