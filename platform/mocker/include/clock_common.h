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

#ifndef _CLOCK_COMMON_H_
#define _CLOCK_COMMON_H_

#include <protocol_common.h>

struct arm_scmi_clock_protocol_version {
    struct  {
        uint32_t version;
    } returns;
};

struct arm_scmi_clock_protocol_attributes {
    struct {
        uint32_t attributes;
    } returns;
    /* all the enums must be at the end */
    enum {
        CLK_NUMBER_CLOCKS_LOW = 0,
        CLK_NUMBER_CLOCKS_HIGH = 15,
        CLK_MAX_NUM_PENDING_ASYNC_RATE_CHGS_SPRT_LOW = 16,
        CLK_MAX_NUM_PENDING_ASYNC_RATE_CHGS_SPRT_HIGH = 23,
        CLK_RESERVED_LOW = 24,
        CLK_RESERVED_HIGH = 31
    }attributes_bits;
};

struct arm_scmi_clock_attributes {
    struct {
        uint32_t clock_id;
    } parameters;
    struct {
        uint32_t attributes;
        uint8_t clock_name[SCMI_NAME_STR_SIZE];
    } returns;
    /* enums for attributes */
    enum {
        CLK_ATTRIBUTES_ENABLED  =  0,
        CLK_ATTRIBUTES_RESERVED_LOW  =  1,
        CLK_ATTRIBUTES_RESERVED_HIGH  =  31
    } clk_attributes_bits;
};

struct arm_scmi_clock_protocol_message_attributes {
    struct {
        uint32_t message_id;
    } parameters;
    struct {
        uint32_t attributes;
    } returns;
};

struct arm_scmi_clock_describe_rates {
    struct {
        uint32_t clock_id;
        uint32_t rate_index;
    } parameters;
    struct {
        uint32_t num_rates_flags;
        uint32_t rates[2];
    } returns;
    /* enums for num_rates_flags */
    enum {
        CLK_DESC_RATE_NUM_RATES_RETURNED_LOW = 0,
        CLK_DESC_RATE_NUM_RATES_RETURNED_HIGH = 11,
        CLK_DESC_RATE_RETURN_FORMAT_LOW = 12,
        CLK_DESC_RATE_RETURN_FORMAT_HIGH = 12,
        CLK_DESC_RATE_RESERVED_LOW = 13,
        CLK_DESC_RATE_RESERVED_HIGH = 15,
        CLK_DESC_RATE_NUM_RATES_REMAINING_LOW = 16,
        CLK_DESC_RATE_NUM_RATES_REMAINING_HIGH = 31
    } num_rates_flags_bits;
};

struct arm_scmi_clock_rate_set {
    struct {
        uint32_t flags;
        uint32_t clock_id;
        uint32_t rate;
    } parameters;
    /* enums for clock rate set flag parameter */
    enum {
        CLK_RATESET_ASYNC_FLAG_LOW = 0,
        CLK_RATESET_ASYNC_FLAG_HIGH = 1,
        CLK_RATESET_IGNORE_DELAY_RSP_LOW = 1,
        CLK_RATESET_IGNORE_DELAY_RSP_HIGH = 1,
        CLK_RATESET_IGNORE_ROUNDDOWN_LOW = 2,
        CLK_RATESET_IGNORE_ROUNDDOWN_HIGH = 2,
        CLK_RATESET_RESERVED_LOW = 3,
        CLK_RATESET_RESERVED_HIGH = 31
    } rate_set_flags;
};

struct arm_scmi_clock_rate_get {
    struct {
        uint32_t clock_id;
    } parameters;
    struct {
        uint32_t rate;
    } returns;
};

struct arm_scmi_clock_config_set {
    struct {
        uint32_t clock_id;
        uint32_t attributes;
    } parameters;
    enum {
        CLK_CONFIGSET_ATTR_ENABLED = 0,
        CLK_CONFIGSET_ATTR_RESERVED_LOW = 1,
        CLK_CONFIGSET_ATTR_RESERVED_HIGH = 31,
    } config_set_flags;
};

#endif /* CLOCK_COMMON_H_ */
