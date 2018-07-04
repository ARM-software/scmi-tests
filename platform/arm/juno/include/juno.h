/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef JUNO_H_
#define JUNO_H_

#include "test_executor.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <base_expected.h>
#include <platform_exports.h>
#include <test_executor.h>

#ifdef POWER_PROTOCOL
#include <power_expected.h>
#endif
#ifdef SYSTEM_POWER_PROTOCOL
#include <system_power_expected.h>
#endif
#ifdef PERFORMANCE_PROTOCOL
#include <performance_expected.h>
#endif
#ifdef CLOCK_PROTOCOL
#include <clock_expected.h>
#endif
#ifdef SENSOR_PROTOCOL
#include <sensor_expected.h>
#endif

#define VERSION_ENCODE32(MAJOR, MINOR, PATCH) (((MAJOR & 0xff) << 24) | \
                                                ((MINOR & 0xff) << 16)| \
                                                (PATCH & 0xffff))
#define LOG_FILE "arm_scmi_test_log.txt"

/*!
 * @brief Function used to create a log file.
 *
 */
void arm_scmi_create_log();

/* mailbox memory byte offset mapping */
enum {
	MB_RESERVED=0x0,
	MB_CHANNEL_STATUS=0x4,
	MB_RESERVED_IMPLEMENTATION_DEFINED=0x8,
	MB_FLAGS=0x10,
	MB_HEADER_PAYLOAD_LENGTH=0x14,
	MB_MESSAGE_HEADER=0x18,
	MB_MESSAGE_PAYLOAD=0x1c
} mailbox_byte_offset;
/* mailbox memory byte length mapping */
enum {
	MB_LENGTH_RESERVED=0x4,
	MB_LENGTH_CHANNEL_STATUS=0x4,
	MB_LENGTH_RESERVED_IMPLEMENTATION_DEFINED=0x8,
	MB_LENGTH_FLAGS=0x4,
	MB_LENGTH_HEADER_PAYLOAD_LENGTH=0x4,
	MB_LENGTH_MESSAGE_HEADER=0x4
} mailbox_byte_length;

enum power_domain_identifier {
    /* CPU power domains */
    BIG_CPU0,
    BIG_CPU1,
    LITTLE_CPU0,
    LITTLE_CPU1,
    LITTLE_CPU2,
    LITTLE_CPU3,
    /* Cluster power domains */
    BIG_SSTOP,
    LITTLE_SSTOP,
    /* Device power domains */
    DBGSYS,
    GPUTOP,
    POWER_DOMAIN_COUNT
};

enum juno_versions {
    JUNO_REV_R0=0,
    JUNO_REV_R1,
    JUNO_REV_R2
};

enum protocol_ids {
    PROTOCOL_ID_POWER=0x11,
    PROTOCOL_ID_SYSTEM_POWER=0x12,
    PROTOCOL_ID_PERF=0x13,
    PROTOCOL_ID_CLOCK=0x14,
    PROTOCOL_ID_SENSOR=0x15,
    PROTOCOL_ID_APCORE=0x90
};

static struct arm_scmi_base_expected ospm_base_expectations = {
    .flags_mask = (1 << BASE_PROTOCOL_VERSION) |
                  (1 << NUMBER_AGENTS) |
                  (1 << NUM_PROTOCOLS_IMPLEMENTED) |
                  (1 << VENDOR_IDENTIFIER) |
                  (1 << SUBVENDOR_IDENTIFIER) |
                  (1 << IMPLEMENTATION_VERSION) |
                  (1 << SUPPORTED_PROTOCOLS) |
                  (1 << AGENT_NAMES) |
                  (1 << ERROR_NOTIFICATION_SUPPORTED) |
                  (1 << DISCOVER_AGENT_CMD_SUPPORTED) |
                  (1 << SUBVENDOR_CMD_SUPPORTED),
    .protocol_version = 0x10000,
    .vendor_identifier = "ARM",
    .subvendor_identifier = "ARM",
    .implementation_version = VERSION_ENCODE32(1, 27, 0),
    .num_protocols_implemented = 6,
    .supported_protocols = (uint32_t[]) {
        PROTOCOL_ID_POWER,
        PROTOCOL_ID_SYSTEM_POWER,
        PROTOCOL_ID_PERF,
        PROTOCOL_ID_CLOCK,
        PROTOCOL_ID_SENSOR,
	PROTOCOL_ID_APCORE,
    },
    .num_agents = 2,
    .agent_names = (char *[]) { "OSPM", "PSCI" },
    .notify_error_cmd_supported = false,
    .subvendor_cmd_supported = true,
    .discover_agent_cmd_supported = true
};

#endif /* JUNO_H_ */


