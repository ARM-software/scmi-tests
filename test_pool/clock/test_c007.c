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

#define TEST_NUM  (SCMI_CLOCK_TEST_NUM_BASE + 7)
#define TEST_DESC "Clock describe rates check                   "

#define PARAMETER_SIZE 2

uint32_t clock_query_describe_rates(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t clock_id, num_clocks, num_remaining_rates;
    uint32_t i, return_format, num_rates_retured, rate_index;
    uint32_t parameters[PARAMETER_SIZE];
    uint64_t clock_rate, clock_rate_higher, step_size;
    uint32_t upper_word, lower_word, num_rate_flag;
    uint32_t *rate_array;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    num_clocks = val_clock_get_info(NUM_CLOCKS, 0);
    if (num_clocks == 0) {
        val_print(VAL_PRINT_ERR, "\n       No Clock found                              ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM CLOCKS     : %d", num_clocks);

    /* Check Clock describe rate for valid clocks */

    for (clock_id = 0; clock_id < num_clocks; clock_id++)
    {
        rate_index = 0;
        num_remaining_rates = 0;
        num_rates_retured = 0;

        val_print(VAL_PRINT_TEST, "\n     CLOCK ID: %d", clock_id);
        val_print(VAL_PRINT_TEST, "\n     [Check 1] Query describe rates", clock_id);
        /*  Get rate details by sending CLOCK_DESCRIBE_RATES repeatedly until,
           all rates got received */
        do
        {
            VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
            parameters[param_count++] = clock_id;
            rate_index += num_rates_retured;
            parameters[param_count++] = rate_index;

            cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_DESCRIBE_RATES, COMMAND_MSG);
            val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                             &return_value_count, return_values);

            if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            val_print_return_values(return_value_count, return_values);

            num_rate_flag = return_values[RATE_FLAG_OFFSET];

            num_remaining_rates = VAL_EXTRACT_BITS(num_rate_flag, 16, 31);
            val_print(VAL_PRINT_DEBUG, "\n       NUM OF REMAINING RATES : %d", num_remaining_rates);

            if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(num_rate_flag, 13, 15)) !=
                                                VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            return_format =  VAL_EXTRACT_BITS(num_rate_flag, 12, 12);
            val_print(VAL_PRINT_DEBUG, "\n       Return format          : %d", return_format);

            num_rates_retured = VAL_EXTRACT_BITS(num_rate_flag, 0, 11);
            val_print(VAL_PRINT_DEBUG, "\n       NUM OF RATES RETURNED  : %d", num_rates_retured);

            /* Rate array starts from return_values[1]*/
            rate_array = &return_values[RATES_OFFSET];

            for (i = 0; i < num_rates_retured; i++)
            {
                val_print(VAL_PRINT_DEBUG, "\n       Rate Index             : %d", rate_index + i);

                if (return_format == CLOCK_RATE_FMT_ARRAY) {
                    lower_word = rate_array[i * 2];
                    upper_word = rate_array[1 + i * 2];

                    clock_rate = VAL_GET_64BIT_DATA(upper_word, lower_word);
                    val_print(VAL_PRINT_DEBUG, "\n       CLOCK RATE             : 0x%llX",
                            clock_rate);
                    /* Save first & last value*/
                    if (i == 0) {
                        val_clock_save_rate(CLOCK_LOWEST_RATE,
                                clock_id, clock_rate);
                    }

                    if (i == (num_rates_retured - 1)) {
                        val_clock_save_rate(CLOCK_HIGHEST_RATE,
                                clock_id, clock_rate);
                    }
                } else {
                    /* Find lowest clock rate from rate array */
                    lower_word = rate_array[i * 6];
                    upper_word = rate_array[1 + i * 6];
                    clock_rate = VAL_GET_64BIT_DATA(upper_word, lower_word);

                    /* Find highest clock rate from rate array */
                    lower_word = rate_array[2 + i * 6];
                    upper_word = rate_array[3 + i * 6];
                    clock_rate_higher = VAL_GET_64BIT_DATA(upper_word, lower_word);

                    /* Find step size from rate array */
                    lower_word = rate_array[4 + i * 6];
                    upper_word = rate_array[5 + i * 6];
                    step_size = VAL_GET_64BIT_DATA(upper_word, lower_word);

                    val_print(VAL_PRINT_DEBUG, "\n       LOWEST RATE            : 0x%llX",
                            clock_rate);
                    val_print(VAL_PRINT_DEBUG, "\n       HIGHEST RATE           : 0x%llX",
                            clock_rate_higher);
                    val_print(VAL_PRINT_DEBUG, "\n       STEP SIZE              : 0x%llX",
                            step_size);

                    /* Store lowest & highest rates*/
                    val_clock_save_rate(CLOCK_LOWEST_RATE,
                                        clock_id, clock_rate);
                    val_clock_save_rate(CLOCK_HIGHEST_RATE,
                                        clock_id, clock_rate_higher);
                }
            }
        } while (num_remaining_rates > 0);

        if (val_compare("NUM OF RATES        ", (rate_index + num_rates_retured),
                        val_clock_get_expected_number_of_rates(clock_id)))
            return VAL_STATUS_FAIL;

        /* For CLOCK_RATE_FMT_STEP_SIZE , rate index is ignored*/
        if (return_format == CLOCK_RATE_FMT_STEP_SIZE) {
            continue;
        }

        /* Clock describe rate for invalid rate index */
        rate_index += num_rates_retured;
        val_print(VAL_PRINT_TEST, "\n     [Check 2] Query with invalid index: %d", rate_index);

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = clock_id;
        parameters[param_count++] = rate_index; /*Invalid rate index*/
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_DESCRIBE_RATES, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_OUT_OF_RANGE) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;
    }

    return VAL_STATUS_PASS;
}
