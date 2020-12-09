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
#include "val_performance.h"

#define TEST_NUM  (SCMI_PERFORMANCE_TEST_NUM_BASE + 7)
#define TEST_DESC "Performance describe levels check            "

#define PARAMETER_SIZE 2
#define PERF_LEVEL_ARRAY_COUNT 3

uint32_t performance_query_describe_levels(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t domain_id, num_domains, num_remaining_levels, num_levels_retured;
    uint32_t i, perf_level, power_cost, latency, level_index;
    uint32_t min_perf_level, max_perf_level;
    uint32_t parameters[PARAMETER_SIZE];
    uint32_t *perf_level_array;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Check performance describe levels for valid domain */
    num_domains = val_performance_get_info(NUM_PERF_DOMAINS, 0x00);
    if (num_domains == 0) {
        val_print(VAL_PRINT_ERR, "\n       No performance domains found                ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM DOMAINS    : %d", num_domains);

    for (domain_id = 0; domain_id < num_domains; domain_id++)
    {
        level_index = 0;
        num_remaining_levels = 0;
        num_levels_retured  = 0;
        min_perf_level = 0xFFFFFFFF;
        max_perf_level = 0;

        val_print(VAL_PRINT_TEST, "\n     PERFORMANCE DOMAIN ID: %d", domain_id);
        val_print(VAL_PRINT_TEST, "\n     [Check 1] Query describe levels");

        do
        {
            VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
            parameters[param_count++] = domain_id;
            level_index += num_levels_retured;
            parameters[param_count++] = level_index;

            cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_DESCRIBE_LEVELS,
                                             COMMAND_MSG);
            val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

            if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            val_print_return_values(return_value_count, return_values);

            num_remaining_levels = VAL_EXTRACT_BITS(return_values[NUM_LEVEL_OFFSET], 16, 31);
            val_print(VAL_PRINT_DEBUG, "\n       NUM OF REMAINING LEVELS: %d",
                                         num_remaining_levels);

            if (val_reserved_bits_check_is_zero(
                VAL_EXTRACT_BITS(return_values[NUM_LEVEL_OFFSET], 12, 15)) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            num_levels_retured = VAL_EXTRACT_BITS(return_values[NUM_LEVEL_OFFSET], 0, 11);
            val_print(VAL_PRINT_DEBUG, "\n       NUM OF LEVELS RETURNED : %d", num_levels_retured);

            perf_level_array = &return_values[PERF_LEVEL_ARRAY_OFFSET];
            for (i = 0; i < num_levels_retured; i++)
            {
                perf_level = perf_level_array[i*PERF_LEVEL_ARRAY_COUNT];
                val_print(VAL_PRINT_DEBUG, "\n       PERFORMANCE LEVEL[%d]      : %08x",
                                            level_index + i, perf_level);

                if (min_perf_level > perf_level)
                    min_perf_level = perf_level;

                if (max_perf_level < perf_level)
                    max_perf_level = perf_level;

                /* Store a intermediate value*/
                if (i == 1)
                     val_performance_save_info(PERF_DOMAIN_INTERMEDIATE_LEVEL, domain_id,
                                               perf_level);

                power_cost = perf_level_array[1 + (i * PERF_LEVEL_ARRAY_COUNT)];
                val_print(VAL_PRINT_DEBUG, "\n       PERFORMANCE POWER COST[%d] : %08x",
                                            level_index + i, power_cost);

                latency = VAL_EXTRACT_BITS(perf_level_array[2 + (i * PERF_LEVEL_ARRAY_COUNT)],
                                                           0, 15);
                val_print(VAL_PRINT_DEBUG, "\n       PERFORMANCE LATENCY[%d]    : %08x",
                                            level_index + i, latency);

                if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(
                    perf_level_array[2 + (i * PERF_LEVEL_ARRAY_COUNT)], 16, 31)) != VAL_STATUS_PASS)
                    return VAL_STATUS_FAIL;
            }
        } while (num_remaining_levels > 0);

        if (val_compare("NUM OF LEVELS       ", (level_index + num_levels_retured),
                        val_performance_get_expected_number_of_level(domain_id)))
            return VAL_STATUS_FAIL;

        val_performance_save_info(PERF_DOMAIN_MAX_LEVEL, domain_id, max_perf_level);
        val_performance_save_info(PERF_DOMAIN_MIN_LEVEL, domain_id, min_perf_level);

        /* Performance domain attributes for invalid level index */
        val_print(VAL_PRINT_TEST, "\n     [Check 2] Query with invalid level_index ");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = domain_id;
        parameters[param_count++] = (level_index + num_levels_retured) + 1; /*Invalid level index*/
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_DESCRIBE_LEVELS,
                                         COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_INVALID_PARAMETERS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;
    }

    return VAL_STATUS_PASS;
}
