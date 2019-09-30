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

#define TEST_NUM  (SCMI_BASE_TEST_NUM_BASE + 9)
#define TEST_DESC "Base query protocol list                               "
#define MAX_RETURN_VALUE_SIZE 4

uint32_t base_query_protocol_list(void)
{
    int32_t status;
    uint32_t rsp_msg_hdr, cmd_msg_hdr;
    uint32_t param_count, skip, num_protocols;
    uint32_t return_value_count, return_values[MAX_RETURN_VALUE_SIZE];

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Query list of all supported protocols */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 1] Send valid skip value 0");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    skip = 0;
    param_count++;
    val_memset_zero(return_values, MAX_RETURN_VALUE_SIZE);
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_DISCOVER_LIST_PROTOCOLS, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &skip, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    num_protocols = val_base_get_info(BASE_NUM_PROTOCOLS);
    if (num_protocols == return_values[0])
        val_print(VAL_PRINT_DEBUG, "\n\tCHECK NUM PROTOCOLS: PASSED [%d]", num_protocols);
    else {
        val_print(VAL_PRINT_ERR, "\n\tCHECK NUM PROTOCOLS: FAILED                            ");
        return VAL_STATUS_FAIL;
    }

    /* DISCOVER_LIST_PROTOCOLS with invalid skip value should return INVALID PARAMETERS status */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 2] Send invalid skip value");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    skip = 8; /* skip value 8 is invalid as SCMI v2 specifications has only 7 protocols */
    param_count++;
    val_memset_zero(return_values, MAX_RETURN_VALUE_SIZE);
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_DISCOVER_LIST_PROTOCOLS, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &skip, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_INVALID_PARAMETERS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
