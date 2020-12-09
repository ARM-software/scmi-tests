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

#define TEST_NUM  (SCMI_PERFORMANCE_TEST_NUM_BASE + 5)
#define TEST_DESC "Performance domain attributes check          "

uint32_t performance_query_domain_attributes_v1(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t domain_id, num_domains, attribute;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Check performance domain attributes for valid domain */
    num_domains = val_performance_get_info(NUM_PERF_DOMAINS, 0x00);
    if (num_domains == 0) {
        val_print(VAL_PRINT_ERR, "\n       No performance domains found                ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM DOMAINS    : %d", num_domains);

    for (domain_id = 0; domain_id < num_domains; domain_id++)
    {
        val_print(VAL_PRINT_TEST, "\n     PERFORMANCE DOMAIN ID: %d", domain_id);
        val_print(VAL_PRINT_TEST, "\n     [Check 1] Query attributes");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        param_count++;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_DOMAIN_ATTRIBUTES,
                                         COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, &domain_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        attribute = return_values[ATTRIBUTE_OFFSET];
        if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attribute, 0, 27)) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print(VAL_PRINT_INFO, "\n       SET LIMIT SUPPORT         : %d",
                                   VAL_EXTRACT_BITS(attribute, 31, 31));
        val_performance_save_info(PERF_DOMAIN_SET_PERFORMANCE_LIMIT_SUPPORT, domain_id,
                                   VAL_EXTRACT_BITS(attribute, 31, 31));

        val_print(VAL_PRINT_INFO, "\n       SET LEVEL SUPPORT         : %d",
                                   VAL_EXTRACT_BITS(attribute, 30, 30));
        val_performance_save_info(PERF_DOMAIN_SET_PERFORMANCE_LEVEL_SUPPORT, domain_id,
                                  VAL_EXTRACT_BITS(attribute, 30, 30));

        val_print(VAL_PRINT_INFO, "\n       LIMIT CHANGE NOTIFY SUPPORT: %d",
                                   VAL_EXTRACT_BITS(attribute, 29, 29));
        val_performance_save_info(PERF_DOMAIN_LIMIT_CHANGE_NTFY_SUPPORT, domain_id,
                                  VAL_EXTRACT_BITS(attribute, 29, 29));

        val_print(VAL_PRINT_INFO, "\n       LEVEL CHANGE NOTIFY SUPPORT: %d",
                                   VAL_EXTRACT_BITS(attribute, 28, 28));
        val_performance_save_info(PERF_DOMAIN_LVL_CHANGE_NTFY_SUPPORT, domain_id,
                                  VAL_EXTRACT_BITS(attribute, 28, 28));

        if (val_reserved_bits_check_is_zero(
            VAL_EXTRACT_BITS(return_values[RATE_LIMIT_OFFSET], 20, 31)) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print(VAL_PRINT_INFO, "\n       RATE LIMIT                : %d",
                                   VAL_EXTRACT_BITS(return_values[RATE_LIMIT_OFFSET], 0, 19));
        val_print(VAL_PRINT_INFO, "\n       SUSTAINED FREQENCY        : %d",
                                   return_values[FREQUENCY_OFFSET]);
        val_print(VAL_PRINT_INFO, "\n       SUSTAINED LEVEL           : %d",
                                   return_values[SUSTAINED_LEVEL_OFFSET]);

        if (val_compare_str("DOMAIN NAME", (char *)&return_values[NAME_OFFSET],
                        (char *)val_performance_get_expected_name(domain_id), SCMI_NAME_STR_SIZE))
            return VAL_STATUS_FAIL;
    }

    return VAL_STATUS_PASS;
}
