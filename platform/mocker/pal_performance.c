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

#include <pal_interface.h>
#include <pal_performance_expected.h>
#include <performance_protocol.h>

/*-----------  Common PERFORMANCE API's across platforms ----------*/

uint32_t pal_performance_get_expected_num_domains(void)
{
    return num_performance_domains;
}

uint32_t pal_performance_get_expected_stats_addr_low(void)
{
    return statistics_address_low_perf;
}

uint32_t pal_performance_get_expected_stats_addr_high(void)
{
    return (statistics_address_low_perf + statistics_address_len_perf);
}

uint32_t pal_performance_get_expected_stats_addr_len(void)
{
    return statistics_address_len_perf;
}

uint32_t pal_performance_get_expected_command_fast_ch_support(uint32_t message_id)
{
    return perf_msg_fast_ch_support[message_id];
}

uint32_t pal_performance_get_expected_set_limit_support(uint32_t domain_id)
{
    return set_limit_capable[domain_id];
}

uint32_t pal_performance_get_expected_set_level_support(uint32_t domain_id)
{
    return set_performance_level_capable[domain_id];
}

uint32_t pal_performance_get_expected_level_notification_support(uint32_t domain_id)
{
    return performance_level_notification_support[domain_id];
}

uint32_t pal_performance_get_expected_limit_notification_support(uint32_t domain_id)
{
    return performance_limit_notification_support[domain_id];
}

uint32_t pal_performance_get_expected_fast_channel_support(uint32_t domain_id)
{
    return perf_domain_fast_ch_support[domain_id];
}

uint32_t pal_performance_get_expected_rate_limit(uint32_t domain_id)
{
    return performance_rate_limit[domain_id];
}

uint32_t pal_performance_get_expected_sustaind_freq(uint32_t domain_id)
{
    return performance_sustained_freq[domain_id];
}

uint32_t pal_performance_get_expected_sustaind_level(uint32_t domain_id)
{
    return performance_sustained_level[domain_id];
}

uint32_t pal_performance_get_expected_performance_level(uint32_t domain_id, uint32_t index)
{
    return perf_performance_level_values[domain_id][index];
}

uint32_t pal_performance_get_expected_power_cost(uint32_t domain_id, uint32_t index)
{
    return perf_performance_level_power_costs[domain_id][index];
}

uint32_t pal_performance_get_expected_latency(uint32_t domain_id, uint32_t index)
{
    return perf_performance_level_worst_latency[domain_id][index];
}

uint8_t* pal_performance_get_expected_name(uint32_t domain_id)
{
    return (uint8_t*)&performance_domain_names[domain_id][0];
}

uint32_t pal_performance_get_expected_number_of_level(uint32_t domain_id)
{
    return perf_num_performance_levels[domain_id];
}

uint8_t pal_performance_get_expected_fast_ch_doorbell_support(uint32_t domain_id,uint32_t message_id)
{
    if(message_id < PERF_LIMIT_SET_MSG_ID || message_id > PERF_LVL_GET_MSG_ID)
        return 0;

    return perf_performance_fast_ch_doorbell_support[domain_id][message_id-PERF_LIMIT_SET_MSG_ID];
}

uint8_t pal_performance_get_expected_fast_ch_doorbell_data_width(uint32_t domain_id,uint32_t message_id)
{
    if(message_id < PERF_LIMIT_SET_MSG_ID || message_id > PERF_LVL_GET_MSG_ID)
        return 0;

    return perf_performance_fast_ch_doorbell_data_width[domain_id][message_id-PERF_LIMIT_SET_MSG_ID];
}

uint32_t pal_performance_get_expected_fast_ch_addr_low(uint32_t domain_id,uint32_t message_id)
{
    if(message_id < PERF_LIMIT_SET_MSG_ID || message_id > PERF_LVL_GET_MSG_ID)
        return 0;

    return perf_performance_fast_chan_addr_low[domain_id][message_id-PERF_LIMIT_SET_MSG_ID];
}

uint32_t pal_performance_get_expected_fast_ch_addr_high(uint32_t domain_id,uint32_t message_id)
{
    if(message_id < PERF_LIMIT_SET_MSG_ID || message_id > PERF_LVL_GET_MSG_ID)
        return 0;

    return perf_performance_fast_chan_addr_high[domain_id][message_id-PERF_LIMIT_SET_MSG_ID];
}

uint32_t pal_performance_get_expected_fast_ch_size(uint32_t domain_id,uint32_t message_id)
{
    if(message_id < PERF_LIMIT_SET_MSG_ID || message_id > PERF_LVL_GET_MSG_ID)
        return 0;

    return perf_performance_fast_chan_size[domain_id][message_id-PERF_LIMIT_SET_MSG_ID];
}

uint32_t pal_performance_get_expected_fast_ch_rate_limit(uint32_t domain_id,uint32_t message_id)
{
    if(message_id < PERF_LIMIT_SET_MSG_ID || message_id > PERF_LVL_GET_MSG_ID)
        return 0;

    return perf_performance_fast_chan_rate_limit[domain_id][message_id-PERF_LIMIT_SET_MSG_ID];
}

uint32_t pal_performance_get_expected_fast_ch_doorbell_addr_low(uint32_t domain_id,uint32_t message_id)
{
    if(message_id < PERF_LIMIT_SET_MSG_ID || message_id > PERF_LVL_GET_MSG_ID)
        return 0;

    return perf_performance_fast_chan_doorbell_addr_low[domain_id][message_id-PERF_LIMIT_SET_MSG_ID];
}

uint32_t pal_performance_get_expected_fast_ch_doorbell_addr_high(uint32_t domain_id,uint32_t message_id)
{
    if(message_id < PERF_LIMIT_SET_MSG_ID || message_id > PERF_LVL_GET_MSG_ID)
        return 0;

    return perf_performance_fast_chan_doorbell_addr_high[domain_id][message_id-PERF_LIMIT_SET_MSG_ID];
}

uint32_t pal_performance_get_expected_fast_ch_doorbell_set_mask_low(uint32_t domain_id,uint32_t message_id)
{
    if(message_id < PERF_LIMIT_SET_MSG_ID || message_id > PERF_LVL_GET_MSG_ID)
        return 0;

    return perf_performance_fast_chan_doorbell_set_mask_low[domain_id][message_id-PERF_LIMIT_SET_MSG_ID];
}

uint32_t pal_performance_get_expected_fast_ch_doorbell_set_mask_high(uint32_t domain_id,uint32_t message_id)
{
    if(message_id < PERF_LIMIT_SET_MSG_ID || message_id > PERF_LVL_GET_MSG_ID)
        return 0;

    return perf_performance_fast_chan_doorbell_set_mask_high[domain_id][message_id-PERF_LIMIT_SET_MSG_ID];
}

uint32_t pal_performance_get_expected_fast_ch_doorbell_preserve_mask_low(uint32_t domain_id,uint32_t message_id)
{
    if(message_id < PERF_LIMIT_SET_MSG_ID || message_id > PERF_LVL_GET_MSG_ID)
        return 0;

    return perf_performance_fast_chan_doorbell_preserve_mask_low[domain_id][message_id-PERF_LIMIT_SET_MSG_ID];
}

uint32_t pal_performance_get_expected_fast_ch_doorbell_preserve_mask_high(uint32_t domain_id,uint32_t message_id)
{
    if(message_id < PERF_LIMIT_SET_MSG_ID || message_id > PERF_LVL_GET_MSG_ID)
        return 0;

    return perf_performance_fast_chan_doorbell_preserve_mask_high[domain_id][message_id-PERF_LIMIT_SET_MSG_ID];
}
