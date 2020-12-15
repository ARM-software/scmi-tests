/** @file
 * Copyright (c) 2019-2020, Arm Limited or its affiliates. All rights reserved.
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

#define TEST_NUM  (SCMI_PERFORMANCE_TEST_NUM_BASE + 2)
#define TEST_DESC "Performance protocol attributes check        "

uint32_t performance_query_protocol_attributes(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t *parameters;
    uint32_t attributes, num_perf_domains, stats_addr_low, stats_addr_high, stats_len;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Query performance protocol and check num of performance domains */
    val_print(VAL_PRINT_TEST, "\n     [Check 1] Query protocol attributes");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    /* No parameters for this command */
    parameters = NULL;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_PROTOCOL_ATTRIBUTES,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    attributes = return_values[ATTRIBUTE_OFFSET];
    if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attributes, 17, 31) != VAL_STATUS_PASS))
        return VAL_STATUS_FAIL;

    val_print(VAL_PRINT_INFO, "\n       PWR VAL in Mw  : %d", VAL_EXTRACT_BITS(attributes, 16, 16));

    /* Compare & save the number of performance domains */
    num_perf_domains = VAL_EXTRACT_BITS(attributes, 0, 15);
    if (val_compare("NUM PERF DOMAINS", num_perf_domains,
                     val_performance_get_expected_num_domains()))
        return VAL_STATUS_FAIL;
    val_performance_save_info(NUM_PERF_DOMAINS, 0x00, VAL_EXTRACT_BITS(attributes, 0, 15));

    /* Compare & save  the performance statistics memory lower address */
    stats_addr_low = return_values[STATS_ADDR_LOW_OFFSET];
    if (val_compare("STATS ADDR LOW  ", stats_addr_low,
                    val_performance_get_expected_stats_addr_low()))
        return VAL_STATUS_FAIL;
    val_performance_save_info(PERF_STATS_ADDR_LOW, 0x00, stats_addr_low);

    /* Compare & save the performance statistics memory higher address */
    stats_addr_high = return_values[STATS_ADDR_HIGH_OFFSET];
    if (val_compare("STATS ADDR HIGH ", stats_addr_high,
                     val_performance_get_expected_stats_addr_high()))
        return VAL_STATUS_FAIL;
    val_performance_save_info(PERF_STATS_ADDR_HIGH, 0x00, stats_addr_high);

    /* Compare & save the performance statistics memory length */
    stats_len = return_values[STATS_LENGTH_OFFSET];
    if (val_compare("STATS ADDR LEN  ", stats_len, val_performance_get_expected_stats_addr_len()))
        return VAL_STATUS_FAIL;
    val_performance_save_info(PERF_STATS_ADDR_LEN, 0x00, stats_len);

    return VAL_STATUS_PASS;
}
