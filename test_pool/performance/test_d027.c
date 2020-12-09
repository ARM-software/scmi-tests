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

#define TEST_NUM  (SCMI_PERFORMANCE_TEST_NUM_BASE + 27)
#define TEST_DESC "Performance level changed notification check "

/********* TEST ALGO ********************
 * Get the domain which has level change noti and set level support
 * Enable level notification
 * Get allowed performance levels for the domain
 * Set new performance level
 * Check if notification of new level is received
 * Disable notification
 * Restore the default level
*****************************************/

uint32_t performance_level_set_async(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t domain_id, num_domains, attribute, cur_level, default_level;
    uint32_t parameters[2], set_level_support = 0, level_notify_support = 0;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Check for set level and notify level change support for each domain */
    num_domains = val_performance_get_info(NUM_PERF_DOMAINS, 0x00);
    if (num_domains == 0) {
        val_print(VAL_PRINT_ERR, "\n       No performance domains found                ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM DOMAINS    : %d", num_domains);

    for (domain_id = 0; domain_id < num_domains; domain_id++)
    {
        set_level_support = 0;
        level_notify_support = 0;

        val_print(VAL_PRINT_TEST, "\n     PERFORMANCE DOMAIN ID: %d", domain_id);
        val_print(VAL_PRINT_TEST,
                  "\n     [Check 1] Find set level & level notify supported domain");

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

        set_level_support = VAL_EXTRACT_BITS(attribute, 30, 30);
        val_print(VAL_PRINT_DEBUG, "\n       SET LEVEL SUPPORT: %d", set_level_support);
        level_notify_support = VAL_EXTRACT_BITS(attribute, 28, 28);
        val_print(VAL_PRINT_DEBUG, "\n       LEVEL CHG NOTIFY SUPPORT: %d", level_notify_support);
        if (set_level_support && level_notify_support)
            break;
    }

    if ((set_level_support && level_notify_support) == 0) {
        val_print(VAL_PRINT_ERR, "\n       No domain has both set/notify level support ");
        return VAL_STATUS_SKIP;
    }

    /* Enable level change notification for the domain */
    val_print(VAL_PRINT_TEST, "\n     [Check 2] Enable level change notify for domain_id: %d",
                                domain_id);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = domain_id;
    parameters[param_count++] = NOTIFY_ENABLE; /* Enable notification */
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_NOTIFY_LEVEL, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Get the allowed performance levels for the domain */
    val_print(VAL_PRINT_TEST, "\n     [Check 3] Get current level of domain_id: %d",
                               domain_id);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = domain_id;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_LEVEL_GET,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    default_level = return_values[PERFORMANCE_LEVEL_OFFSET];

    /* Find the new performance level */
    if (default_level != val_performance_get_info(PERF_DOMAIN_MAX_LEVEL, domain_id))
        cur_level = val_performance_get_info(PERF_DOMAIN_MAX_LEVEL, domain_id);
    else
        cur_level = val_performance_get_info(PERF_DOMAIN_MIN_LEVEL, domain_id);

    /* Change the performance level */
    val_print(VAL_PRINT_TEST, "\n     [Check 4] Change the performance level to: %08x", cur_level);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = domain_id;
    parameters[param_count++] = cur_level;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_LEVEL_SET, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Wait for notification */
    val_print(VAL_PRINT_TEST, "\n     [Check 5] Wait for level change notify");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    val_receive_notification(&rsp_msg_hdr, &return_value_count, return_values);

    val_print(VAL_PRINT_TEST, "\n       NOTIFY MSG HDR   : %08x", rsp_msg_hdr);
    if (val_compare("PROTOCOL ID", VAL_EXTRACT_BITS(rsp_msg_hdr, 10, 17), PROTOCOL_PERFORMANCE))
        return VAL_STATUS_FAIL;
    if (val_compare("MSG TYPE   ", VAL_EXTRACT_BITS(rsp_msg_hdr, 8, 9), NOTIFICATION_MSG))
        return VAL_STATUS_FAIL;
    if (val_compare("MSG ID     ", VAL_EXTRACT_BITS(rsp_msg_hdr, 0, 7), PERFORMANCE_LEVEL_CHANGED))
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);
    if (val_compare("DOMAIN ID  ", return_values[NOTIFICATION_DOMAIN_ID_OFFSET], domain_id))
        return VAL_STATUS_FAIL;
    if (val_compare("PERF LEVEL ", return_values[NOTIFICATION_LEVEL_OFFSET], cur_level))
        return VAL_STATUS_FAIL;

    /* Disable level change notification for the domain */

    val_print(VAL_PRINT_TEST, "\n     [Check 6] Disable level change notify for domain_id: %d",
                               domain_id);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = domain_id;
    parameters[param_count++] = NOTIFY_DISABLE; /* Disable notification */
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_NOTIFY_LEVEL, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Restore performance level for the domain */
    val_print(VAL_PRINT_TEST, "\n     [Check 7] Restore perf level for domain_id: %d",
                                        domain_id);
    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = domain_id;
    parameters[param_count++] = default_level;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_LEVEL_SET,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
            &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}

