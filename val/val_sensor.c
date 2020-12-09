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
#ifdef SENSOR_PROTOCOL

#include "val_interface.h"
#include "val_sensor.h"

static SENSOR_INFO_s g_sensor_info_table;

/**
  @brief   This API is called from app layer to execute sensor domain tests
  @param   none
  @return  test execution result
**/
uint32_t val_sensor_execute_tests(void)
{
    uint32_t version = 0;

    val_memset((void *)&g_sensor_info_table, 0, sizeof(g_sensor_info_table));

    if (val_agent_check_protocol_support(PROTOCOL_SENSOR)) {
        if (RUN_TEST(sensor_query_protocol_version(&version)))
            return VAL_STATUS_FAIL;

        RUN_TEST(sensor_query_protocol_attributes());
        RUN_TEST(sensor_query_mandatory_command_support());
        RUN_TEST(sensor_invalid_messageid_call());
        RUN_TEST(sensor_query_description_get());
        RUN_TEST(sensor_trip_point_nfy_event_ctrl_check());
        RUN_TEST(sensor_trip_point_nfy_invalid_id_check());
        RUN_TEST(sensor_trip_point_config_invalid_param_check());
        RUN_TEST(sensor_trip_point_config_invalid_id_check());
        RUN_TEST(sensor_trip_point_config_check());
        RUN_TEST(sensor_reading_get_invalid_flag_check());
        RUN_TEST(sensor_reading_get_invalid_id_check());
        RUN_TEST(sensor_reading_get_sync_mode());
        RUN_TEST(sensor_reading_get_async_mode());
        RUN_TEST(sensor_reading_get_async_mode_not_supported());
    }
    else
        val_print(VAL_PRINT_ERR, "\n Calling agent have no access to SENSOR protocol");


    return VAL_STATUS_PASS;
}

/**
  @brief   This API is used to set sensor protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Sensor protocol info table.
  @param   param_identifier  id of parameter which will be set
  @param   param_value       value of parameter
  @return  none
**/
void val_sensor_save_info(uint32_t param_identifier, uint32_t param_value)
{
    switch (param_identifier)
    {
    case NUM_SENSORS:
        g_sensor_info_table.num_sensors = param_value;
        break;
    case SENSOR_STATS_ADDR_LOW:
        g_sensor_info_table.sensor_stats_addr_low = param_value;
        break;
    case SENSOR_STATS_ADDR_HIGH:
        g_sensor_info_table.sensor_stats_addr_high = param_value;
        break;
    case SENSOR_STATS_ADDR_LEN:
        g_sensor_info_table.sensor_stats_addr_len = param_value;
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }
}

/**
  @brief   This API is used to get sensor protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Sensor protocol info table.
  @param   param_identifier  id of parameter which will be set
  @return  param_value       value of the parameter
**/
uint32_t val_sensor_get_info(uint32_t param_identifier)
{
    uint32_t param_value = 0;

    switch (param_identifier)
    {
    case NUM_SENSORS:
        param_value = g_sensor_info_table.num_sensors;
        break;
    case SENSOR_STATS_ADDR_LOW:
        param_value = g_sensor_info_table.sensor_stats_addr_low;
        break;
    case SENSOR_STATS_ADDR_HIGH:
        param_value = g_sensor_info_table.sensor_stats_addr_high;
        break;
    case SENSOR_STATS_ADDR_LEN:
        param_value = g_sensor_info_table.sensor_stats_addr_len;
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }

    return param_value;
}


/**
  @brief   This API is used to set sensor protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Sensor protocol info table.
  @param   param_identifier  id of parameter which will be set
  @param   sensor_id         Sensor id
  @param   param_value       value of parameter
  @return  none
**/
void val_sensor_save_desc_info(uint32_t param_identifier, uint32_t sensor_id, uint32_t param_value)
{
    switch (param_identifier)
    {
    case SENSOR_NUM_OF_TRIP_POINTS:
        g_sensor_info_table.desc_info[sensor_id].num_trip_points = param_value;
        break;
    case SENSOR_ASYNC_READ_SUPPORT:
        g_sensor_info_table.desc_info[sensor_id].async_read_support = param_value;
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }
}

/**
  @brief   This API is used to get sensor protocol info
           1. Caller       -  Test Suite.
           2. Prerequisite -  Sensor protocol info table.
  @param   param_identifier  id of parameter which will be set
    @param   sensor_id         Sensor id
  @return  param_value       value of the parameter
**/
uint32_t val_sensor_get_desc_info(uint32_t param_identifier, uint32_t sensor_id)
{
    uint32_t param_value = 0;
    switch (param_identifier)
    {
    case SENSOR_NUM_OF_TRIP_POINTS:
        param_value = g_sensor_info_table.desc_info[sensor_id].num_trip_points;
        break;
    case SENSOR_ASYNC_READ_SUPPORT:
        param_value =  g_sensor_info_table.desc_info[sensor_id].async_read_support;
        break;
    default:
        val_print(VAL_PRINT_ERR, "\nUnidentified parameter %d", param_identifier);
    }

    return param_value;
}

/**
  @brief   This API is used for checking num of sensors
  @param   none
  @return  num of sensors
**/
uint32_t val_sensor_get_expected_num_sensors(void)
{
    return pal_sensor_get_expected_num_sensors();
}

/**
  @brief   This API is used for checking sensor statistics addr low
  @param   none
  @return  statistics addr low
**/
uint32_t val_sensor_get_expected_stats_addr_low(void)
{
    return pal_sensor_get_expected_stats_addr_low();
}

/**
  @brief   This API is used for checking sensor statistics addr high
  @param   none
  @return  statistics addr high
**/
uint32_t val_sensor_get_expected_stats_addr_high(void)
{
    return pal_sensor_get_expected_stats_addr_high();
}

/**
  @brief   This API is used for checking sensor statistics addr len
  @param   none
  @return  statistics addr len
**/
uint32_t val_sensor_get_expected_stats_addr_len(void)
{
    return pal_sensor_get_expected_stats_addr_len();
}

#endif
