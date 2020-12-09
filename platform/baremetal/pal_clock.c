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

#ifdef CLOCK_PROTOCOL

#include "pal_interface.h"
#include "pal_clock_expected.h"

extern void pal_clock_set_expected(const void *);
static struct arm_scmi_clock_expected *g_clock_info;

/**
  @brief   Sets clock protocol info
  @param   platform clock protocol info
  @return  none
**/
void pal_clock_set_expected(const void *info)
{
    if (info == NULL)
        return;
    g_clock_info = (struct arm_scmi_clock_expected *)info;
}

/*-----------  Common PAL API's across platforms ----------*/

/**
  @brief   This API is used for checking num of clocks
  @param   none
  @return  num of clocks
**/
uint32_t pal_clock_get_expected_num_clocks(void)
{
    return g_clock_info->number_clocks;
}

/**
  @brief   This API is used for checking max num of pending async cmd support for clock
  @param   none
  @return  max num of pending async cmd
**/
uint32_t pal_clock_get_expected_max_async_cmd(void)
{
    return g_clock_info->max_num_pending_async_rate_chg_supported;
}

/**
  @brief   This API is used for checking num of rates a clock supports
  @param   clock id
  @return  num of rates a clock supports
**/
uint32_t pal_clock_get_expected_number_of_rates(uint32_t clock_id)
{
    return g_clock_info->num_rates[clock_id];
}

/**
  @brief   This API checks if clock supports config change
  @param   clock id
  @return  supported / unsupported
**/
uint32_t pal_check_clock_config_change_support(uint32_t clock_id)
{
    int32_t i;

    for (i = 0; i < g_clock_info->num_of_on_clocks; i++)
    {
        if (clock_id == g_clock_info->always_on_clocks[i])
            return 1;
    }
    return 0;
}

#endif
