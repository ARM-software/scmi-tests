/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef BASE_EXPORTS_H_
#define BASE_EXPORTS_H_

#include <base_expected.h>

/*!
 * @brief Structure to be used to hold global data from the base protocol.
 *
 */
struct arm_scmi_base_global_data {
	uint32_t num_agents;
	uint32_t num_protocols_implemented;
	uint32_t protocols[MAX_PROTOCOL_COUNT];
};

/* Function defined in the base protocol executor */
void arm_scmi_base_execute(const void *,
		struct arm_scmi_protocol_report **);

/* Global variable filled in the implementation but used in several modules */
extern struct arm_scmi_base_global_data arm_scmi_base_protocol_data;

#endif /* BASE_EXPORTS_H_ */
