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

#ifndef PERFORMANCE_COMMON_H_
#define PERFORMANCE_COMMON_H_

#include <protocol_common.h>

struct arm_scmi_performance_protocol_version {
    struct  {
        uint32_t version;
    } returns;
};
struct arm_scmi_performance_protocol_attributes {
    struct {
        uint32_t attributes;
        uint32_t statistics_address_low;
        uint32_t statistics_address_high;
        uint32_t statistics_len;
    } returns;
    /* all the enums must be at the end */
    enum  {
        PERFORMANCE_PROT_ATTR_NUMBER_DOMAINS_LOW = 0,
        PERFORMANCE_PROT_ATTR_NUMBER_DOMAINS_HIGH = 15,
        PERFORMANCE_PROT_POWER_VALUES_LOW = 16,
        PERFORMANCE_PROT_POWER_VALUES_HIGH = 16,
        PERFORMANCE_PROT_ATTR_PROT_RESERVED_LOW = 17,
        PERFORMANCE_PROT_ATTR_PROT_RESERVED_HIGH = 31
    }arm_scmi_performance_protocol_attributes_bits;
};
struct arm_scmi_performance_protocol_message_attributes {
    struct {
        uint32_t message_id;
    } parameters;
    struct {
        uint32_t attributes;
    } returns;
    /* all the enums must be at the end */
    enum  {
        PERFORMANCE_PROT_MSG_ATTR_FASTCHANNEL_ENABLED = 0,
        PERFORMANCE_PROT_MSG_ATTR_RESERVED_LOW = 1,
        PERFORMANCE_PROT_MSG_ATTR_RESERVED_HIGH = 31
    }arm_scmi_performance_domain_attributes_bit;
};
struct arm_scmi_performance_domain_attributes {
    struct {
        uint32_t domain_id;
    } parameters;
    struct {
        uint32_t attributes;
        uint32_t rate_limit;
        uint32_t sustained_freq;
        uint32_t sustained_perf_level;
        uint8_t name[SCMI_NAME_STR_SIZE];
    } returns;
    enum {
        PERFORMANCE_DOMAIN_ATTR_LIMIT_SET = 31,
        PERFORMANCE_DOMAIN_ATTR_LEVEL_SET = 30,
        PERFORMANCE_DOMAIN_ATTR_LIMIT_NOTIFY = 29,
        PERFORMANCE_DOMAIN_ATTR_LEVEL_NOTIFY = 28,
        PERFORMANCE_DOMAIN_ATTR_FASTCHANNEL_SUPPORT = 27,
        PERFORMANCE_DOMAIN_ATTR_RESERVED_LOW = 0,
        PERFORMANCE_DOMAIN_ATTR_RESERVED_HIGH = 26,
        PERFORMANCE_DOMAIN_ATTR_RATE_LIMIT_LOW = 0,
        PERFORMANCE_DOMAIN_ATTR_RATE_LIMIT_HIGH = 19,
        PERFORMANCE_DOMAIN_ATTR_RATE_RESERVED_LOW = 20,
        PERFORMANCE_DOMAIN_ATTR_RATE_RESERVED_HIGH = 31
    }arm_scmi_performance_domain_attributes_bits;
};
struct arm_scmi_performance_describe_levels {
    struct {
        uint32_t domain_id;
        uint32_t level_index;
    } parameters;
    struct {
        uint32_t num_levels;
        struct arm_scmi_perf_levels {
            uint32_t performance_level_value;
            uint32_t power_cost;
            uint32_t attributtes;
        } perf_levels;
    } returns;
    enum {
        PERFORMANCE_DESC_LEVELS_NUM_LEVELS_REMAINING_LOW = 16,
        PERFORMANCE_DESC_LEVELS_NUM_LEVELS_REMAINING_HIGH = 31,
        PERFORMANCE_DESC_LEVELS_NUM_LEVELS_RESERVED_LOW = 12,
        PERFORMANCE_DESC_LEVELS_NUM_LEVELS_RESERVED_HIGH = 15,
        PERFORMANCE_DESC_LEVELS_NUM_LEVELS_LOW = 0,
        PERFORMANCE_DESC_LEVELS_NUM_LEVELS_HIGH = 11,
        PERFORMANCE_DESC_LEVELS_ATTR_TRANSITION_LOW = 0,
        PERFORMANCE_DESC_LEVELS_ATTR_TRANSITION_HIGH = 15,
        PERFORMANCE_DESC_LEVELS_ATTR_RESERVED_LOW = 16,
        PERFORMANCE_DESC_LEVELS_ATTR_RESERVED_HIGH = 31
    }arm_scmi_performance_describe_levels_bits;
};
struct arm_scmi_performance_limits_set {
    struct {
        uint32_t domain_id;
        uint32_t range_max;
        uint32_t range_min;
    } parameters;
};
struct arm_scmi_performance_limits_get {
    struct {
        uint32_t domain_id;
    } parameters;
    struct {
        uint32_t range_max;
        uint32_t range_min;
    } returns;
};
struct arm_scmi_performance_levels_set {
    struct {
        uint32_t domain_id;
        uint32_t performance_level;
    } parameters;
};
struct arm_scmi_performance_levels_get {
    struct {
        uint32_t domain_id;
    } parameters;
    struct {
        uint32_t performance_level;
    } returns;
};

#endif /* PERFORMANCE_COMMON_H_ */
