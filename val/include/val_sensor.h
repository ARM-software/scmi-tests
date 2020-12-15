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

#ifndef __VAL_SENSOR_H__
#define __VAL_SENSOR_H__

#define MAX_NUM_OF_SENSORS                  32

#define VERSION_OFFSET                      0
#define ATTRIBUTE_OFFSET                    0
#define REG_ADDR_LOW_OFFSET                 1
#define REG_ADDR_HIGH_OFFSET                2
#define REG_LENGTH_OFFSET                   3
#define NUM_SENSOR_FLAG_OFFSET              0
#define DESC_OFFSET                         1
#define SENSOR_VAL_LOW_OFFSET               0
#define SENSOR_VAL_HIGH_OFFSET              1

#define DELAYED_RESP_SENSOR_ID_OFFSET       0
#define DELAYED_RESP_SENSOR_VAL_LOW_OFFSET  1
#define DELAYED_RESP_SENSOR_VAL_HIGH_OFFSET 2

#define INVALID_FLAG_VAL                    0xF
#define SENSOR_READ_ASYNC_MODE              0x1
#define SENSOR_READ_SYNC_MODE               0x0
#define TRIP_POINT_NFY_ENABLE               0x1
#define TRIP_POINT_NFY_DISABLE              0x0
#define EVENT_CTRL_INVALID_VAL              0xF
#define TRIP_POINT_DISABLE                  0x0
#define TRIP_POINT_DEFAULT_LOW_VAL          0x0
#define TRIP_POINT_DEFAULT_HIGH_VAL         0x0
#define TRIP_POINT_CROSSED_POISTIVE_DIR     0x1
#define TRIP_POINT_CROSSED_NEGETIVE_DIR     0x2
#define TRIP_POINT_CROSSED_EITHER_DIR       0x3
#define TRIP_POINT_ID_LOW                   0x4

typedef struct {
    uint32_t num_trip_points;
    uint32_t async_read_support;
} SENSOR_DESC_INFO_s;

typedef struct {
    uint32_t num_sensors;
    uint32_t sensor_stats_addr_low;
    uint8_t  sensor_stats_addr_high;
    uint8_t  sensor_stats_addr_len;
    SENSOR_DESC_INFO_s desc_info[MAX_NUM_OF_SENSORS];
} SENSOR_INFO_s;

/* Common Tests */
uint32_t sensor_query_protocol_version(uint32_t *version);
uint32_t sensor_query_protocol_attributes(void);
uint32_t sensor_query_mandatory_command_support(void);
uint32_t sensor_invalid_messageid_call(void);
uint32_t sensor_query_description_get(void);
uint32_t sensor_trip_point_nfy_event_ctrl_check(void);
uint32_t sensor_trip_point_nfy_invalid_id_check(void);
uint32_t sensor_trip_point_config_invalid_param_check(void);
uint32_t sensor_trip_point_config_invalid_id_check(void);
uint32_t sensor_trip_point_config_check(void);
uint32_t sensor_reading_get_invalid_flag_check(void);
uint32_t sensor_reading_get_invalid_id_check(void);
uint32_t sensor_reading_get_sync_mode(void);
uint32_t sensor_reading_get_async_mode(void);
uint32_t sensor_reading_get_async_mode_not_supported(void);

uint32_t val_sensor_get_expected_num_sensors(void);
uint32_t val_sensor_get_expected_stats_addr_low(void);
uint32_t val_sensor_get_expected_stats_addr_high(void);
uint32_t val_sensor_get_expected_stats_addr_len(void);

#endif
