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

#define TEST_NUM  (SCMI_SENSOR_TEST_NUM_BASE + 15)
#define TEST_DESC "Sensor reading get async mode unsupported chk"

#define MAX_PARAMETER_SIZE 2

uint32_t sensor_reading_get_async_mode_not_supported(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t parameters[MAX_PARAMETER_SIZE];
    uint32_t sensor_id, num_sensors, run_flag = 0;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    num_sensors = val_sensor_get_info(NUM_SENSORS);
    if (num_sensors == 0) {
        val_print(VAL_PRINT_ERR, "\n       No Sensor found                             ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM SENSORS     : %d", num_sensors);

    /* Reading sensor value in async mode for unsupported sensor */
    for (sensor_id = 0; sensor_id < num_sensors; sensor_id++)
    {
        /* Skip if sensor supported async mode */
        if (val_sensor_get_desc_info(SENSOR_ASYNC_READ_SUPPORT, sensor_id) == 1)
            continue;

        run_flag = 1;

        val_print(VAL_PRINT_TEST, "\n     [Check 1] Async mode read for unsupported sensor: %d",
                                 sensor_id);

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = sensor_id;
        parameters[param_count++] = SENSOR_READ_ASYNC_MODE;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_READING_GET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
             return VAL_STATUS_FAIL;

        if (val_compare_status(status, SCMI_NOT_SUPPORTED) != VAL_STATUS_PASS)
             return VAL_STATUS_FAIL;
    }

    if (run_flag == 0) {
        val_print(VAL_PRINT_ERR, "\n       All sensor support async mode access        ");
        return VAL_STATUS_SKIP;
    }

    return VAL_STATUS_PASS;
}
