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

#include "val_interface.h"
#include "val_clock.h"

CLOCK_INFO_s g_clock_info_table;

/**
  @brief   This API is used to save clock protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Base protocol info table.
  @param   param_identifier  id of parameter which will be set
  @param   clock_id          clock id
  @param   param_value       value of parameter
  @return  none
**/
void val_clock_save_info(uint32_t param_identifier, uint32_t clock_id, uint32_t param_value)
{
    switch (param_identifier)
    {
    case NUM_CLOCKS:
        g_clock_info_table.num_clocks = param_value;
        break;
    case CLOCK_MAX_PENDING_ASYNC_CMD:
        g_clock_info_table.max_async_cmd = param_value;
        break;
    case CLOCK_ATTRIBUTE:
        g_clock_info_table.clock_attribute[clock_id] = param_value;
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }
}

/**
  @brief   This API is used to get clock protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Base protocol info table.
  @param   param_identifier  id of parameter which will be set
  @param   clock_id          clock id
  @return  param_value       value of the parameter
**/
uint32_t val_clock_get_info(uint32_t param_identifier, uint32_t clock_id)
{
    uint32_t param_value = 0;

    switch (param_identifier)
    {
    case NUM_CLOCKS:
        param_value = g_clock_info_table.num_clocks;
        break;
    case CLOCK_MAX_PENDING_ASYNC_CMD:
        param_value = g_clock_info_table.max_async_cmd;
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }

    return param_value;
}

/**
  @brief   This API is used to set clock vendor name
           1. Caller       -  Test Suite.
           2. Prerequisite -  Base protocol info table.
  @param   param_identifier  id of parameter which will be set
  @param   param_value       value of parameter
  @return  none
**/
void val_clock_save_name(uint32_t param_identifier, uint32_t clock_id, uint8_t *param_value)
{
    switch (param_identifier)
    {
    case CLOCK_NAME:
        val_strcpy(&g_clock_info_table.clock_name[clock_id][0], param_value);
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }
}

/**
  @brief   This API is used to get clock protocol implemented version
           1. Caller       -  Test Suite.
           2. Prerequisite -  clock protocol info table.
  @param   none
  @return  clock protocol version
**/
uint32_t val_clock_get_expected_protocol_version(void)
{
    return CLOCK_VERSION;
}
