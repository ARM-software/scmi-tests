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

#define TEST_NUM  (SCMI_SENSOR_TEST_NUM_BASE + 6)
#define TEST_DESC "Sensor trip pnt notify invalid event_ctrl chk"

#define MAX_PARAMETER_SIZE 2

uint32_t sensor_trip_point_nfy_event_ctrl_check(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t parameters[MAX_PARAMETER_SIZE];
    uint32_t sensor_id, num_sensors;
    uint32_t message_id;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* if TRIP POINT NOTIFY not supported, skip the test */
    val_print(VAL_PRINT_TEST, "\n     [Check 1] Query trip point notify cmd support");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = SENSOR_TRIP_POINT_NOTIFY;
    param_count++;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_PROTOCOL_MESSAGE_ATTRIBUTES,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (status == SCMI_NOT_FOUND) {
        val_print(VAL_PRINT_ERR, "\n       SENSOR_TRIP_POINT_NOTIFY unsupported        ");
        return VAL_STATUS_SKIP;
    }

    num_sensors = val_sensor_get_info(NUM_SENSORS);
    if (num_sensors == 0) {
        val_print(VAL_PRINT_ERR, "\n       No Sensor found                             ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM SENSORS     : %d", num_sensors);

    /* Set trip point notify with incorrect event control */
    for (sensor_id = 0; sensor_id < num_sensors; sensor_id++)
    {
        val_print(VAL_PRINT_TEST, "\n     SENSOR ID: %d", sensor_id);
        val_print(VAL_PRINT_TEST, "\n     [Check 2] Send invalid event control");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = sensor_id;
        parameters[param_count++] = EVENT_CTRL_INVALID_VAL;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_TRIP_POINT_NOTIFY, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
             return VAL_STATUS_FAIL;

        if (val_compare_status(status, SCMI_INVALID_PARAMETERS) != VAL_STATUS_PASS)
             return VAL_STATUS_FAIL;
    }

    return VAL_STATUS_PASS;
}
