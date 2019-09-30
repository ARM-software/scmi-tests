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

#define TEST_NUM  (SCMI_BASE_TEST_NUM_BASE + 6)
#define TEST_DESC "Base query subvendor name check                        "

uint32_t base_query_subvendor_info(void)
{
    int32_t status;
    uint32_t rsp_msg_hdr, cmd_msg_hdr;
    uint32_t param_count, *parameters, message_id;
    uint32_t return_value_count, attributes, vendor_identifier[SCMI_NAME_STR_SIZE/4];

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* If DISCOVER SUB VENDOR cmd is not supported, skip the test */

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = BASE_DISCOVER_SUB_VENDOR;
    attributes = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_PROTOCOL_MESSAGE_ATTRIBUTES, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, &attributes);

    if (status == SCMI_NOT_FOUND) {
        val_print(VAL_PRINT_TEST, "\n\tBASE_SUBVENDOR_NAME not supported                     ");
        return VAL_STATUS_SKIP;
    }

    /* DISCOVER SUB VENDOR cmd is supported, query the sub vendor name */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 1] Query subvendor name");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters = NULL;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_DISCOVER_SUB_VENDOR, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, vendor_identifier);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Save the sub-vendor name */
    val_base_save_name(BASE_SUBVENDOR_NAME, (uint8_t *) vendor_identifier);
    val_print(VAL_PRINT_INFO, "\n\tSUBVENDOR NAME: %s                                 ",
                (uint8_t *) vendor_identifier);

    return VAL_STATUS_PASS;
}
