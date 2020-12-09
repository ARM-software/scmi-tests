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

#define TEST_NUM  (SCMI_CLOCK_TEST_NUM_BASE + 10)
#define TEST_DESC "Clock rate set async mode check              "

#define PARAMETER_SIZE 4

/********* TEST ALGO ********************
 * Get the current clock rate
 * Find a new rate from previously saved rates
 * Set new rate in async mode
 * Wait for delayed response & compare with newly set rates
 * Restore the default rate
*****************************************/

uint32_t clock_rate_set_async_check(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t clock_id, num_clocks, num_async;
    uint32_t parameters[PARAMETER_SIZE];
    uint64_t current_rate, new_rate;
    uint32_t new_rate_low, new_rate_high;
    uint32_t upper_word, lower_word;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Check whether platform support async mode */
    num_async = val_clock_get_info(CLOCK_MAX_PENDING_ASYNC_CMD, 0);
    if (num_async == 0) {
        val_print(VAL_PRINT_ERR, "\n       Async rate change is not supported          ");
        return VAL_STATUS_SKIP;
    }

    /* Set clock rate for all clocks in async mode */
    num_clocks = val_clock_get_info(NUM_CLOCKS, 0);
    if (num_clocks == 0) {
        val_print(VAL_PRINT_ERR, "\n       No Clock found                              ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM CLOCKS     : %d", num_clocks);

    for (clock_id = 0; clock_id < num_clocks; clock_id++)
    {
        /* Get current rate using CLOCK_RATE_GET*/
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

        /* Find a new rate from previously stored rates*/
        if (val_clock_get_rate(CLOCK_LOWEST_RATE, clock_id) == current_rate)
            new_rate = val_clock_get_rate(CLOCK_HIGHEST_RATE, clock_id);
        else
            new_rate = val_clock_get_rate(CLOCK_LOWEST_RATE, clock_id);

        val_print(VAL_PRINT_TEST, "\n     [Check 2] Set new rate");
        val_print(VAL_PRINT_TEST, "\n       New clock rate                       :0x%llX",
                new_rate);

        new_rate_low = (uint32_t)(new_rate & MASK_FOR_LOWER_WORD);
        new_rate_high = (uint32_t)((new_rate & MASK_FOR_UPPER_WORD) >> 32);

        /* Set rate with CLOCK_SET_ASYNC_MODE */
        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = CLOCK_SET_ASYNC_MODE;
        parameters[param_count++] = clock_id;
        parameters[param_count++] = new_rate_low;
        parameters[param_count++] = new_rate_high;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_RATE_SET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        /* Wait for delayed response */
        val_print(VAL_PRINT_TEST, "\n     [Check 3] Wait for delayed response");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        val_receive_delayed_response(&rsp_msg_hdr, &status, &return_value_count, return_values);

        val_print(VAL_PRINT_TEST, "\n       NOTIFY MSG HDR   : 0x%08x", rsp_msg_hdr);
        if (val_compare("PROTOCOL ID", VAL_EXTRACT_BITS(rsp_msg_hdr, 10, 17),
                        PROTOCOL_CLOCK))
            return VAL_STATUS_FAIL;
        if (val_compare("MSG TYPE   ", VAL_EXTRACT_BITS(rsp_msg_hdr, 8, 9),
                         DELAYED_RESPONSE_MSG))
            return VAL_STATUS_FAIL;
        if (val_compare("MSG ID     ", VAL_EXTRACT_BITS(rsp_msg_hdr, 0, 7),
                         CLOCK_RATE_SET_COMPLETE))
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        /* Compare send & received clock id*/
        if (val_compare("CLOCK ID           ",
                         clock_id, return_values[DELAYED_RESP_CLOCK_ID_OFFSET]))
            return VAL_STATUS_FAIL;

        /* Compare with new rate & received rate*/
        if (val_compare("LOWER WORD OF RATE  ",
                         new_rate_low, return_values[DELAYED_RESP_RATE_LOWER_OFFSET]))
            return VAL_STATUS_FAIL;

        if (val_compare("UPPER WORD OF RATE  ",
                         new_rate_high, return_values[DELAYED_RESP_RATE_UPPER_OFFSET]))
            return VAL_STATUS_FAIL;

        /* Restore the default rate */
        val_print(VAL_PRINT_TEST, "\n     [Check 4] Set default rate");

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
