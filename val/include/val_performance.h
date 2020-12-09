/** @file
 * Copyright (c) 2019-2020, Arm Limited or its affiliates. All rights reserved.
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

#ifndef __VAL_PERFORMANCE_H__
#define __VAL_PERFORMANCE_H__

#define MAX_PERFORMANCE_DOMAINS                16
#define MAX_PERFORMANCE_LEVELS                 16

#define NOT_SUPPORTED                          0
#define VERSION_OFFSET                         0
#define ATTRIBUTE_OFFSET                       0
#define STATS_ADDR_LOW_OFFSET                  1
#define STATS_ADDR_HIGH_OFFSET                 2
#define STATS_LENGTH_OFFSET                    3
#define RATE_LIMIT_OFFSET                      1
#define FREQUENCY_OFFSET                       2
#define SUSTAINED_LEVEL_OFFSET                 3
#define NAME_OFFSET                            4
#define NUM_LEVEL_OFFSET                       0
#define PERF_LEVEL_ARRAY_OFFSET                1
#define RANGE_MAX_OFFSET                       0
#define RANGE_MIN_OFFSET                       1
#define PERFORMANCE_LEVEL_OFFSET               0
#define FAST_CH_RATE_LIMIT_OFFSET              1
#define CHAN_ADDR_LOW_OFFSET                   2
#define CHAN_ADDR_HIGH_OFFSET                  3
#define CHAN_SIZE_OFFSET                       4
#define DOORBELL_ADDR_LOW_OFFSET               5
#define DOORBELL_ADDR_HIGH_OFFSET              6
#define DOORBELL_MASK_LOW_OFFSET               7
#define DOORBELL_MASK_HIGH_OFFSET              8
#define DOORBELL_PRESERVE_LOW_OFFSET           9
#define DOORBELL_PRESERVE_HIGH_OFFSET          10

#define NOTIFICATION_DOMAIN_ID_OFFSET          1
#define NOTIFICATION_RANGE_MAX_OFFSET          2
#define NOTIFICATION_RANGE_MIN_OFFSET          3
#define NOTIFICATION_LEVEL_OFFSET              2

#define INVALID_NOTIFY_EN_VAL                  0xFF
#define NOTIFY_ENABLE                          1
#define NOTIFY_DISABLE                         0

typedef struct {
    uint8_t  performance_limits_set;
    uint8_t  performance_limits_get;
    uint8_t  performance_level_set;
    uint8_t  performance_level_get;
} PERFORMANCE_CMD_FAST_CH_SUPPORT_s;

typedef struct {
    uint8_t   name[SCMI_NAME_STR_SIZE];
    uint8_t   fast_ch_support;
    uint8_t   Perf_lvl_change_ntfy_support;
    uint8_t   Perf_limit_change_ntfy_support;
    uint8_t   set_performance_level_support;
    uint8_t   set_performance_limit_support;
    uint32_t  maximum_level;
    uint32_t  minimum_level;
    uint32_t  intermediate_level;
} PERFORMANCE_DOMAIN_INFO_s;

typedef struct {
    uint32_t num_perf_domains;
    uint32_t perf_stats_addr_low;
    uint8_t  perf_stats_addr_high;
    uint8_t  perf_stats_addr_len;
    PERFORMANCE_CMD_FAST_CH_SUPPORT_s perf_fast_cmd_ch_support;
    PERFORMANCE_DOMAIN_INFO_s perf_domain_info[MAX_PERFORMANCE_DOMAINS];
} PERFORMANCE_INFO_s;

/* Common Tests */
uint32_t performance_query_protocol_version(uint32_t *version);
uint32_t performance_query_protocol_attributes(void);
uint32_t performance_query_describe_levels(void);
uint32_t performance_query_set_limit(void);
uint32_t performance_query_set_level(void);
uint32_t performance_invalid_messageid_call(void);
uint32_t performance_query_domain_attributes_invalid_domain(void);
uint32_t performance_query_describe_levels_invalid_domain(void);
uint32_t performance_query_set_limit_invalid_range(void);
uint32_t performance_query_set_limit_invalid_domain(void);
uint32_t performance_query_get_limit_invalid_domain(void);
uint32_t performance_query_set_level_invalid_domain(void);
uint32_t performance_query_set_level_invalid_range(void);
uint32_t performance_query_get_level_invalid_domain(void);
uint32_t performance_query_notify_level_invalid_domain(void);
uint32_t performance_query_notify_level_invalid_parameters(void);
uint32_t performance_query_notify_limit_invalid_domain(void);
uint32_t performance_query_notify_limit_invalid_parameters(void);
uint32_t performance_limit_set_async(void);
uint32_t performance_level_set_async(void);

/* V1 Tests */
uint32_t performance_query_mandatory_command_support_v1(void);
uint32_t performance_query_domain_attributes_v1(void);

/* V2 Tests */
uint32_t performance_query_mandatory_command_support(void);
uint32_t performance_query_domain_attributes(void);
uint32_t performance_query_describe_fast_channel(void);
uint32_t performance_query_describe_fast_channel_invalid_domain(void);
uint32_t performance_query_describe_fast_channel_invalid_message(void);
uint32_t performance_query_describe_fast_channel_unsupported_domain(void);
uint32_t performance_query_describe_fast_channel_unsupported_message(void);
uint32_t performance_level_get_fast_channel(void);
uint32_t performance_limits_get_fast_channel(void);

/* Get expected*/
uint32_t val_performance_get_expected_num_domains(void);
uint32_t val_performance_get_expected_stats_addr_low(void);
uint32_t val_performance_get_expected_stats_addr_high(void);
uint32_t val_performance_get_expected_stats_addr_len(void);
uint8_t *val_performance_get_expected_name(uint32_t domain_id);
uint32_t val_performance_get_expected_number_of_level(uint32_t domain_id);

#endif
