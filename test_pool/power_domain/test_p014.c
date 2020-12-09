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

#include"val_interface.h"
#include"val_power_domain.h"

#define TEST_NUM  (SCMI_POWER_DOMAIN_TEST_NUM_BASE + 14)
#define TEST_DESC "Power state notify invalid domain check      "

#define PARAMETER_SIZE 2

uint32_t power_domain_power_state_notify_invalid_domain_check(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t message_id, parameter[PARAMETER_SIZE];

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* If POWER STATE NOTIFY not supported, skip the test */
    val_print(VAL_PRINT_TEST, "\n     [Check 1] Query power state notify cmd support");
    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    param_count++;
    message_id = POWER_STATE_NOTIFY;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_POWER_DOMAIN, PD_PROTOCOL_MESSAGE_ATTRIBUTES,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (status == SCMI_NOT_FOUND) {
        val_print(VAL_PRINT_ERR, "\n       POWER_STATE_NOTIFY unsupported              ");
        return VAL_STATUS_SKIP;
    }

    /* Enabling power state notify for invalid power domain should fail */

    val_print(VAL_PRINT_TEST, "\n     [Check 2] Enable power state notify for invalid domain");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameter[param_count++] = val_power_domain_get_info(0, NUM_POWER_DOMAIN) + 1;
    parameter[param_count++] = NOTIFY_ENABLE; /* Enable notification */
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_POWER_DOMAIN, POWER_STATE_NOTIFY, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameter, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
