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
#include "val_sensor.h"

SENSOR_INFO_s g_sensor_info_table;

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
  @brief   This API is used to get sensor protocol implemented version
           1. Caller       -  Test Suite.
           2. Prerequisite -  Sensor protocol info table.
  @param   none
  @return  sensor protocol version
**/
uint32_t val_sensor_get_expected_protocol_version(void)
{
    return SENSOR_VERSION;
}
