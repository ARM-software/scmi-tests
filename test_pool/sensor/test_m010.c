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

#include "val_interface.h"
#include "val_sensor.h"

#define TEST_NUM  (SCMI_SENSOR_TEST_NUM_BASE + 10)
#define TEST_DESC "Sensor trip point config set check           "

#define MAX_PARAMETER_SIZE 3

uint32_t sensor_trip_point_config_check(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t parameters[MAX_PARAMETER_SIZE];
    uint32_t sensor_id, num_sensors, message_id;
    uint32_t trip_point_evt_ctl, trip_point_id;
    uint32_t run_flag = 0, num_trip_point;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* if TRIP POINT CONFIG not supported, skip the test */
    val_print(VAL_PRINT_TEST, "\n     [Check 1] Query trip point config cmd support");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = SENSOR_TRIP_POINT_CONFIG;
    param_count++;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_PROTOCOL_MESSAGE_ATTRIBUTES,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (status == SCMI_NOT_FOUND) {
        val_print(VAL_PRINT_ERR, "\n       SENSOR_TRIP_POINT_CONFIG unsupported        ");
        return VAL_STATUS_SKIP;
    }

    num_sensors = val_sensor_get_info(NUM_SENSORS);
    if (num_sensors == 0) {
        val_print(VAL_PRINT_ERR, "\n       No Sensor found                             ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM SENSORS     : %d", num_sensors);

    /* Set trip point config with valid parameters */
    for (sensor_id = 0; sensor_id < num_sensors; sensor_id++)
    {
        num_trip_point = val_sensor_get_desc_info(SENSOR_NUM_OF_TRIP_POINTS, sensor_id);

        /* Continue if trip point is not supported */
        if (num_trip_point == 0) {
            val_print(VAL_PRINT_TEST, "\n     No trip point found for sensor_id :%d", sensor_id);
            continue;
        }

        /* Query with valid trip point id and sensor id*/
        val_print(VAL_PRINT_TEST, "\n     SENSOR ID: %d", sensor_id);
        run_flag = 1;
        for (trip_point_id = 0; trip_point_id < num_trip_point; trip_point_id++)
        {
            val_print(VAL_PRINT_TEST, "\n     [Check 2] Query with valid trip_point_id :%d",
                                     trip_point_id);

            trip_point_evt_ctl = ((trip_point_id << TRIP_POINT_ID_LOW) | TRIP_POINT_DISABLE);
            VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
            parameters[param_count++] = sensor_id;
            parameters[param_count++] = trip_point_evt_ctl;
            parameters[param_count++] = TRIP_POINT_DEFAULT_LOW_VAL;
            parameters[param_count++] = TRIP_POINT_DEFAULT_HIGH_VAL;
            cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_TRIP_POINT_CONFIG,
                                             COMMAND_MSG);
            val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                             &return_value_count, return_values);

            if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
                 return VAL_STATUS_FAIL;

            if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
                 return VAL_STATUS_FAIL;
        }
    }
    if (run_flag == 0) {
        val_print(VAL_PRINT_ERR, "\n       No trip point set for any sensor            ");
        return VAL_STATUS_SKIP;
    }

    return VAL_STATUS_PASS;
}
