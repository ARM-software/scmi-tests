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

#define TEST_NUM  (SCMI_PERFORMANCE_TEST_NUM_BASE + 2)
#define TEST_DESC "Performance Domain Protocol Attributes check           "
#define MAX_RETURN_VALUE_SIZE 4

uint32_t performance_query_protocol_attributes(void)
{
    int32_t status;
    uint32_t rsp_msg_hdr, cmd_msg_hdr;
    uint32_t param_count, *parameters;
    uint32_t return_value_count, return_values[MAX_RETURN_VALUE_SIZE];
    uint32_t attributes, stats_addr_low, stats_addr_high, stats_len;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Query performance protocol and check num of performance domains */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 1] Query protocol attributes");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters = NULL; /* No parameters for this command */
    val_memset_zero(return_values, MAX_RETURN_VALUE_SIZE);
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_PROTOCOL_ATTRIBUTES,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    attributes = return_values[0];
    if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attributes, 17, 31) != VAL_STATUS_PASS))
        return VAL_STATUS_FAIL;

    /* Save the number of performance domains */
    val_performance_save_info(NUM_PERF_DOMAINS, VAL_EXTRACT_BITS(attributes, 0, 15));
    val_print(VAL_PRINT_INFO, "\n\tNUM PERFORMANCE DOMAINS: %d                             ",
                             VAL_EXTRACT_BITS(attributes, 0, 15));

    /* Save the performance statitcs memory lower address */
    stats_addr_low = return_values[1];
    val_performance_save_info(PERF_STATS_ADDR_LOW, stats_addr_low);
    /* Save the performance statitcs memory higher address */
    stats_addr_high = return_values[2];
    val_performance_save_info(PERF_STATS_ADDR_HIGH, stats_addr_high);
    /* Save the performance statitcs memory lower address */
    stats_len = return_values[3];
    val_performance_save_info(PERF_STATS_ADDR_LEN, stats_len);

    return VAL_STATUS_PASS;
}
