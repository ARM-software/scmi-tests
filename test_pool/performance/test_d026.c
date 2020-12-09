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

#define TEST_NUM  (SCMI_PERFORMANCE_TEST_NUM_BASE + 26)
#define TEST_DESC "Perf limits changed notification check       "

/********* TEST ALGO ********************
 * Get the domain which has limit change noti and set limit support
 * Enable limit notification
 * Get allowed performance levels for the domain
 * Set new performance limit
 * Check if notification of new limit is received
 * Disable notification
 * Restore the default limits
*****************************************/

uint32_t performance_limit_set_async(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t domain_id, num_domains, attribute;
    uint32_t range_max, range_min, tmp_max;
    uint32_t parameters[3], set_limit_support = 0, limit_notify_support = 0;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Check for set limit and notify limit change support for each domain */
    num_domains = val_performance_get_info(NUM_PERF_DOMAINS, 0x00);
    if (num_domains == 0) {
        val_print(VAL_PRINT_ERR, "\n       No performance domains found                ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM DOMAINS    : %d", num_domains);

    for (domain_id = 0; domain_id < num_domains; domain_id++)
    {
        set_limit_support = 0;
        limit_notify_support = 0;

        val_print(VAL_PRINT_TEST, "\n     PERFORMANCE DOMAIN ID: %d", domain_id);
        val_print(VAL_PRINT_TEST,
                  "\n     [Check 1] Find set limit & limit notify supported domain");
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
        if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attribute, 0, 26)) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        set_limit_support = VAL_EXTRACT_BITS(attribute, 31, 31);
        val_print(VAL_PRINT_TEST, "\n       SET LIMIT SUPPORT: %d", set_limit_support);
        limit_notify_support = VAL_EXTRACT_BITS(attribute, 29, 29);
        val_print(VAL_PRINT_TEST, "\n       LIMIT CHG NOTIFY SUPPORT: %d", limit_notify_support);
        if (set_limit_support && limit_notify_support)
            break;
    }

    if ((set_limit_support && limit_notify_support) == 0) {
        val_print(VAL_PRINT_ERR, "\n       No domain has both set/notify limit support ");
        return VAL_STATUS_SKIP;
    }

    /* Enable limit change notification for the domain */
    val_print(VAL_PRINT_TEST, "\n     [Check 2] Enable limit change notify for domain_id: %d",
                                domain_id);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = domain_id;
    parameters[param_count++] = NOTIFY_ENABLE;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_NOTIFY_LIMITS, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Get the allowed performance limits for the domain */
    val_print(VAL_PRINT_TEST, "\n     [Check 3] Get current limits of domain_id: %d", domain_id);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = domain_id;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_LIMITS_GET,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    range_max = return_values[RANGE_MAX_OFFSET];
    range_min = return_values[RANGE_MIN_OFFSET];
    if (range_max != val_performance_get_info(PERF_DOMAIN_MAX_LEVEL, domain_id))
        tmp_max = val_performance_get_info(PERF_DOMAIN_MAX_LEVEL, domain_id);
    else
        tmp_max = val_performance_get_info(PERF_DOMAIN_INTERMEDIATE_LEVEL, domain_id);

    /* Change the performance limit */
    val_print(VAL_PRINT_TEST, "\n     [Check 4] Change the range_max limit from: " \
           "%08x => %08x", range_max, tmp_max);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = domain_id;
    parameters[param_count++] = tmp_max;
    parameters[param_count++] = range_min;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_LIMITS_SET, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Wait for notification */
    val_print(VAL_PRINT_TEST, "\n     [Check 5] Wait for limit change notify");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    val_receive_notification(&rsp_msg_hdr, &return_value_count, return_values);

    val_print(VAL_PRINT_TEST, "\n       NOTIFY MSG HDR   : %08x", rsp_msg_hdr);
    if (val_compare("PROTOCOL ID", VAL_EXTRACT_BITS(rsp_msg_hdr, 10, 17), PROTOCOL_PERFORMANCE))
        return VAL_STATUS_FAIL;
    if (val_compare("MSG TYPE   ", VAL_EXTRACT_BITS(rsp_msg_hdr, 8, 9), NOTIFICATION_MSG))
        return VAL_STATUS_FAIL;
    if (val_compare("MSG ID     ", VAL_EXTRACT_BITS(rsp_msg_hdr, 0, 7), PERFORMANCE_LIMITS_CHANGED))
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);
    if (val_compare("DOMAIN ID   ", return_values[NOTIFICATION_DOMAIN_ID_OFFSET], domain_id))
        return VAL_STATUS_FAIL;
    if (val_compare("RANGE MAX   ", return_values[NOTIFICATION_RANGE_MAX_OFFSET], tmp_max))
        return VAL_STATUS_FAIL;
    if (val_compare("RANGE MIN   ", return_values[NOTIFICATION_RANGE_MIN_OFFSET], range_min))
        return VAL_STATUS_FAIL;

    /* Disable limit change notification for the domain */
     val_print(VAL_PRINT_TEST, "\n     [Check 6] Disable limit change notify for domain_id: %d",
                                 domain_id);

     VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
     parameters[param_count++] = domain_id;
     parameters[param_count++] = NOTIFY_DISABLE;
     cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_NOTIFY_LIMITS, COMMAND_MSG);
     val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                      &return_value_count, return_values);

     if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
         return VAL_STATUS_FAIL;

     if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
         return VAL_STATUS_FAIL;

    val_print(VAL_PRINT_TEST, "\n    [Check 7] Restore perf limits for domain_id: %d",
                                domain_id);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = domain_id;
    parameters[param_count++] = range_max;
    parameters[param_count++] = range_min;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_LIMITS_SET, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
           &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
       return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
       return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}

