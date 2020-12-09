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

#include "val_interface.h"
#include "val_clock.h"

static CLOCK_INFO_s g_clock_info_table;

/**
  @brief   This API is called from app layer to execute clock tests
  @param   none
  @return  test execution result
**/
uint32_t val_clock_execute_tests(void)
{
    uint32_t version = 0;

    val_memset((void *)&g_clock_info_table, 0, sizeof(g_clock_info_table));

    if (val_agent_check_protocol_support(PROTOCOL_CLOCK)) {
        if (RUN_TEST(clock_query_protocol_version(&version)))
            return VAL_STATUS_FAIL;

        RUN_TEST(clock_query_protocol_attributes());
        RUN_TEST(clock_query_mandatory_command_support());
        RUN_TEST(clock_invalid_messageid_call());
        RUN_TEST(clock_attributes_check());
        RUN_TEST(clock_attributes_check_invalid_clock_id());
        RUN_TEST(clock_query_describe_rates());
        RUN_TEST(clock_query_describe_rates_invalid_clock_id());
        RUN_TEST(clock_rate_set_sync_check());
        RUN_TEST(clock_rate_set_async_check());
        RUN_TEST(clock_rate_set_invalid_param_check());
        RUN_TEST(clock_rate_set_invalid_clock_id_check());
        RUN_TEST(clock_rate_get_check());
        RUN_TEST(clock_rate_get_invalid_clock_id_check());
        RUN_TEST(clock_config_set_invalid_attr_check());
        RUN_TEST(clock_config_set_invalid_clock_id_check());
        RUN_TEST(clock_config_set_check());
    }
    else
        val_print(VAL_PRINT_ERR, "\n Calling agent have no access to CLOCK protocol");

    return VAL_STATUS_PASS;
}

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
  @brief   This API is used to save clock rate info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Base protocol info table.
  @param   param_identifier  id of parameter which will be set
  @param   clock_id          clock id
  @param   param_value       value of parameter
  @return  none
**/
void val_clock_save_rate(uint32_t param_identifier, uint32_t clock_id, uint64_t param_value)
{
    switch (param_identifier)
    {
    case CLOCK_LOWEST_RATE:
        g_clock_info_table.clock_rate[clock_id].lowest_rate =
                param_value;
        break;
    case CLOCK_HIGHEST_RATE:
        g_clock_info_table.clock_rate[clock_id].highest_rate =
                param_value;
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
    case CLOCK_ATTRIBUTE:
        param_value = g_clock_info_table.clock_attribute[clock_id];
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }

    return param_value;
}

/**
  @brief   This API is used to get clock protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Base protocol info table.
  @param   param_identifier  id of parameter which will be set
  @param   clock_id          clock id
  @return  param_value       value of the parameter
**/
uint64_t val_clock_get_rate(uint32_t param_identifier, uint32_t clock_id)
{
    uint64_t param_value = 0;

    switch (param_identifier)
    {
    case CLOCK_LOWEST_RATE:
        param_value =
                g_clock_info_table.clock_rate[clock_id].lowest_rate;
        break;
    case CLOCK_HIGHEST_RATE:
        param_value =
                g_clock_info_table.clock_rate[clock_id].highest_rate;
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
  @brief   This API is used for checking num of clocks
  @param   none
  @return  num of clocks
**/
uint32_t val_clock_get_expected_num_clocks(void)
{
    return pal_clock_get_expected_num_clocks();
}

/**
  @brief   This API is used for checking max num of async cmds supported
  @param   none
  @return  max num of async cmds supported
**/
uint32_t val_clock_get_expected_max_async_cmd(void)
{
    return pal_clock_get_expected_max_async_cmd();
}

/**
  @brief   This API is used for checking num of rates for each clock
  @param   clock id
  @return  num of rates
**/
uint32_t val_clock_get_expected_number_of_rates(uint32_t clock_id)
{
    return  pal_clock_get_expected_number_of_rates(clock_id);
}

/**
  @brief   This API is used for checking if clock supports config change
  @param   clock id
  @return  true/false
**/
uint32_t val_check_clock_config_change_support(uint32_t clock_id)
{
    return pal_check_clock_config_change_support(clock_id);
}

#endif
