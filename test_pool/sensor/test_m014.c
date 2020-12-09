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

#define TEST_NUM  (SCMI_SENSOR_TEST_NUM_BASE + 14)
#define TEST_DESC "Sensor reading get - async mode access check "

#define MAX_PARAMETER_SIZE 2

uint32_t sensor_reading_get_async_mode(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t parameters[MAX_PARAMETER_SIZE];
    uint32_t sensor_id, num_sensors, run_flag = 0;
    uint32_t sensor_id_rcvd, sensor_val_low, sensor_val_high;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    num_sensors = val_sensor_get_info(NUM_SENSORS);
    if (num_sensors == 0) {
        val_print(VAL_PRINT_ERR, "\n       No Sensor found                             ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM SENSORS     : %d", num_sensors);

    /* Reading sensor value in async mode  */
    for (sensor_id = 0; sensor_id < num_sensors; sensor_id++)
    {
        /* Skip if sensor not supported async mode access */
        if (val_sensor_get_desc_info(SENSOR_ASYNC_READ_SUPPORT, sensor_id) == 0)
            continue;

        run_flag = 1;

        /* Query with async mode settings */
        val_print(VAL_PRINT_TEST, "\n     SENSOR ID: %d", sensor_id);
        val_print(VAL_PRINT_TEST, "\n     [Check 1] Get sensor reading asynchronously");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = sensor_id;
        parameters[param_count++] = SENSOR_READ_ASYNC_MODE;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_READING_GET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
             return VAL_STATUS_FAIL;

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
             return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        /* Wait for delayed response */
        val_print(VAL_PRINT_TEST, "\n     [Check 2] Wait for delayed response");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        val_receive_notification(&rsp_msg_hdr, &return_value_count, return_values);

        val_print(VAL_PRINT_TEST, "\n       NOTIFY MSG HDR   : 0x%08x", rsp_msg_hdr);
        if (val_compare("PROTOCOL ID", VAL_EXTRACT_BITS(rsp_msg_hdr, 10, 17), PROTOCOL_SENSOR))
             return VAL_STATUS_FAIL;
        if (val_compare("MSG TYPE   ", VAL_EXTRACT_BITS(rsp_msg_hdr, 8, 9), DELAYED_RESPONSE_MSG))
             return VAL_STATUS_FAIL;
        if (val_compare("MSG ID     ", VAL_EXTRACT_BITS(rsp_msg_hdr, 0, 7),
                        SENSOR_READING_COMPLETE))
             return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        sensor_id_rcvd = return_values[DELAYED_RESP_SENSOR_ID_OFFSET];
        if (val_compare("SENSOR ID           ", sensor_id_rcvd, sensor_id))
                return VAL_STATUS_FAIL;

        sensor_val_low = return_values[DELAYED_RESP_SENSOR_VAL_LOW_OFFSET];
        sensor_val_high = return_values[DELAYED_RESP_SENSOR_VAL_HIGH_OFFSET];

        val_print(VAL_PRINT_DEBUG, "\n       Sensor value low       : 0x%08X", sensor_val_low);
        val_print(VAL_PRINT_DEBUG, "\n       Sensor value high      : 0x%08X", sensor_val_high);
    }

    if (run_flag == 0) {
        val_print(VAL_PRINT_ERR, "\n       No sensor support async mode                ");
        return VAL_STATUS_SKIP;
    }

    return VAL_STATUS_PASS;
}
