/** @file
 * Copyright (c) 2020, Arm Limited or its affiliates. All rights reserved.
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

#ifndef RESET_COMMON_H_
#define RESET_COMMON_H_

#include <protocol_common.h>

/* Structure for reset protocol version*/
struct arm_scmi_reset_protocol_version {
    struct  {
        uint32_t version;
    } returns;
};

/* Structure for reset protocol attributes*/
struct arm_scmi_reset_protocol_attributes {
    struct {
        uint32_t attributes;
    } returns;
    /* all the enums must be at the end */
    enum {
        RESET_NUMBER_DOMAIN_LOW = 0,
        RESET_NUMBER_DOMAIN_HIGH = 15,
    } attributes_bits;
};

/* Structure for reset message attributes*/
struct arm_scmi_reset_protocol_message_attributes {
    struct {
        uint32_t message_id;
    } parameters;
    struct {
        uint32_t attributes;
    } returns;
};

/* Structure for reset domain attributes*/
struct arm_scmi_reset_protocol_domain_attributes {
    struct {
        uint32_t domain_id;
    } parameters;
    struct {
        uint32_t attributes;
        uint32_t latency;
        char domain_name[SCMI_NAME_STR_SIZE];
    } returns;

    /* all the enums must be at the end */
     enum {
         RESET_ASYNC_SUPPORT_BIT = 31,
         RESET_NOTIFY_SUPPORT_BIT = 30
     } attributes_bits;
};

/* Structure for reset command*/
struct arm_scmi_reset_protocol_reset {
    struct {
        uint32_t domain_id;
        uint32_t flags;
        uint32_t reset_state;
    } parameters;

    /* all the enums must be at the end */
     enum {
         RESET_FLAG_ASYNC_RESET_BIT = 2,
         RESET_FLAG_EXPLICIT_SIGNAL_BIT = 1,
         RESET_FLAG_AUTONOMOUS_RESET_BIT = 0
     } flags_bits;
};

/* Structure for notify enable command*/
struct arm_scmi_reset_protocol_notify_enable {
    struct {
        uint32_t domain_id;
        uint32_t notify_enable;
    } parameters;

    /* all the enums must be at the end */
     enum {
         RESET_NOTIFY_ENABLE_BIT = 0
     } notify_en_bits;
};

#endif /* RESET_COMMON_H_ */
