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

#define TEST_NUM  (SCMI_CLOCK_TEST_NUM_BASE + 11)
#define TEST_DESC "Clock rate set invalid param check           "

#define PARAMETER_SIZE 4

uint32_t clock_rate_set_invalid_param_check(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t clock_id, num_clocks;
    uint32_t parameters[PARAMETER_SIZE];
    uint32_t upper_word, lower_word;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    num_clocks = val_clock_get_info(NUM_CLOCKS, 0);
    if (num_clocks == 0) {
        val_print(VAL_PRINT_ERR, "\n       No Clock found                              ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM CLOCKS     : %d", num_clocks);

    for (clock_id = 0; clock_id < num_clocks; clock_id++)
    {
        val_print(VAL_PRINT_TEST, "\n     CLOCK ID: %d", clock_id);
        val_print(VAL_PRINT_TEST, "\n     [Check 1] Query with invalid rate");

        upper_word = (uint32_t)((val_clock_get_rate(CLOCK_HIGHEST_RATE, clock_id) &
                                MASK_FOR_UPPER_WORD) >> 32);
        lower_word = (uint32_t)(val_clock_get_rate(CLOCK_LOWEST_RATE, clock_id) &
                                MASK_FOR_LOWER_WORD);

        /* Query with invalid rate should fail with INVALID_PARAMETERS*/
        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = CLOCK_SET_SYNC_MODE;
        parameters[param_count++] = clock_id;
        /* Invalid rates*/
        parameters[param_count++] = lower_word + 1;
        parameters[param_count++] = upper_word + 1;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_RATE_SET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_INVALID_PARAMETERS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print(VAL_PRINT_TEST, "\n       [Check 2] Query with invalid flag");

        /* Query with invalid flag should fail with INVALID_PARAMETERS*/
        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = CLOCK_SET_INVALID_FLAG;
        parameters[param_count++] = clock_id;
        parameters[param_count++] = lower_word;
        parameters[param_count++] = upper_word;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_RATE_SET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_INVALID_PARAMETERS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;
    }

    return VAL_STATUS_PASS;
}
