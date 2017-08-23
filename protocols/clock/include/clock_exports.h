/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef CLOCK_EXPORTS_H_
#define CLOCK_EXPORTS_H_

#include <clock_expected.h>

struct arm_scmi_build_protocol_executor arm_scmi_clock_protocol_executor;

/* Function defined in the clock protocol executor */
extern void arm_scmi_clock_execute(const void *,
	struct arm_scmi_protocol_report **);

#endif /* CLOCK_EXPORTS_H_ */
