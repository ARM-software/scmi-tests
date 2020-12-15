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

#ifndef _POWER_COMMON_H_
#define _POWER_COMMON_H_

#include <protocol_common.h>

struct arm_scmi_power_protocol_version {
    struct  {
        uint32_t version;
    } returns;
};

struct arm_scmi_power_protocol_attributes {
    struct {
        uint32_t attributes;
        uint32_t statistics_address_low;
        uint32_t statistics_address_high;
        uint32_t statistics_len;
    } returns;
    /* all the enums must be at the end */
    enum {
        POWER_PROT_ATTR_NUMBER_DOMAINS_LOW = 0,
        POWER_PROT_ATTR_NUMBER_DOMAINS_HIGH = 15,
        POWER_PROT_ATTR_PROT_RESERVED_LOW = 16,
        POWER_PROT_ATTR_PROT_RESERVED_HIGH = 31
    } attributes_bits;
};

struct arm_scmi_power_protocol_message_attributes {
    struct {
        uint32_t message_id;
    } parameters;
    struct {
        uint32_t attributes;
    } returns;
};

struct arm_scmi_power_domain_attributes {
    struct {
        uint32_t domain_id;
    } parameters;
    struct {
        uint32_t attributes;
        uint8_t name[SCMI_NAME_STR_SIZE];
    } returns;
    /* all the enums must be at the end */
    enum {
        POWER_DOMAIN_ATTR_RESERVED_LOW = 0,
        POWER_DOMAIN_ATTR_RESERVED_HIGH = 28,
        POWER_DOMAIN_ATTR_SYNC_LOW = 29,
        POWER_DOMAIN_ATTR_SYNC_HIGH = 29,
        POWER_DOMAIN_ATTR_ASYNC_LOW = 30,
        POWER_DOMAIN_ATTR_ASYNC_HIGH = 30,
        POWER_DOMAIN_ATTR_STATE_NOTIFICATIONS_LOW = 31,
        POWER_DOMAIN_ATTR_STATE_NOTIFICATIONS_HIGH = 31
    } attributes_bits;
};

struct arm_scmi_power_state_set {
    struct {
        uint32_t flags;
        uint32_t domain_id;
        uint32_t power_state;
    } parameters;
    /* all the enums must be at the end */
    enum {
        POWER_STATE_SET_RESERVED_LOW = 1,
        POWER_STATE_SET_RESERVED_HIGH = 31,
        POWER_STATE_SET_ASYNC_LOW = 0,
        POWER_STATE_SET_ASYNC_HIGH = 0
    } flags_bits;
};

struct arm_scmi_power_state_get {
    struct {
        uint32_t domain_id;
    } parameters;
    struct {
        uint32_t power_state;
    } returns;
};

struct arm_scmi_power_state_notify {
    struct {
        uint32_t domain_id;
        uint32_t notify_enable;
    } parameters;
    /* all the enums must be at the end */
    enum {
        POWER_STATE_NOTIFY_RESERVED_LOW = 1,
        POWER_STATE_NOTIFY_RESERVED_HIGH = 31,
        POWER_STATE_NOTIFY_ENABLE_LOW = 0,
        POWER_STATE_NOTIFY_ENABLE_HIGH = 0
    } power_notify_bits;
};

struct arm_scmi_power_state_change_requested_notify {
    struct {
        uint32_t domain_id;
        uint32_t notify_enable;
    } parameters;
    /* all the enums must be at the end */
    enum {
        POWER_STATE_CHANGE_REQ_NOTIFY_RESERVED_LOW = 1,
        POWER_STATE_CHANGE_REQ_NOTIFY_RESERVED_HIGH = 31,
        POWER_STATE_CHANGE_REQ_NOTIFY_ENABLE_LOW = 0,
        POWER_STATE_CHANGE_REQ_NOTIFY_ENABLE_HIGH = 0
    } power_notify_bits;
};


#endif /* POWER_COMMON_H_ */
