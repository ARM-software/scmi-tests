/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef TEST_AGENT_EXPORTS_H_
#define TEST_AGENT_EXPORTS_H_

#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>
#include <arm_scmi_constants.h>

/*
 * Test results structures passed as return value from executing test agent
 * sequence.
 */
struct arm_scmi_test_case_report {
	const char *test_case_name;
	int test_status;
};

struct arm_scmi_test_suite_report {
	char *test_suite_name;
	size_t test_case_count;
	bool skip;
	struct arm_scmi_test_case_report *test_case_report_table;
};

struct arm_scmi_protocol_report {
	unsigned int protocol_id;
	size_t test_suite_report_size;
	struct arm_scmi_test_suite_report *test_suite_report_table;
};

struct arm_scmi_test_report {
	size_t protocol_report_count;
	struct arm_scmi_protocol_report *protocol_report_table[MAX_PROTOCOL_COUNT];
};

/*
 * Structure to pass information from the platform to arm_scmi_agent_execute
 */
struct arm_scmi_platform_protocol_info {
	unsigned int protocol_id;
	const void *expected_return_values;
};

struct arm_scmi_platform_info {
	size_t platform_protocol_info_count;
	struct arm_scmi_platform_protocol_info *platform_protocol_info_table;
};

typedef void (*protocol_executor_t)(const void *,
	struct arm_scmi_protocol_report **);

struct arm_scmi_build_protocol_executor {
	unsigned int protocol_id;
	protocol_executor_t protocol_executor;
};

int arm_scmi_agent_execute(
		const struct arm_scmi_platform_info *, const uint32_t*,
		struct arm_scmi_test_report *);

#endif /* TEST_AGENT_EXPORTS_H_ */
