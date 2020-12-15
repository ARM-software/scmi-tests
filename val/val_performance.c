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
#ifdef PERFORMANCE_PROTOCOL

#include "val_interface.h"
#include "val_performance.h"

static PERFORMANCE_INFO_s g_performance_info_table;

/**
  @brief   This API is called from app layer to execute performance tests
  @param   none
  @return  test execution result
**/
uint32_t val_performance_execute_tests(void)
{
    uint32_t version = 0;

    val_memset((void *)&g_performance_info_table, 0, sizeof(g_performance_info_table));

    if (val_agent_check_protocol_support(PROTOCOL_PERFORMANCE)) {
        if (RUN_TEST(performance_query_protocol_version(&version)))
            return VAL_STATUS_FAIL;

        RUN_TEST(performance_query_protocol_attributes());

        if (version == PROTOCOL_VERSION_1) {
            RUN_TEST(performance_query_mandatory_command_support_v1());
            RUN_TEST(performance_invalid_messageid_call());
            RUN_TEST(performance_query_domain_attributes_v1());
        }

        if (version == PROTOCOL_VERSION_2) {
            RUN_TEST(performance_query_mandatory_command_support());
            RUN_TEST(performance_invalid_messageid_call());
            RUN_TEST(performance_query_domain_attributes());
        }
        RUN_TEST(performance_query_domain_attributes_invalid_domain());
        RUN_TEST(performance_query_describe_levels());
        RUN_TEST(performance_query_describe_levels_invalid_domain());
        RUN_TEST(performance_query_set_limit());
        RUN_TEST(performance_query_set_limit_invalid_range());
        RUN_TEST(performance_query_set_limit_invalid_domain());
        RUN_TEST(performance_query_get_limit_invalid_domain());
        RUN_TEST(performance_query_set_level());
        RUN_TEST(performance_query_set_level_invalid_range());
        RUN_TEST(performance_query_set_level_invalid_domain());
        RUN_TEST(performance_query_get_level_invalid_domain());
        RUN_TEST(performance_query_notify_limit_invalid_parameters());
        RUN_TEST(performance_query_notify_limit_invalid_domain());
        RUN_TEST(performance_query_notify_level_invalid_parameters());
        RUN_TEST(performance_query_notify_level_invalid_domain());

        if (version == PROTOCOL_VERSION_2) {
            RUN_TEST(performance_query_describe_fast_channel());
            RUN_TEST(performance_query_describe_fast_channel_invalid_domain());
            RUN_TEST(performance_query_describe_fast_channel_invalid_message());
            RUN_TEST(performance_query_describe_fast_channel_unsupported_domain());
            RUN_TEST(performance_query_describe_fast_channel_unsupported_message());
        }
        RUN_TEST(performance_limit_set_async());
        RUN_TEST(performance_level_set_async());

        if (version == PROTOCOL_VERSION_2) {
            RUN_TEST(performance_level_get_fast_channel());
            RUN_TEST(performance_limits_get_fast_channel());
        }
    }
    else
        val_print(VAL_PRINT_ERR, "\n Calling agent have no access to PERFORMANCE protocol");

    return VAL_STATUS_PASS;
}

/**
  @brief   This API is used to set performance protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Performance protocol info table.
  @param   param_identifier  id of parameter which will be set
  @param   perf_id           Performance domain identifier,
                             (used as Message id for PERF_MESSAGE_FAST_CH_SUPPORT)
  @param   param_value       value of parameter
  @return  none
**/
void val_performance_save_info(uint32_t param_identifier, uint32_t perf_id, uint32_t param_value)
{
    switch (param_identifier)
    {
    case NUM_PERF_DOMAINS:
        g_performance_info_table.num_perf_domains = param_value;
        break;
    case PERF_STATS_ADDR_LOW:
        g_performance_info_table.perf_stats_addr_low = param_value;
        break;
    case PERF_STATS_ADDR_HIGH:
        g_performance_info_table.perf_stats_addr_high = param_value;
        break;
    case PERF_STATS_ADDR_LEN:
        g_performance_info_table.perf_stats_addr_len = param_value;
        break;
    case PERF_DOMAIN_FAST_CH_SUPPORT:
        g_performance_info_table.perf_domain_info[perf_id].fast_ch_support = param_value;
        break;
    case PERF_DOMAIN_LVL_CHANGE_NTFY_SUPPORT:
        g_performance_info_table.perf_domain_info[perf_id].Perf_lvl_change_ntfy_support =
                param_value;
        break;
    case PERF_DOMAIN_LIMIT_CHANGE_NTFY_SUPPORT:
        g_performance_info_table.perf_domain_info[perf_id].Perf_limit_change_ntfy_support =
                param_value;
        break;
    case PERF_DOMAIN_SET_PERFORMANCE_LEVEL_SUPPORT:
        g_performance_info_table.perf_domain_info[perf_id].set_performance_level_support =
                param_value;
        break;
    case PERF_DOMAIN_SET_PERFORMANCE_LIMIT_SUPPORT:
        g_performance_info_table.perf_domain_info[perf_id].set_performance_limit_support =
                param_value;
        break;
    case PERF_DOMAIN_MAX_LEVEL:
       g_performance_info_table.perf_domain_info[perf_id].maximum_level = param_value;
       break;
    case PERF_DOMAIN_MIN_LEVEL:
       g_performance_info_table.perf_domain_info[perf_id].minimum_level = param_value;
       break;
    case PERF_DOMAIN_INTERMEDIATE_LEVEL:
       g_performance_info_table.perf_domain_info[perf_id].intermediate_level = param_value;
       break;
    case PERF_MESSAGE_FAST_CH_SUPPORT:
         switch (perf_id)
         {
         case PERFORMANCE_LIMITS_SET:
             g_performance_info_table.perf_fast_cmd_ch_support.performance_level_set =
                     param_value;
             break;
         case PERFORMANCE_LIMITS_GET:
             g_performance_info_table.perf_fast_cmd_ch_support.performance_level_get =
                     param_value;
             break;
         case PERFORMANCE_LEVEL_SET:
             g_performance_info_table.perf_fast_cmd_ch_support.performance_level_set = param_value;
             break;
         case PERFORMANCE_LEVEL_GET:
             g_performance_info_table.perf_fast_cmd_ch_support.performance_level_get = param_value;
             break;
         default:
             val_print(VAL_PRINT_ERR, "\nUnidentified Command %d,parameter identifier = %d",
                     perf_id, param_identifier);
         }
         break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }
}

/**
  @brief   This API is used to get performance protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Performance protocol info table.
  @param   param_identifier  id of parameter which will be set
  @param   perf_id           Performance domain identifier,
                             (used as Message id for PERF_MESSAGE_FAST_CH_SUPPORT)
  @return  param_value       value of the parameter
**/
uint32_t val_performance_get_info(uint32_t param_identifier, uint32_t perf_id)
{
    uint32_t param_value = 0;

    switch (param_identifier)
    {
    case NUM_PERF_DOMAINS:
        param_value = g_performance_info_table.num_perf_domains;
        break;
    case PERF_STATS_ADDR_LOW:
        param_value = g_performance_info_table.perf_stats_addr_low;
        break;
    case PERF_STATS_ADDR_HIGH:
        param_value = g_performance_info_table.perf_stats_addr_high;
        break;
    case PERF_STATS_ADDR_LEN:
        param_value =  g_performance_info_table.perf_stats_addr_len;
        break;
    case PERF_DOMAIN_FAST_CH_SUPPORT:
        param_value = g_performance_info_table.perf_domain_info[perf_id].fast_ch_support;
        break;
    case PERF_DOMAIN_LVL_CHANGE_NTFY_SUPPORT:
        param_value =
                g_performance_info_table.perf_domain_info[perf_id].Perf_lvl_change_ntfy_support;
        break;
    case PERF_DOMAIN_LIMIT_CHANGE_NTFY_SUPPORT:
        param_value =
                g_performance_info_table.perf_domain_info[perf_id].Perf_limit_change_ntfy_support;
        break;
    case PERF_DOMAIN_SET_PERFORMANCE_LEVEL_SUPPORT:
        param_value =
                g_performance_info_table.perf_domain_info[perf_id].set_performance_level_support;
        break;
    case PERF_DOMAIN_SET_PERFORMANCE_LIMIT_SUPPORT:
        param_value =
                g_performance_info_table.perf_domain_info[perf_id].set_performance_limit_support;
        break;
    case PERF_DOMAIN_MAX_LEVEL:
        param_value = g_performance_info_table.perf_domain_info[perf_id].maximum_level;
       break;
    case PERF_DOMAIN_MIN_LEVEL:
       param_value = g_performance_info_table.perf_domain_info[perf_id].minimum_level;
       break;
    case PERF_DOMAIN_INTERMEDIATE_LEVEL:
       param_value = g_performance_info_table.perf_domain_info[perf_id].intermediate_level;
       break;
    case PERF_MESSAGE_FAST_CH_SUPPORT:
         switch (perf_id)
         {
         case PERFORMANCE_LIMITS_SET:
             param_value = g_performance_info_table.perf_fast_cmd_ch_support.performance_level_set;
             break;
         case PERFORMANCE_LIMITS_GET:
             param_value = g_performance_info_table.perf_fast_cmd_ch_support.performance_level_get;
             break;
         case PERFORMANCE_LEVEL_SET:
             param_value = g_performance_info_table.perf_fast_cmd_ch_support.performance_level_set;
             break;
         case PERFORMANCE_LEVEL_GET:
             param_value = g_performance_info_table.perf_fast_cmd_ch_support.performance_level_get;
             break;
         default:
             val_print(VAL_PRINT_ERR, "\nUnidentified Command %d,parameter identifier = %d",
                     perf_id, param_identifier);
         }
         break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }

    return param_value;
}

/**
  @brief   This API is used for checking number of perf domains
  @param   none
  @return  num of perf domains
**/
uint32_t val_performance_get_expected_num_domains(void)
{
    return pal_performance_get_expected_num_domains();
}

/**
  @brief   This API is used for checking perf statistics addr low
  @param   none
  @return  statistics addr low
**/
uint32_t val_performance_get_expected_stats_addr_low(void)
{
    return pal_performance_get_expected_stats_addr_low();
}

/**
  @brief   This API is used for checking perf statistics addr high
  @param   none
  @return  statistics addr high
**/
uint32_t val_performance_get_expected_stats_addr_high(void)
{
    return pal_performance_get_expected_stats_addr_high();
}

/**
  @brief   This API is used for checking statistics addr len
  @param   none
  @return  statistics addr len
**/
uint32_t val_performance_get_expected_stats_addr_len(void)
{
    return pal_performance_get_expected_stats_addr_len();
}

/**
  @brief   This API is used for checking domain name
  @param   perf domain id
  @return  perf domain name
**/
uint8_t *val_performance_get_expected_name(uint32_t domain_id)
{
    return pal_performance_get_expected_name(domain_id);
}

/**
  @brief   This API is used for checking num of level for perf domain
  @param   perf domain id
  @return  num of level for perf domain
**/
uint32_t val_performance_get_expected_number_of_level(uint32_t domain_id)
{
    return  pal_performance_get_expected_number_of_level(domain_id);
}

#endif
