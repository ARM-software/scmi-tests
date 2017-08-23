/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef TEST_EXECUTE_H_
#define TEST_EXECUTE_H_
#include <common.h>
#include <platform_exports.h>
#include <test_log_stats.h>
#include <test_agent_exports.h>

/* Global variables to be used within protocols and test agent */
extern char description[LOG_OUTPUT_STR_SIZE];
extern char name[LOG_OUTPUT_STR_SIZE];

/* Global variable for Platform message storage to be used within protocols
 * and test agent
 */
extern struct arm_scmi_message received;

/*
 * Macro used to obtain the masked value (not normalised) from a uint32_t using
 * a mask with MSB and LSB.
 */
#define GET_BITS(d, u, l) (arm_scmi_get_norm_bits(d, u - l, 0) << (l))

struct precondition_data {
	enum test_status result;
	int status;
};

/*
 * Structure holding data related to protocol testing
 */
struct arm_scmi_protocol_execution_context {
	/* Array of test suites containing base address of each test suite */
	struct arm_scmi_test_suite *test_suites;
	/* Number of test suites in the array */
	int num_test_suites;
	/* Internal pointers to current testing test suite and test case */
	int test_suite_index;
	int test_case_counter;
	/* Pointer to function to fill expected return table, protocol wide */
	get_return_values_func_t get_return_values_func;
	/* Test protocol stats */
	struct arm_scmi_protocol_test_stats test_stats;
	/* Test protocol report */
	struct arm_scmi_protocol_report *protocol_report_table;
	/* Precondition status */
	struct precondition_data precondition_data;
};

#define EOT "END_OF_TEST_LIST"

void arm_scmi_init_protocol_execution_context(
		struct arm_scmi_protocol_execution_context *,
		get_return_values_func_t, struct arm_scmi_protocol_report *);
void arm_scmi_set_test_suite_table(
		struct arm_scmi_protocol_execution_context *,
		struct arm_scmi_test_suite *, size_t);
void arm_scmi_skip_test_suite(struct arm_scmi_protocol_execution_context *,
		int, bool);
int arm_scmi_execute_and_assert_test_case(
		struct arm_scmi_protocol_execution_context *, arm_scmi_test_case_t *,
		struct arm_scmi_message *);
bool arm_scmi_check_end_of_test_suite(
		struct arm_scmi_protocol_execution_context *);
bool arm_scmi_is_test_suite_skipped(
		struct arm_scmi_protocol_execution_context *, int);
void arm_scmi_info_status(struct arm_scmi_protocol_execution_context *,
		struct arm_scmi_message *);
uint32_t arm_scmi_get_norm_bits(uint32_t, uint8_t, uint8_t);
void arm_scmi_run(struct arm_scmi_protocol_execution_context *,
		struct arm_scmi_message *);
char *arm_scmi_status_code_str(int);

#endif /* TEST_EXECUTE_H_ */
