/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef SENSOR_EXPORTS_H_
#define SENSOR_EXPORTS_H_

#include <sensor_expected.h>

struct arm_scmi_build_protocol_executor arm_scmi_sensor_protocol_executor;

/* Function defined in the sensor protocol executor */
extern void arm_scmi_sensor_execute(const void *,
	struct arm_scmi_protocol_report **);

#endif /* SENSOR_EXPORTS_H_ */
