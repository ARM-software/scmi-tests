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
#ifdef SYSTEM_POWER_PROTOCOL

#include "val_interface.h"
#include "val_system_power.h"

static SYSTEM_POWER_INFO_s g_system_power_info_table;

/**
  @brief   This API is called from app layer to execute system power tests
  @param   none
  @return  test execution result
**/
uint32_t val_system_power_execute_tests(void)
{
    uint32_t version = 0;

    val_memset((void *)&g_system_power_info_table, 0, sizeof(g_system_power_info_table));

    if (val_agent_check_protocol_support(PROTOCOL_SYSTEM_POWER)) {
        if (RUN_TEST(system_power_query_protocol_version(&version)))
            return VAL_STATUS_FAIL;

        RUN_TEST(system_power_query_protocol_attributes());
        RUN_TEST(system_power_query_mandatory_command_support());
        RUN_TEST(system_power_invalid_messageid_call());
        RUN_TEST(system_power_state_set_invalid_parameters());
        RUN_TEST(system_power_state_get_check());
        RUN_TEST(system_power_state_notify_invalid_parameters());
    }
    else
        val_print(VAL_PRINT_ERR, "\n Calling agent have no access to SYSTEM POWER protocol");

    return VAL_STATUS_PASS;
}

/**
  @brief   This API is used to set system_power protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  System Power protocol info table.
  @param   param_identifier  id of parameter which will be set
  @param   param_value       value of parameter
  @return  none
**/
void val_system_power_save_info(uint32_t param_identifier, uint32_t param_value)
{
    switch (param_identifier)
    {
    case SYSTEM_RESET_SUPPORT:
        g_system_power_info_table.system_power_reset_support = param_value;
        break;
    case SYSTEM_SUSPEND_SUPPORT:
        g_system_power_info_table.system_power_suspend_support = param_value;
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }
}

/**
  @brief   This API is used to get system_power protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  System Power protocol info table.
  @param   param_identifier  id of parameter which will be set
  @return  param_value       value of the parameter
**/
uint32_t val_system_power_get_info(uint32_t param_identifier)
{
    uint32_t param_value = 0;

    switch (param_identifier)
    {
    case SYSTEM_RESET_SUPPORT:
        param_value = g_system_power_info_table.system_power_reset_support;
        break;
    case SYSTEM_SUSPEND_SUPPORT:
        param_value = g_system_power_info_table.system_power_suspend_support;
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }

    return param_value;
}

#endif
