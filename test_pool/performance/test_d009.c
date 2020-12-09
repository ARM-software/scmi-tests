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

#define TEST_NUM  (SCMI_PERFORMANCE_TEST_NUM_BASE + 9)
#define TEST_DESC "Performance limits set change limit check    "
#define PARAMETER_SIZE 3

/********* TEST ALGO ********************
 * Get the default level
 * Set a new level
 * Get level & compare whether set level is success
*****************************************/

uint32_t performance_query_set_limit(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t domain_id, num_domains;
    uint32_t parameters[PARAMETER_SIZE];
    uint32_t range_max, range_min, temp_max;
    uint32_t run_flag = 0;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Check performance set levels for valid domain */
    num_domains = val_performance_get_info(NUM_PERF_DOMAINS, 0x00);
    if (num_domains == 0) {
        val_print(VAL_PRINT_ERR, "\n       No performance domains found                ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM DOMAINS    : %d", num_domains);

    for (domain_id = 0; domain_id < num_domains; domain_id++)
    {
        if (val_performance_get_info(PERF_DOMAIN_SET_PERFORMANCE_LIMIT_SUPPORT, domain_id) == 0)
            continue;
        run_flag = 1;

        val_print(VAL_PRINT_TEST, "\n     PERFORMANCE DOMAIN ID: %d", domain_id);
        val_print(VAL_PRINT_TEST, "\n     [Check 1] Get performance limits ");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        param_count++;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_LIMITS_GET,
                                         COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, &domain_id, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        range_max = return_values[RANGE_MAX_OFFSET];
        range_min = return_values[RANGE_MIN_OFFSET];
        if (range_max != val_performance_get_info(PERF_DOMAIN_MAX_LEVEL, domain_id))
            temp_max = val_performance_get_info(PERF_DOMAIN_MAX_LEVEL, domain_id);
        else
            temp_max = val_performance_get_info(PERF_DOMAIN_INTERMEDIATE_LEVEL, domain_id);

        val_print(VAL_PRINT_TEST, "\n    [Check 2] Change performance limits ");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = domain_id;
        parameters[param_count++] = temp_max;
        parameters[param_count++] = range_min;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_LIMITS_SET,
                                         COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print(VAL_PRINT_TEST, "\n    [Check 3] Verify the changed perf limit");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        param_count++;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_LIMITS_GET,
                                         COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, &domain_id, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        if (val_compare("RANGE_MAX", return_values[RANGE_MAX_OFFSET], temp_max))
            return VAL_STATUS_FAIL;

        if (val_compare("RANGE_MIN", return_values[RANGE_MIN_OFFSET], range_min))
            return VAL_STATUS_FAIL;

        val_print(VAL_PRINT_TEST, "\n    [Check 4] Restore original performance limits");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = domain_id;
        parameters[param_count++] = range_max;
        parameters[param_count++] = range_min;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_LIMITS_SET,
                                         COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

    }
    if (run_flag == 0) {
        val_print(VAL_PRINT_ERR, "\n       No domain support PERF SET LIMIT            ");
        return VAL_STATUS_SKIP;
    }
    return VAL_STATUS_PASS;
}
