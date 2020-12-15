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

#ifndef _BASE_COMMON_H_
#define _BASE_COMMON_H_

#include <protocol_common.h>

struct arm_scmi_base_protocol_version {
    struct  {
        uint32_t version;
    } returns;
};

struct arm_scmi_base_protocol_attributes {
    struct {
        uint32_t attributes;
    } returns;
    /* all the enums must be at the end */
    enum {
        BASE_NUMBER_PROTOCOLS_LOW = 0,
        BASE_NUMBER_PROTOCOLS_HIGH = 7,
        BASE_NUMBER_AGENTS_LOW = 8,
        BASE_NUMBER_AGENTS_HIGH = 15,
        BASE_RESERVED_LOW = 16,
        BASE_RESERVED_HIGH = 31
    }attributes_bits;
};

struct arm_scmi_base_protocol_message_attributes {
    struct {
        uint32_t message_id;
    } parameters;
    struct {
        uint32_t attributes;
    } returns;
};

struct arm_scmi_base_discover_vendor {
    struct {
        uint8_t vendor_identifier[SCMI_NAME_STR_SIZE];
    } returns;
};

struct arm_scmi_base_discover_subvendor {
    struct {
        uint8_t vendor_identifier[SCMI_NAME_STR_SIZE];
    } returns;
};

struct arm_scmi_base_discover_implementation_version {
    struct {
        uint32_t implementation_version;
    } returns;
};

struct arm_scmi_base_discover_list_protocols {
    struct {
        uint32_t skip;
    } parameters;
    struct {
        uint32_t num_protocols;
        /* 'protocols' is a dynamic array is fine for offset as long there is
         * no more return value after it. If there are more return values then
         * the size must be determined run-time.
         */
        uint32_t protocols;
    } returns;
};

struct arm_scmi_base_discover_agent {
    struct {
        uint32_t agent_id;
    } parameters;
    struct {
        uint32_t ret_agent_id;
        uint8_t name[SCMI_NAME_STR_SIZE];
    } returns;
};

struct arm_scmi_base_notify_error {
    struct {
        uint32_t notify_enable;
    } parameters;
    enum {
        NOTIFY_ENABLE = 0,
        NOTIFY_ENABLE_RESERVED_LOW = 1,
        NOTIFY_ENABLE_RESERVED_HIGH = 31
    } notify_enable_bits;
};

struct arm_scmi_base_set_device_permissions {
    struct {
        uint32_t agent_id;
        uint32_t device_id;
        uint32_t flags;
    } parameters;
    enum {
        DEVICE_PERMISSIONS_ACCESS_TYPE = 0,
        DEVICE_PERMISSIONS_RESERVED_LOW = 1,
        DEVICE_PERMISSIONS_RESERVED_HIGH = 31
    } flags_bits;
};

struct arm_scmi_base_set_protocol_permissions {
    struct {
        uint32_t agent_id;
        uint32_t device_id;
        uint32_t command_id;
        uint32_t flags;
    } parameters;
    enum {
        PROTOCOLS_PERMISSIONS_ACCESS_TYPE = 0,
        PROTOCOLS_PERMISSIONS_RESERVED_LOW = 1,
        PROTOCOLS_PERMISSIONS_RESERVED_HIGH = 31
    } flags_bits;
    enum {
        PROTOCOL_ID_TYPE_LOW = 0,
        PROTOCOL_ID_TYPE_HIGH = 7,
        PROTOCOL_PERMISSIONS_ID_LOW = 8,
        PROTOCOL_PERMISSIONS_ID_HIGH = 31
    } command_id_bits;
};

struct arm_scmi_base_reset_agent_configuration {
    struct {
        uint32_t agent_id;
        uint32_t flags;
    } parameters;
    enum {
        PERMISSIONS_RESET_TYPE = 0,
        RESET_AGENT_CONFIG_RESERVED_LOW = 1,
        RESET_AGENT_CONFIG_RESERVED_HIGH = 31
    } flags_bits;
};

#endif /*_BASE_COMMON_H_ */
