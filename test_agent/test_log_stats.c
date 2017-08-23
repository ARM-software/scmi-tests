/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <platform_exports.h>
#include <test_log_stats.h>

static const char *test_status_strings[] = {
		"UNDEFINED",
		"PASSED",
		"FAILED",
		"SKIP",
		"INFO",
		"ERROR",
		"CONFORMANT",
		"NON CONFORMANT",
		"UNTESTED"
};

/* Char buffer */
static char buffer_s[ID_STR_SIZE];

/*
 * This file provides access to the protocol log report functions that send
 * formatted strings to the platform log function and keeps track of protocol
 * test statistics.
 */

/*!
 * @brief Output a string to the platform.
 *
 *   @param test_stats Pointer to the statistics of the protocol tests. For
 *   future use and consistency.
 *   @param output Formatted string passed to the platform output interface.
 *
 */
void arm_scmi_log(struct arm_scmi_protocol_test_stats *test_stats,
		const char *output)
{
	arm_scmi_log_output(output);
}

/*!
 * @brief Format, output and store a protocol name.
 *
 *   @param test_stats Pointer to the statistics of the protocol tests.
 *   @param output String protocol name.
 *
 */
void arm_scmi_log_protocol_name(
		struct arm_scmi_protocol_test_stats *test_stats, const char *name)
{
	snprintf(buffer_s, NUM_ELEMS(buffer_s), "\nProtocol %s.-", name);
	test_stats->name = name;
	arm_scmi_log_output(buffer_s);
}

/*!
 * @brief Format, output and store a test suite name.
 *
 *   @param test_stats Pointer to the statistics of the protocol tests.
 *   @param output Test suite name.
 *
 */
void arm_scmi_log_test_suite_start(
		struct arm_scmi_protocol_test_stats *test_stats, const char *name)
{
	test_stats->test_suite.name = name;
	snprintf(buffer_s, NUM_ELEMS(buffer_s), "\n\n\t%s:", name);
	arm_scmi_log_output(buffer_s);
}

/*!
 * @brief Store a test case name.
 *
 *   @param test_stats Pointer to the statistics of the protocol tests.
 *   @param output Test case name.
 *
 */
void arm_scmi_log_test_case_start(
		struct arm_scmi_protocol_test_stats *test_stats, const char *name)
{
	test_stats->test_suite.test_case.name = name;
}

/*!
 * @brief Format and output the current test result along with
 * updating the test case statistics.
 *
 * @details The function keeps track of failures, errors and test results a test
 * case produces. It also prints the description of the test result based on the
 * test result status.
 *
 *   @param test_stats Pointer to the statistics of the protocol tests.
 *   @param status Test result status.
 *   @param name String name for test result.
 *   @param failure String description for test result that is only printed if
 *   the test result status is not PASSED.
 *
 */
void arm_scmi_log_test_result(struct arm_scmi_protocol_test_stats *test_stats,
		int status, const char *name, const char *description)
{
	snprintf(buffer_s, NUM_ELEMS(buffer_s), "\n\t\t\t%s: %s [%s]", name,
			test_status_strings[status], description);
	test_stats->test_suite.test_case.result = status;
	test_stats->test_suite.test_case.number.tests++;
	if (status == FAILED)
		test_stats->test_suite.test_case.number.failures++;
	else if (status == ERROR)
		test_stats->test_suite.test_case.number.errors++;
	else if (status == SKIP)
		test_stats->test_suite.test_case.number.skips++;
	arm_scmi_log_output(buffer_s);
}

/*!
 * @brief Return the test status of the current test case.
 *
 *   @param test_stats Pointer to the statistics of the protocol tests.
 *   @returns Test case status.
 *
 */
int arm_scmi_get_current_test_case_status(
		struct arm_scmi_protocol_test_stats *test_stats)
{
	if (test_stats->test_suite.test_case.number.failures > 0)
		return NON_CONFORMANT;
	else if (test_stats->test_suite.test_case.number.errors > 0)
		return UNDEFINED;
	else if (test_stats->test_suite.test_case.number.tests !=
		test_stats->test_suite.test_case.number.skips)
		return CONFORMANT;
	return UNTESTED;
}

/*!
 * @brief Format and output the current test case result along
 * with updating the test suite statistics.
 *
 * @details The function keeps track of failures, errors and test cases a test
 * suite produces. It also prints the description of the test case result based
 * in the test case status. This function must be called at the end of each test
 * case.
 *
 *   @param test_stats Pointer to the statistics of the protocol tests.
 *
 */
void arm_scmi_log_test_case_end(
		struct arm_scmi_protocol_test_stats *test_stats)
{
	snprintf(buffer_s, NUM_ELEMS(buffer_s), "\n\t\t%s: %s",
		test_stats->test_suite.test_case.name, test_status_strings[
		arm_scmi_get_current_test_case_status(test_stats)]);
	test_stats->test_suite.number.tests++;
	if (test_stats->test_suite.test_case.number.failures > 0)
		test_stats->test_suite.number.failures++;
	else if (test_stats->test_suite.test_case.number.errors > 0)
		test_stats->test_suite.number.errors++;
	else if (test_stats->test_suite.test_case.number.skips > 0)
		test_stats->test_suite.number.skips++;
	test_stats->test_suite.test_case.number = (struct arm_scmi_stats_counters)
		{};
	arm_scmi_log_output(buffer_s);
}

/*!
 * @brief Format and output a test suite result along with
 * updating its protocol report statistics.
 *
 * @details The function keeps track of failures, errors and test suites a
 * protocol report produces. This function must be called at the end of each
 * test suite.
 *
 *   @param test_stats Pointer to the statistics of the protocol tests.
 *
 */
void arm_scmi_log_test_suite_end(
		struct arm_scmi_protocol_test_stats *test_stats)
{
	test_stats->number.tests += test_stats->test_suite.number.tests;
	test_stats->number.failures += test_stats->test_suite.number.failures;
	test_stats->number.errors += test_stats->test_suite.number.errors;
	test_stats->number.skips += test_stats->test_suite.number.skips;
	test_stats->test_suite.number = (struct arm_scmi_stats_counters) {};
}
