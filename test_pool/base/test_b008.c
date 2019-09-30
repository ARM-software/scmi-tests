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

#define TEST_NUM  (SCMI_BASE_TEST_NUM_BASE + 8)
#define TEST_DESC "Base notify errors check                               "

uint32_t base_query_notify_error_support(void)
{
    int32_t status;
    uint32_t rsp_msg_hdr, cmd_msg_hdr;
    uint32_t param_count, notify_enable, message_id;
    uint32_t return_value_count, attributes, return_values;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* If BASE NOTIFY ERRORS not supported, skip the test */

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = BASE_NOTIFY_ERRORS;
    param_count++;
    attributes = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_PROTOCOL_MESSAGE_ATTRIBUTES, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, &attributes);

    if (status == SCMI_NOT_FOUND) {
        val_print(VAL_PRINT_TEST, "\n\tBASE_NOTIFY_ERRORS not supported.                      ");
        return VAL_STATUS_SKIP;
    }

    /* BASE NOTIFY ERRORS with invalid notify_enable should return status INVALID PARAMETERS */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 1] Query base notify error with invalid notify_enable");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    notify_enable = 0x4; /* Invalid notify enable value */
    param_count++;
    return_values = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_NOTIFY_ERRORS, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &notify_enable, &rsp_msg_hdr, &status,
                     &return_value_count, &return_values);

    if (val_compare_status(status, SCMI_INVALID_PARAMETERS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
