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

#ifndef _SYSTEM_POWER_COMMON_H_
#define _SYSTEM_POWER_COMMON_H_

#include <protocol_common.h>

struct arm_scmi_system_power_protocol_version {
    struct  {
        uint32_t version;
    } returns;
};

struct arm_scmi_system_power_protocol_attributes {
    struct {
        uint32_t attributes;
    } returns;
    /* all the enums must be at the end */
    enum {
        SYSTEM_POWER_PROTOCOLS_ATTR_RESERVED_LOW = 0,
        SYSTEM_POWER_PROTOCOLS_ATTR_RESERVED_HIGH = 31
    }attributes_bits;
};

struct arm_scmi_system_power_protocol_message_attributes {
    struct {
        uint32_t message_id;
    } parameters;
    struct {
        uint32_t attributes;
    } returns;
    enum {
        SYSTEM_POWER_MSG_PROTO_ATTRIB_RESERVED_LOW = 0,
        SYSTEM_POWER_MSG_PROTO_ATTRIB_RESERVED_HIGH = 29,
        SYSTEM_POWER_MSG_PROTO_ATTRIB_SUSPEND = 30,
        SYSTEM_POWER_MSG_PROTO_ATTRIB_WARMREST = 31
    } system_power_message_attributes_bits;
};

struct arm_scmi_system_power_state_set {
    struct {
        uint32_t flags;
        uint32_t system_state;
    } parameters;
    enum {
        SYSTEM_POWER_REQUEST_TYPE_LOW = 0,
        SYSTEM_POWER_REQUEST_TYPE_HIGH = 0,
        SYSTEM_POWER_REQUEST_RESERVED_LOW = 1,
        SYSTEM_POWER_REQUEST_RESERVED_HIGH = 31
    } system_power_request_flags;
};

#endif /* SYSTEM_POWER_COMMON_H_ */
