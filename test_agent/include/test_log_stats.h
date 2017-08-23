/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef TEST_LOG_STATS_H_
#define TEST_LOG_STATS_H_

#include <common.h>

struct arm_scmi_stats_counters {
	int tests;
	int failures;
	int errors;
	int skips;
};

struct arm_scmi_protocol_test_stats {
	struct arm_scmi_test_suite_stats{
		const char *name;
		struct test_case_stats{
			const char *name;
			enum test_status result;
			struct arm_scmi_stats_counters number;
		} test_case;
		struct arm_scmi_stats_counters number;
	} test_suite;
	struct arm_scmi_stats_counters number;
	const char *name;
};

void arm_scmi_log(struct arm_scmi_protocol_test_stats *,
		const char *);
void arm_scmi_log_protocol_name(struct arm_scmi_protocol_test_stats *,
		const char *);
void arm_scmi_log_test_suite_start(struct arm_scmi_protocol_test_stats *,
		const char *);
void arm_scmi_log_test_case_start(struct arm_scmi_protocol_test_stats *,
		const char *);
void arm_scmi_log_test_result(struct arm_scmi_protocol_test_stats *, int,
		const char *, const char *);
void arm_scmi_log_test_case_end(struct arm_scmi_protocol_test_stats *);
void arm_scmi_log_test_suite_end(struct arm_scmi_protocol_test_stats *);
int arm_scmi_get_current_test_case_status(
		struct arm_scmi_protocol_test_stats *);

#endif /* TEST_LOG_STATS_H_ */
