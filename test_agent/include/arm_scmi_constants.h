/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef ARM_SCMI_CONSTANTS_H_
#define ARM_SCMI_CONSTANTS_H_

/* Frequently used constants */
#define ID_STR_SIZE 164
#define LOG_OUTPUT_STR_SIZE 96
#define MAX_PARAMETERS_SIZE 5
#define MAX_RETURNS_SIZE 32
#define NO_INVALID_PARAM 0
#define MAX_PROTOCOL_COUNT 16
#define MAX_TEST_SUITES 10
#define MAX_TEST_CASES 16
#define MAX_POWER_DOMAIN_COUNT 16
#define ALL_BITS_SET ~((uint32_t) 0)
#define SCMI_NAME_STR_SIZE 16

/* Base Protocol */
#define BASE_PROTO_ID 0x10

#endif /* ARM_SCMI_CONSTANTS_H_ */
