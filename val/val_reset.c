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
#ifdef RESET_PROTOCOL

#include "val_interface.h"
#include "val_reset.h"

static RESET_INFO_s g_reset_info_table;

/**
  @brief   This API is called from app layer to execute reset domain tests
  @param   none
  @return  test execution result
**/
uint32_t val_reset_execute_tests(void)
{
    uint32_t version = 0;

    val_memset((void *)&g_reset_info_table, 0, sizeof(g_reset_info_table));

    if (val_agent_check_protocol_support(PROTOCOL_RESET)) {
        if (RUN_TEST(reset_query_protocol_version(&version)))
            return VAL_STATUS_FAIL;

        RUN_TEST(reset_query_protocol_attributes());
        RUN_TEST(reset_query_mandatory_command_support());
        RUN_TEST(reset_invalid_messageid_call());
        RUN_TEST(reset_query_domain_attributes());
        RUN_TEST(reset_query_domain_attributes_invalid_id());
        RUN_TEST(reset_query_reset_command_invalid_id());
        RUN_TEST(reset_query_reset_command_invalid_flag());
        RUN_TEST(reset_query_reset_command_invalid_reset_state());
        RUN_TEST(reset_query_notify_invalid_id());
        RUN_TEST(reset_query_invalid_notify_enable());
    }
    else
        val_print(VAL_PRINT_ERR, "\n Calling agent have no access to RESET protocol");

    return VAL_STATUS_PASS;
}

/**
  @brief   This API is used to set reset protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Reset protocol info table.
  @param   param_identifier  id of parameter which will be set
  @param   param_value       value of parameter
  @return  none
**/
void val_reset_save_info(uint32_t param_identifier, uint32_t domain_id, uint32_t param_value)
{
    switch (param_identifier)
    {
    case NUM_RESET_DOMAINS:
        g_reset_info_table.num_domains = param_value;
        break;
    case RESET_ASYNC_SUPPORT:
        g_reset_info_table.domain_info[domain_id].async_support =
                param_value;
        break;
    case RESET_NOTIFY_SUPPORT:
        g_reset_info_table.domain_info[domain_id].notify_support =
                param_value;
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }
}

/**
  @brief   This API is used to get reset protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Reset protocol info table.
  @param   param_identifier  id of parameter which will be set
  @return  param_value       value of the parameter
**/
uint32_t val_reset_get_info(uint32_t param_identifier, uint32_t domain_id)
{
    uint32_t param_value = 0;

    switch (param_identifier)
    {
    case NUM_RESET_DOMAINS:
        param_value = g_reset_info_table.num_domains;
        break;
    case RESET_ASYNC_SUPPORT:
        param_value = g_reset_info_table.domain_info[domain_id].async_support;
        break;
    case RESET_NOTIFY_SUPPORT:
        param_value = g_reset_info_table.domain_info[domain_id].notify_support;
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }

    return param_value;
}

/**
  @brief   This API is used for checking num of reset domain
  @param   none
  @return  num of reset domain
**/
uint32_t val_reset_get_expected_num_domains(void)
{
    return pal_reset_get_expected_num_domains();
}

/**
  @brief   This API is used for checking reset domain name
  @param   domain id
  @return  reset domain name
**/
uint8_t *val_reset_get_expected_name(uint32_t domain_id)
{
    return pal_reset_get_expected_name(domain_id);
}

#endif
