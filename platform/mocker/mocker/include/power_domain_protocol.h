/** @file
 * Copyright (c) 2019, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#ifndef _POWER_PROTOCOL_H_
#define _POWER_PROTOCOL_H_

#include <inttypes.h>
#include <stdbool.h>

#ifdef SCMI_VERSION_1
#define POWER_DOMAIN_VERSION 0x00010000
#else
#define POWER_DOMAIN_VERSION 0x00020000
#endif

#define PWR_PROTO_ID                                0x11
#define PWR_PROTO_VER_MSG_ID                        0x0
#define PWR_PROTO_ATTR_MSG_ID                       0x1
#define PWR_PROTO_MSG_ATTR_MSG_ID                   0x2
#define PWR_DOMAIN_ATTRIB_MSG_ID                    0x3
#define PWR_STATE_SET_MSG_ID                        0x4
#define PWR_STATE_GET_MSG_ID                        0x5
#define PWR_STATE_NOTIFY_MSG_ID                     0x6
#define PWR_STATE_CHANGE_REQUESTED_NOTIFY_MSG_ID    0x7

#define PWR_NOTIFY_ENABLE_SET       1
#define PWR_NOTIFY_ENABLE_UNSET     0

struct arm_scmi_power_protocol {
    /*
     * This enum specifies the bit position in the flags_mask field
     * indicating if the expected value is given (bit set) or ignored
     * (bit unset).
     */
    enum {
        POWER_PROTOCOL_VERSION,
        STATISTICS_ADDRESS_LOW,
        STATISTICS_ADDRESS_HIGH,
        STATISTICS_LEN,
        NUM_POWER_DOMAINS,
        POWER_STATE_NOTIFICATION_SUPPORT,
        POWER_SYNCHRONOUS_SUPPORT,
        POWER_ASYNCHRONOUS_SUPPORT,
        POWER_DOMAIN_NAMES,
        POWER_STATE_NOTIFY_CMD_SUPPORTED
    } FLAGS;
    /*
     * The expectation is that there is no more
     * than 32 elements specified via this expected results data structure.
     * This mask is used to indicate which of the structure members hold valid
     * data.
     */
    uint32_t protocol_version;
    /* Relevant only when statistics supported is set as True */
    uint32_t statistics_address_low;
    uint32_t statistics_address_high;
    uint32_t statistics_length;
    uint32_t num_power_domains;
    /*This should have notification support info for each power domain in this
     * platform bounded by the num_power_domains variable*/
    uint8_t *power_state_notification_support;
    /* This should have power synchronous support info for each power domain in
     * this platform bounded by the num_power_domains variable */
    uint8_t *power_synchronous_support;
    /* This should have power asynchronous support info for each power domain in
     * this platform bounded by the num_power_domains variable */
    uint8_t *power_asynchronous_support;
    /* This should have the list of power domain names in the platform bounded
     * by the num_power_domains variable */
    char **power_domain_names;
    /* This specifies if the power state notify command is expected to be
     * supported in the platform or not available to the agent */
    bool power_state_notify_cmd_supported;
    bool power_state_change_requested_notify_cmd_supported;
};

#endif /* POWER_PROTOCOL_H_ */
