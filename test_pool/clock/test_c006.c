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

#define TEST_NUM  (SCMI_CLOCK_TEST_NUM_BASE + 6)
#define TEST_DESC "Clock config set check                                 "
#define MAX_PARAMETER_SIZE 2

uint32_t clock_config_set_check(void)
{
    int32_t status;
    uint32_t rsp_msg_hdr, cmd_msg_hdr;
    uint32_t param_count, clock_id, parameters[MAX_PARAMETER_SIZE];
    uint32_t return_value_count, return_values;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* clock config set for invalid clock id should return NOT_FOUND status */

    clock_id = val_clock_get_info(NUM_CLOCKS, 0) + 1;
    val_print(VAL_PRINT_DEBUG, "\n\t[Check 1] Query with invalid clock_id %d", clock_id);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = clock_id;
    parameters[param_count++] = 0x01; /* Enable clock */
    return_values = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_CONFIG_SET, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, &return_values);

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Clock config set with invalid attributes */

    clock_id = 0;
    val_print(VAL_PRINT_DEBUG, "\n\t[Check 2] Query with invalid attributes");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = clock_id;
    parameters[param_count++] = 0x04; /* Invalid attributes value */
    return_values = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_CONFIG_SET, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, &return_values);

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_status(status, SCMI_INVALID_PARAMETERS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
