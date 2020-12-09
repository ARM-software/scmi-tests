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

#include "val_interface.h"
#include "val_base.h"

BASE_INFO_s g_base_info_table;

/**
  @brief   This API is called from app layer to execute base tests
  @param   none
  @return  test execution result
**/
uint32_t val_base_execute_tests(void)
{
    uint32_t version = 0;

    val_memset((void *)&g_base_info_table, 0, sizeof(g_base_info_table));

    if (RUN_TEST(base_query_protocol_version(&version)))
        return VAL_STATUS_FAIL;

    RUN_TEST(base_query_protocol_attributes());
    RUN_TEST(base_query_mandatory_command_support());
    RUN_TEST(base_invalid_messageid_call());
    RUN_TEST(base_query_vendor_name());
    RUN_TEST(base_query_subvendor_info());
    RUN_TEST(base_query_implementation_version());
    RUN_TEST(base_query_protocol_list());
    if (version == PROTOCOL_VERSION_1) {
        RUN_TEST(base_discover_agent_v1());
    }
    if (version == PROTOCOL_VERSION_2) {
        RUN_TEST(base_discover_agent());
    }
    RUN_TEST(base_query_notify_error_support());

    if (version == PROTOCOL_VERSION_2) {
        RUN_TEST(base_set_device_permissions_check());
        RUN_TEST(base_deny_restore_device_access());
        RUN_TEST(base_set_protocol_permissions_check());
        RUN_TEST(base_deny_restore_protocol_access());
        RUN_TEST(base_reset_agent_configuration_check());
        RUN_TEST(base_restore_device_access_with_reset_agent_configuration());
        RUN_TEST(base_restore_protocol_access_with_reset_agent_configuration());
    }

    return VAL_STATUS_PASS;
}

/**
  @brief   This API is used to set base protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Base protocol info table.
  @param   param_identifier  id of parameter which will be set
  @param   param_value       value of parameter
  @return  none
**/
void val_base_save_info(uint32_t param_identifier, uint32_t param_value)
{
    switch (param_identifier)
    {
    case BASE_NUM_AGENTS:
        g_base_info_table.num_agents = param_value;
        break;
    case BASE_NUM_PROTOCOLS:
        g_base_info_table.num_protocols = param_value;
        break;
    case BASE_TEST_AGENT_ID:
        g_base_info_table.test_agent_id = param_value;
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified base parameter %d", param_identifier);
    }
}

/**
  @brief   This API is used to get base protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Base protocol info table.
  @param   param_identifier  id of parameter which will be set
  @return  param_value       value of the parameter
**/
uint32_t val_base_get_info(uint32_t param_identifier)
{
    uint32_t param_value = 0;

    switch (param_identifier)
    {
    case BASE_NUM_AGENTS:
        param_value = g_base_info_table.num_agents;
        break;
    case BASE_NUM_PROTOCOLS:
        param_value = g_base_info_table.num_protocols;
        break;
    case BASE_TEST_AGENT_ID:
        param_value = g_base_info_table.test_agent_id;
        break;
    case BASE_IMPLEMENTATION_VERSION:
        param_value = g_base_info_table.implementation_version;
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }

    return param_value;
}

/**
  @brief   This API is used to set base vendor name
           1. Caller       -  Test Suite.
           2. Prerequisite -  Base protocol info table.
  @param   param_identifier  id of parameter which will be set
  @param   param_value       value of parameter
  @return  none
**/
void val_base_save_name(uint32_t param_identifier, uint8_t *param_value)
{
    switch (param_identifier)
    {
    case BASE_VENDOR_NAME:
        val_strcpy(&(g_base_info_table.vendor_name[0]), param_value);
        break;
    case BASE_SUBVENDOR_NAME:
        val_strcpy(&(g_base_info_table.subvendor_name[0]), param_value);
        break;
    case BASE_TEST_AGENT_NAME:
        val_strcpy(&(g_base_info_table.test_agent_name[0]), param_value);
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }
}

/**
  @brief   This API is used for checking vendor name
  @param   none
  @return  vendor name
**/
char *val_base_get_expected_vendor_name(void)
{
    return pal_base_get_expected_vendor_name();
}

/**
  @brief   This API is used for checking subvendor name
  @param   none
  @return  subvendor name
**/
char *val_base_get_expected_subvendor_name(void)
{
    return pal_base_get_expected_subvendor_name();
}

/**
  @brief   This API is used for checking implementation version
  @param   none
  @return  implementation version
**/
uint32_t val_base_get_expected_implementation_version(void)
{
    return pal_base_get_expected_implementation_version();
}

/**
  @brief   This API is used for checking num of agents
  @param   none
  @return  num of agents
**/
uint32_t val_base_get_expected_num_agents(void)
{
    return pal_base_get_expected_num_agents();
}

/**
  @brief   This API is used for checking num of protocols
  @param   none
  @return  num of protocols agent have access
**/
uint32_t val_base_get_expected_num_protocols(void)
{
    return pal_base_get_expected_num_protocols();
}
