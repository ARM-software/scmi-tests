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

#ifdef POWER_DOMAIN_PROTOCOL

#include "pal_interface.h"
#include "pal_power_domain_expected.h"

extern void pal_power_set_expected(const void *);
static struct arm_scmi_power_expected *g_power_info;

/**
  @brief   Sets power domain protocol info
  @param   platform power domain protocol info
  @return  none
**/
void pal_power_set_expected(const void *info)
{
    if (info == NULL)
        return;
    g_power_info = (struct arm_scmi_power_expected *)info;
}

/*-----------  Common PAL API's across platforms ----------*/

/**
  @brief   This API is used for checking num of power domain
  @param   none
  @return  num of power domain
**/
uint32_t pal_power_get_expected_num_domains(void)
{
    return g_power_info->num_power_domains;
}

/**
  @brief   This API is used for checking power domain statistics addr low
  @param   none
  @return  statistics addr low
**/
uint32_t pal_power_get_expected_stats_addr_low(void)
{
    return g_power_info->statistics_address_low;
}

/**
  @brief   This API is used for checking power domain statistics addr high
  @param   none
  @return  statistics addr high
**/
uint32_t pal_power_get_expected_stats_addr_high(void)
{
    return g_power_info->statistics_address_high;
}

/**
  @brief   This API is used for checking power domain statistics addr len
  @param   none
  @return  statistics addr len
**/
uint32_t pal_power_get_expected_stats_addr_len(void)
{
    return g_power_info->statistics_length;
}

#endif
