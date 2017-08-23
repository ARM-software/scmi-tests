/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef SYSTEM_POWER_EXPORTS_H_
#define SYSTEM_POWER_EXPORTS_H_

#include <system_power_expected.h>

/*!
 * @brief Structure to be used to hold global data from the base protocol.
 *
 */
struct arm_scmi_system_global_data {
	uint32_t warm_reset_support;
	uint32_t system_suspend_support;
};

extern struct arm_scmi_build_protocol_executor
	arm_scmi_system_power_protocol_executor;

/* Function defined in the system power management protocol executor */
void arm_scmi_system_power_execute(const void *,
		struct arm_scmi_protocol_report **);

#endif /* SYSTEM_POWER_EXPORTS_H_ */
