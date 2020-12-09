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

#ifndef _BASE_PROTOCOL_H_
#define _BASE_PROTOCOL_H_

#include <inttypes.h>
#include <stdbool.h>

#ifdef SCMI_VERSION_1
#define BASE_VERSION 0x00010000
#else
#define BASE_VERSION 0x00020000
#endif

#define BASE_PROTO_ID                           0x10
#define BASE_PROTO_VERSION_MSG_ID               0x0
#define BASE_PROTO_ATTR_MSG_ID                  0x1
#define BASE_PROTO_MSG_ATTR_MSG_ID              0x2
#define BASE_DISC_VENDOR_MSG_ID                 0x3
#define BASE_DISC_SUBVENDOR_MSG_ID              0x4
#define BASE_DISC_IMPLEMENTATION_VERSION_MSG_ID 0x5
#define BASE_DISC_PROTO_LIST_MSG_ID             0x6
#define BASE_DISC_AGENT_MSG_ID                  0x7
#define BASE_NOTIFY_ERRORS_MSG_ID               0x8
#define BASE_SET_DEVICE_PERMISSIONS_MSG_ID      0x9
#define BASE_SET_PROTOCOL_PERMISSIONS_MSG_ID    0xA
#define BASE_RESET_AGENT_CONFIGURATION_MSG_ID   0xB

#define VENDOR_ID_SIZE      16
#define SUB_VENDOR_ID_SIZE  16

struct device_access_info
{
    char *name;
    bool device_access;
    uint32_t device_id;
};

struct device_protocol_access_info
{
    uint32_t protocol_id;
    bool protocol_access;
};

struct arm_scmi_base_protocol {
    /*
     * This enum specifies the bit position in the flags_mask field
     * indicating if the expected value is given (bit set) or ignored
     * (bit unset).
     */
    enum {
        BASE_PROTOCOL_VERSION,
        VENDOR_IDENTIFIER,
        SUBVENDOR_IDENTIFIER,
        IMPLEMENTATION_VERSION,
        NUM_PROTOCOLS_IMPLEMENTED,
        SUPPORTED_PROTOCOLS,
        NUMBER_AGENTS,
        AGENT_NAMES,
        ERROR_NOTIFICATION_SUPPORTED,
        SUBVENDOR_CMD_SUPPORTED,
        DISCOVER_AGENT_CMD_SUPPORTED
    } FLAGS;
    /*
     * The expectation is that there is no more
     * than 32 elements specified via this expected results data structure.
     * This mask is used to indicate which of the structure members hold valid
     * data.
     */
    uint32_t protocol_version;
    /* The name of the vendor as a string */
    char vendor_identifier[VENDOR_ID_SIZE];
    /* The name of the sub vendor as a string */
    char subvendor_identifier[SUB_VENDOR_ID_SIZE];
    uint32_t implementation_version;
    uint32_t num_protocols_implemented;
    /*
     * This should have the list of protocol_ids for the commands expected to be
     * supported in this platform limited by the num_protocols_implemented
     * variable.
     */
    uint32_t *supported_protocols;
    uint32_t num_agents;
    /*
     * This should have the list of agent names for the all the agents in the
     * platform limited by the num_agents variable, if agent_discoverable is
     * set to non-zero value
     */
    char **agent_names;
    /*
     * This specifies if the error notification is expected to be supported in
     * the platform.
     */
    bool notify_error_cmd_supported;
    /*
     * This specifies if the sub vendor command is expected to be supported in
     * the platform.
     */
    bool subvendor_cmd_supported;
    /*
     * This specifies if the discover agent command is expected to be supported
     * in the platform.
     */
    bool discover_agent_cmd_supported;
    /*
     * This specifies if the device permissions command is expected to be supported in
     * the platform.
     */
    bool device_permissions_cmd_supported;
    /*
     * This specifies if the protocol permissions command is expected to be supported
     * in the platform.
     */
    bool protocol_permissions_cmd_supported;
    /*
     * This specifies if the reset agent configuration command is expected to be supported in
     * the platform.
     */
    bool reset_agent_config_cmd_supported;
};

#endif /* BASE_PROTOCOL_H_ */
