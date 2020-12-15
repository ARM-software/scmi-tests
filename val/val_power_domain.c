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

#include "val_interface.h"
#include "val_power_domain.h"

static POWER_DOMAIN_INFO_s g_power_domain_info_table;

/**
  @brief   This API is called from app layer to execute power domain tests
  @param   none
  @return  test execution result
**/
uint32_t val_power_domain_execute_tests(void)
{
    uint32_t version = 0;

    val_memset((void *)&g_power_domain_info_table, 0, sizeof(g_power_domain_info_table));

    if (val_agent_check_protocol_support(PROTOCOL_POWER_DOMAIN)) {
        if (RUN_TEST(power_domain_query_protocol_version(&version)))
            return VAL_STATUS_FAIL;

        RUN_TEST(power_domain_query_protocol_attributes());
        RUN_TEST(power_domain_query_mandatory_command_support());
        RUN_TEST(power_domain_invalid_messageid_call());
        RUN_TEST(power_domain_query_domain_attributes());
        RUN_TEST(power_domain_query_domain_attributes_invalid_doamin());
        RUN_TEST(power_domain_set_power_state_check());
        RUN_TEST(power_domain_set_power_state_unsupported_domain_check());
        RUN_TEST(power_domain_set_power_state_check_invalid_domain());
        RUN_TEST(power_domain_get_power_state_check());
        RUN_TEST(power_domain_get_power_state_check_invalid_domain());
        RUN_TEST(power_domain_power_state_notify_check());
        RUN_TEST(power_domain_power_state_notify_unspported_domain_check());
        RUN_TEST(power_domain_power_state_notify_invalid_domain_check());

        if (version == PROTOCOL_VERSION_2) {
            RUN_TEST(power_domain_power_state_change_requested_notify_check());
            RUN_TEST(power_domain_power_state_change_requested_notify_invalid_domain_check());
        }
    }
    else
        val_print(VAL_PRINT_ERR, "\n Calling agent have no access to POWER DOMAIN protocol");

    return VAL_STATUS_PASS;
}

/**
  @brief   This API is used to set power_domain protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Power Domain protocol info table.
  @param   param_identifier  id of parameter which will be set
  @param   pd_id             power domain identifier
  @param   param_value       value of parameter
  @return  none
**/
void val_power_domain_save_info(uint32_t param_identifier, uint32_t pd_id, uint32_t param_value)
{
    switch (param_identifier)
    {
    case NUM_POWER_DOMAIN:
        g_power_domain_info_table.num_power_domain = param_value;
        break;
    case PD_STATS_ADDR_LOW:
        g_power_domain_info_table.pd_stats_addr_low = param_value;
        break;
    case PD_STATS_ADDR_HIGH:
        g_power_domain_info_table.pd_stats_addr_high = param_value;
        break;
    case PD_STATS_ADDR_LEN:
        g_power_domain_info_table.pd_stats_addr_len = param_value;
        break;
    case PD_STATE_CHANGE_NOTI_SUPPORT:
        g_power_domain_info_table.pd_noti_support[pd_id] = param_value;
        break;
    case PD_STATE_ASYNC_SUPPORT:
        g_power_domain_info_table.pd_async_support[pd_id] = param_value;
        break;
    case PD_STATE_SYNC_SUPPORT:
        g_power_domain_info_table.pd_sync_support[pd_id] = param_value;
        break;
    default:
        val_print(VAL_PRINT_WARN, "\nUnidentified parameter %d", param_identifier);
    }
}

/**
  @brief   This API is used to get power_domain protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Power Domain protocol info table.
  @param   pd_id             power domain identifier
  @param   param_identifier  id of parameter which will be set
  @return  param_value       value of the parameter
**/
uint32_t val_power_domain_get_info(uint32_t pd_id, uint32_t param_identifier)
{
    uint32_t param_value = 0;

    switch (param_identifier)
    {
    case NUM_POWER_DOMAIN:
        param_value = g_power_domain_info_table.num_power_domain;
        break;
    case PD_STATS_ADDR_LOW:
        param_value = g_power_domain_info_table.pd_stats_addr_low;
        break;
    case PD_STATS_ADDR_HIGH:
        param_value = g_power_domain_info_table.pd_stats_addr_high;
        break;
    case PD_STATS_ADDR_LEN:
        param_value = g_power_domain_info_table.pd_stats_addr_len;
        break;
    case PD_STATE_CHANGE_NOTI_SUPPORT:
        param_value = g_power_domain_info_table.pd_noti_support[pd_id];
        break;
    case PD_STATE_ASYNC_SUPPORT:
        param_value = g_power_domain_info_table.pd_async_support[pd_id];
        break;
    case PD_STATE_SYNC_SUPPORT:
        param_value = g_power_domain_info_table.pd_sync_support[pd_id];
        break;
    default:
        val_print(VAL_PRINT_WARN, "\nUnidentified parameter %d", param_identifier);
    }

    return param_value;
}

/**
  @brief   This API is used to set power domain name
           1. Caller       -  Test Suite.
           2. Prerequisite -  Base protocol info table.
  @param   param_identifier  id of parameter which will be set
  @param   pd_id             power domain identifier
  @param   param_value       value of parameter
  @return  none
**/
void val_power_domain_save_name(uint32_t param_identifier, uint32_t pd_id, uint8_t *param_value)
{
    switch (param_identifier)
    {
    case PD_DOMAIN_NAME:
        val_strcpy(&g_power_domain_info_table.pd_name[pd_id][0], param_value);
        break;
    default:
        val_print(VAL_PRINT_WARN, "\nUnidentified parameter %d", param_identifier);
    }
}

/**
  @brief   This API is used for checking num of power domain
  @param   none
  @return  num of power domain
**/
uint32_t val_power_get_expected_num_domains(void)
{
    return pal_power_get_expected_num_domains();
}

/**
  @brief   This API is used for checking power domain statistics addr low
  @param   none
  @return  statistics addr low
**/
uint32_t val_power_get_expected_stats_addr_low(void)
{
    return pal_power_get_expected_stats_addr_low();
}

/**
  @brief   This API is used for checking power domain statistics addr high
  @param   none
  @return  statistics addr high
**/
uint32_t val_power_get_expected_stats_addr_high(void)
{
    return pal_power_get_expected_stats_addr_high();
}

/**
  @brief   This API is used for checking power domain statistics addr len
  @param   none
  @return  statistics addr len
**/
uint32_t val_power_get_expected_stats_addr_len(void)
{
    return pal_power_get_expected_stats_addr_len();
}

#endif
