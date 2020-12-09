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
#include "val_clock.h"

#define TEST_NUM  (SCMI_CLOCK_TEST_NUM_BASE + 12)
#define TEST_DESC "Clock rate set invalid clock id check        "

#define PARAMETER_SIZE 4

uint32_t clock_rate_set_invalid_clock_id_check(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t clock_id;
    uint32_t parameters[PARAMETER_SIZE];

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Query with invalid id should fail with NOT_FOUND */
    clock_id = val_clock_get_info(NUM_CLOCKS, 0) + 1; /* Invalid clock id*/
    val_print(VAL_PRINT_TEST, "\n     [Check 1] Query for invalid clock_id :%d", clock_id);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = CLOCK_SET_SYNC_MODE;
    parameters[param_count++] = clock_id;
    parameters[param_count++] =
        (uint32_t)(val_clock_get_rate(CLOCK_LOWEST_RATE, clock_id) & MASK_FOR_LOWER_WORD);
    parameters[param_count++] =
        (uint32_t)((val_clock_get_rate(CLOCK_HIGHEST_RATE, clock_id) & MASK_FOR_UPPER_WORD) >> 32);

    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_RATE_SET, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
