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

#include"val_interface.h"

#define TEST_NUM  (SCMI_SENSOR_TEST_NUM_BASE + 3)
#define TEST_DESC "Sensor Protocol mandatory command support check        "

uint32_t sensor_query_mandatory_command_support(void)
{
    int32_t status;
    uint32_t rsp_msg_hdr, cmd_msg_hdr;
    uint32_t param_count, message_id;
    uint32_t return_value_count, attributes;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Mandatory cmd SENSOR DESCRIPTION GET should be supported */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 1] SENSOR DESCRIPTION GET support");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = SENSOR_DESCRIPTION_GET;
    param_count++;
    attributes = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_PROTOCOL_MESSAGE_ATTRIBUTES,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, &attributes);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_reserved_bits_check_is_zero(attributes) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Mandatory cmd SENSOR READING GET should be supported */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 2] SENSOR READING GET support");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = SENSOR_READING_GET;
    param_count++;
    attributes = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_SENSOR, SENSOR_PROTOCOL_MESSAGE_ATTRIBUTES,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, &attributes);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_reserved_bits_check_is_zero(attributes) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
