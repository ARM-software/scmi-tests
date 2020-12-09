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

#define TEST_NUM  (SCMI_SENSOR_TEST_NUM_BASE + 5)
#define TEST_DESC "Sensor description get check                 "

#define SENSOR_DESC_LEN 7

uint32_t sensor_query_description_get(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t num_remaining_desc, num_desc_retured = 0;
    uint32_t i, desc_index = 0, sensor_id;
    uint32_t async_support, num_trip_points;
    uint32_t *sensor_desc, num_sensor_flag;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    val_print(VAL_PRINT_TEST, "\n     [Check 1] Query sensor description ");

    /* Reading sensor description repeatedly until all sensor details received*/
    do
    {
        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        desc_index += num_desc_retured;
        param_count++;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_DESCRIPTION_GET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, &desc_index, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        num_sensor_flag = return_values[NUM_SENSOR_FLAG_OFFSET];
        num_remaining_desc = VAL_EXTRACT_BITS(num_sensor_flag, 16, 31);
        val_print(VAL_PRINT_DEBUG, "\n       NUM OF REMAINING DESC  : %d", num_remaining_desc);

        if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(num_sensor_flag, 12, 15)) !=
                VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        num_desc_retured = VAL_EXTRACT_BITS(num_sensor_flag, 0, 11);
        val_print(VAL_PRINT_DEBUG, "\n       NUM OF DESC RETURNED   : %d", num_desc_retured);

        /* Sensor desc start from return_values[1] */
        sensor_desc = &return_values[DESC_OFFSET];
        for (i = 0; i < num_desc_retured; i++)
        {
            sensor_id = sensor_desc[i * SENSOR_DESC_LEN];
            async_support = VAL_EXTRACT_BITS(sensor_desc[1 + (i * SENSOR_DESC_LEN)], 31, 31);
            num_trip_points = VAL_EXTRACT_BITS(sensor_desc[1 + (i * SENSOR_DESC_LEN)], 0, 7);
            if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(
                    sensor_desc[1 + (i * SENSOR_DESC_LEN)], 8, 30)) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;
            if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(
                    sensor_desc[2 + (i * SENSOR_DESC_LEN)], 16, 21)) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;
            if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(
                    sensor_desc[2 + (i * SENSOR_DESC_LEN)], 8, 10)) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            val_print(VAL_PRINT_DEBUG, "\n       Sensor id              : %d", sensor_id);
            val_print(VAL_PRINT_DEBUG, "\n       Async read support     : %d", async_support);
            val_print(VAL_PRINT_DEBUG, "\n       Number of trip points  : %d", num_trip_points);
            val_print(VAL_PRINT_DEBUG, "\n       Sensor update interval : %dx10^%d",
                    VAL_EXTRACT_BITS(sensor_desc[2 + (i * SENSOR_DESC_LEN)], 27, 31),
                    VAL_EXTRACT_BITS(sensor_desc[2 + (i * SENSOR_DESC_LEN)], 22, 26));
            val_print(VAL_PRINT_DEBUG, "\n       Unit multiplier        : %d",
                    VAL_EXTRACT_BITS(sensor_desc[2 + (i * SENSOR_DESC_LEN)], 11, 15));
            val_print(VAL_PRINT_DEBUG, "\n       Sensor name            : %s",
                    &sensor_desc[3 + (i * SENSOR_DESC_LEN)]);

            /* Save sensor information */
            val_sensor_save_desc_info(SENSOR_NUM_OF_TRIP_POINTS, sensor_id, num_trip_points);
            val_sensor_save_desc_info(SENSOR_ASYNC_READ_SUPPORT, sensor_id, async_support);
        }
    } while (num_remaining_desc > 0);

    desc_index += num_desc_retured;
    if (val_compare("NUM OF DESC  ", desc_index, val_sensor_get_expected_num_sensors()))
        return VAL_STATUS_FAIL;

    /* Sensor get desc for invalid descriptor index */
    val_print(VAL_PRINT_TEST, "\n     [Check 2] Query with invalid desc_index : %d", desc_index);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    param_count++;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_DESCRIPTION_GET, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &desc_index, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_INVALID_PARAMETERS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
