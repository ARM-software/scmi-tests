/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef TEST_HELPER_H_
#define TEST_HELPER_H_

#include <test_log_stats.h>

#define ALL_BITS 0

void arm_scmi_check_and_report_dec(struct arm_scmi_protocol_test_stats *,
		uint32_t, uint32_t, uint32_t, uint32_t, const char *);
void arm_scmi_check_and_report_hex(struct arm_scmi_protocol_test_stats *,
		uint32_t, uint32_t, uint32_t, uint32_t, const char *);
void arm_scmi_check_and_report_str(struct arm_scmi_protocol_test_stats *,
		uint32_t, uint32_t, char *, char *, const char *);
bool arm_scmi_check_suite_skipped(struct arm_scmi_protocol_test_stats *,
		uint32_t, uint32_t, bool, uint32_t, uint32_t);
int arm_scmi_print_or_check(uint32_t, uint32_t);
bool arm_scmi_skip_return_values(int, int32_t);

#endif /* TEST_HELPER_H_ */
