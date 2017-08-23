/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef PERFORMANCE_EXPORTS_H_
#define PERFORMANCE_EXPORTS_H_

#include <performance_expected.h>

struct arm_scmi_build_protocol_executor arm_scmi_performance_protocol_executor;

/* Function defined in the performance protocol executor */
extern void arm_scmi_performance_execute(const void *,
	struct arm_scmi_protocol_report **);

#endif /* PERFORMANCE_EXPORTS_H_ */
