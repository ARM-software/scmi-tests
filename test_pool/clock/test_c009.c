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

#define TEST_NUM  (SCMI_CLOCK_TEST_NUM_BASE + 9)
#define TEST_DESC "Clock rate set sync mode check               "

#define PARAMETER_SIZE 4

/********* TEST ALGO ********************
 * Get the current clock rate
 * Find a new rate from previously saved rates
 * Set new rate in sync mode
 * Get the rate again and compare with newly set rates
 * Restore the default rate
*****************************************/

uint32_t clock_rate_set_sync_check(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t clock_id, num_clocks;
    uint32_t parameters[PARAMETER_SIZE], new_rate_low, new_rate_high;
    uint64_t current_rate, new_rate;
    uint32_t upper_word, lower_word;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    num_clocks = val_clock_get_info(NUM_CLOCKS, 0);
    if (num_clocks == 0) {
        val_print(VAL_PRINT_ERR, "\n       No Clock found                              ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM CLOCKS     : %d", num_clocks);

    /* Set clock rate for all clocks in sync mode */
    for (clock_id = 0; clock_id < num_clocks; clock_id++)
    {
        /* Get current rate */
        val_print(VAL_PRINT_TEST, "\n     CLOCK ID: %d", clock_id);
        val_print(VAL_PRINT_TEST, "\n     [Check 1] Get current rate");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        param_count++;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_RATE_GET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, &clock_id, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        lower_word = return_values[RATE_LOWER_WORD_OFFSET];
        upper_word = return_values[RATE_UPPER_WORD_OFFSET];
        current_rate = VAL_GET_64BIT_DATA(upper_word, lower_word);

        /* Find a new valid rate from previously saved rates*/
        if (val_clock_get_rate(CLOCK_LOWEST_RATE, clock_id) == current_rate)
            new_rate = val_clock_get_rate(CLOCK_HIGHEST_RATE, clock_id);
        else
            new_rate = val_clock_get_rate(CLOCK_LOWEST_RATE, clock_id);

        /* Set new rate in CLOCK_SET_SYNC_MODE*/
        val_print(VAL_PRINT_TEST, "\n     [Check 2] Set new rate");
        val_print(VAL_PRINT_TEST, "\n       New clock rate                       :0x%llX",
                                    new_rate);

        new_rate_low = (uint32_t)(new_rate & MASK_FOR_LOWER_WORD);
        new_rate_high = (uint32_t)((new_rate & MASK_FOR_UPPER_WORD) >> 32);

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = CLOCK_SET_SYNC_MODE;
        parameters[param_count++] = clock_id;
        parameters[param_count++] = new_rate_low;
        parameters[param_count++] = new_rate_high;;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_RATE_SET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        /* Get the rate for comparing with new rates*/
        val_print(VAL_PRINT_TEST, "\n     [Check 3] Get the rates and compare");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        param_count++;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_RATE_GET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, &clock_id, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        /* Compare with newly set rate and received rate*/
        if (val_compare("LOWER WORD OF RATE  ",
                         new_rate_low, return_values[RATE_LOWER_WORD_OFFSET]))
            return VAL_STATUS_FAIL;

        if (val_compare("UPPER WORD OF RATE  ",
                         new_rate_high, return_values[RATE_UPPER_WORD_OFFSET]))
            return VAL_STATUS_FAIL;

        val_print(VAL_PRINT_TEST, "\n     [Check 4] Set default rate");

        /* Restore the default rate */
        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = CLOCK_SET_SYNC_MODE;
        parameters[param_count++] = clock_id;
        parameters[param_count++] = (uint32_t)(current_rate & MASK_FOR_LOWER_WORD);
        parameters[param_count++] = (uint32_t)((current_rate & MASK_FOR_UPPER_WORD) >> 32);
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_RATE_SET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);
    }

    return VAL_STATUS_PASS;
}
