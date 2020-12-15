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
#include "val_reset.h"

#define TEST_NUM  (SCMI_RESET_TEST_NUM_BASE + 11)
#define TEST_DESC "Reset notify invalid notify_enable           "

#define PARAMETER_SIZE 2

uint32_t reset_query_invalid_notify_enable(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t domain_id, message_id, num_domains;
    uint32_t parameters[PARAMETER_SIZE];

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* If RESET_PROTOCOL_NOTIFY not supported, skip the test */
    val_print(VAL_PRINT_TEST, "\n     [Check 1] Query reset notify cmd support");
    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    param_count++;
    message_id = RESET_PROTOCOL_NOTIFY;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_RESET, RESET_PROTOCOL_MESSAGE_ATTRIBUTES,
                                  COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (status == SCMI_NOT_FOUND) {
        val_print(VAL_PRINT_ERR, "\n       RESET_PROTOCOL_NOTIFY not supported         ");
        return VAL_STATUS_SKIP;
    }

    num_domains = val_reset_get_info(NUM_RESET_DOMAINS, 0x0);

    /* Skip if no domains found*/
    if (num_domains == 0) {
        val_print(VAL_PRINT_ERR, "\n       No reset domains found                      ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM DOMAINS                    : %d", num_domains);

    for (domain_id = 0; domain_id < num_domains; domain_id++)
    {
        /* Check notify for invalid notify_enable */
        val_print(VAL_PRINT_TEST, "\n     RESET DOMAIN ID: %d", domain_id);
        val_print(VAL_PRINT_TEST, "\n     [Check 2] Query reset for invalid notify_enable");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = domain_id;
        parameters[param_count++] = RESET_INVALID_NOTIFY_ENABLE;

        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_RESET, RESET_PROTOCOL_NOTIFY,
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
