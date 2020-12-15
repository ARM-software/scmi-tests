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
#ifdef PERFORMANCE_PROTOCOL

#include "pal_interface.h"
#include "pal_performance_expected.h"

/**
  @brief   This API is used for checking number of perf domains
  @param   none
  @return  num of perf domains
**/
uint32_t  pal_performance_get_expected_num_domains(void)
{
    return num_perf_domains;
}

/**
  @brief   This API is used for checking perf statistics addr low
  @param   none
  @return  statistics addr low
**/
uint32_t  pal_performance_get_expected_stats_addr_low(void)
{
    return statistics_address_low_perf;
}

/**
  @brief   This API is used for checking perf statistics addr high
  @param   none
  @return  statistics addr high
**/
uint32_t  pal_performance_get_expected_stats_addr_high(void)
{
    return (statistics_address_low_perf + statistics_address_len_perf);
}

/**
  @brief   This API is used for checking statistics addr len
  @param   none
  @return  statistics addr len
**/
uint32_t  pal_performance_get_expected_stats_addr_len(void)
{
    return statistics_address_len_perf;
}

/**
  @brief   This API is used for checking domain name
  @param   perf domain id
  @return  perf domain name
**/
uint8_t *pal_performance_get_expected_name(uint32_t domain_id)
{
    return (uint8_t *)&performance_domain_names[domain_id][0];
}

/**
  @brief   This API is used for checking num of level for perf domain
  @param   perf domain id
  @return  num of level for perf domain
**/
uint32_t  pal_performance_get_expected_number_of_level(uint32_t domain_id)
{
    return perf_num_performance_levels[domain_id];
}

#endif
