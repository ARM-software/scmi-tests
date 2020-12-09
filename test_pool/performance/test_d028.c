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

#define TEST_NUM  (SCMI_PERFORMANCE_TEST_NUM_BASE + 28)
#define TEST_DESC "Performance level get via fast channel       "

#define PARAMETER_SIZE 2

uint32_t performance_level_get_fast_channel(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t domain_id, num_domains, message_id, attributes;
    uint32_t parameters[PARAMETER_SIZE], perf_level;
    uint32_t chl_addr_low, chl_addr_high ;
    uint32_t run_flag = 0;
    uint64_t chl_addr = 0x0;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Skip if describe fast channel is not supported*/
    val_print(VAL_PRINT_TEST, "\n     [Check 1] Query performance describe fast channel support");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = PERFORMANCE_DESCRIBE_FASTCHANNEL;
    param_count++;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_PROTOCOL_MESSAGE_ATTRIBUTES,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (status == SCMI_NOT_FOUND) {
        val_print(VAL_PRINT_ERR, "\n       PERF_DESCRIBE_FASTCHANNEL not supported     ");
        return VAL_STATUS_SKIP;
    }

    /* Skip if no performance domains found */
    num_domains = val_performance_get_info(NUM_PERF_DOMAINS, 0x00);
    if (num_domains == 0) {
        val_print(VAL_PRINT_ERR, "\n       No performance domains found                ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM DOMAINS    : %d", num_domains);

    for (domain_id = 0; domain_id < num_domains; domain_id++)
    {
        /* Continue till we find domain which supports fastchannel */
        if (val_performance_get_info(PERF_DOMAIN_FAST_CH_SUPPORT, domain_id) == 0)
            continue;

        val_print(VAL_PRINT_TEST, "\n     FAST CHANNEL supported for DOMAIN: %d",domain_id);

        message_id = PERFORMANCE_LEVEL_GET;
        /* if perf level get doesn't support fastchannel, skip */
        if (val_performance_get_info(PERF_MESSAGE_FAST_CH_SUPPORT, message_id))
        {
            run_flag = 1;
            val_print(VAL_PRINT_TEST, "\n     FAST CHANNEL supported for perf level get");
            val_print(VAL_PRINT_TEST, "\n     [Check 1] Get fast chl details for perf level get");

            VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
            parameters[param_count++] = domain_id;
            parameters[param_count++] = message_id;
            cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_DESCRIBE_FASTCHANNEL,
                                             COMMAND_MSG);
            val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                             &return_value_count, return_values);

            if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            val_print_return_values(return_value_count, return_values);

            attributes = return_values[ATTRIBUTE_OFFSET];
            if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attributes, 3, 31)) !=
                                                VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            /* Doorbell should not be supported for perf level get 4.5.2 */
            if (val_compare("DOORBELL SUPPORT", VAL_EXTRACT_BITS(attributes, 0, 0), NOT_SUPPORTED))
                return VAL_STATUS_FAIL;

            chl_addr_low = return_values[CHAN_ADDR_LOW_OFFSET];
            chl_addr_high = return_values[CHAN_ADDR_HIGH_OFFSET];
            chl_addr = (uint64_t) chl_addr_high << 32 | chl_addr_low;

            if (chl_addr_high)
                perf_level = *(volatile uint64_t *)chl_addr;
            else
                perf_level = *(volatile uint32_t*)chl_addr;

            /* Get perf level from normal channel and and compare  */
            val_print(VAL_PRINT_TEST, "\n     [Check 2] Get perf level via normal channel");

            VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
            param_count++;
            cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_LEVEL_GET,
                                             COMMAND_MSG);
            val_send_message(cmd_msg_hdr, param_count, &domain_id, &rsp_msg_hdr, &status,
                             &return_value_count, return_values);

           if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

           if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
                return VAL_STATUS_FAIL;

            val_print_return_values(return_value_count, return_values);

            if (val_compare("LEVEL    ", return_values[PERFORMANCE_LEVEL_OFFSET], perf_level))
                return VAL_STATUS_FAIL;

        }
    }
    if (run_flag == 0) {
        val_print(VAL_PRINT_ERR, "\n       No domain/msg support FAST CHANNEL           ");
        return VAL_STATUS_SKIP;
    }
    return VAL_STATUS_PASS;
}
