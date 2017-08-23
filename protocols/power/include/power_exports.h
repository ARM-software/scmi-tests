/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef POWER_EXPORTS_H_
#define POWER_EXPORTS_H_

#include <power_expected.h>

extern struct arm_scmi_build_protocol_executor arm_scmi_power_protocol_executor;

/* Function defined in the power protocol executor */
extern void arm_scmi_power_execute(const void *,
		struct arm_scmi_protocol_report **);

#endif /* POWER_EXPORTS_H_ */
