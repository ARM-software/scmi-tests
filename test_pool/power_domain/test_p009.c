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

#define TEST_NUM  (SCMI_POWER_DOMAIN_TEST_NUM_BASE + 9)
#define TEST_DESC "Power state change requested notify check              "
#define PARAMETER_SIZE 2

uint32_t power_domain_power_state_change_requested_notify_check(void)
{
    int32_t status;
    uint32_t rsp_msg_hdr, cmd_msg_hdr;
    uint32_t param_count, message_id, parameter[PARAMETER_SIZE];
    uint32_t return_value_count, return_value;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* If POWER STATE CHANGE REQUESTED NOTIFY not supported, skip the test */

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = POWER_STATE_CHANGE_REQUESTED_NOTIFY;
    param_count++;
    return_value = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_POWER_DOMAIN, PD_PROTOCOL_MESSAGE_ATTRIBUTES,
                                  COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, &return_value);

    if (status == SCMI_NOT_FOUND) {
        val_print(VAL_PRINT_TEST, "\n\tPOWER_STATE_CHANGE_REQUESTED_NOTIFY not supported     ");
        return VAL_STATUS_SKIP;
    }

    /* Enabling power state change notify for invalid power domain should fail */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 1] Enable notification for invalid power domain");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameter[param_count++] = val_power_domain_get_info(0, NUM_POWER_DOMAIN) + 1;
    parameter[param_count++] = 1; /* Enable notification */
    return_value = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_POWER_DOMAIN, POWER_STATE_CHANGE_REQUESTED_NOTIFY,
                              COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameter, &rsp_msg_hdr, &status,
                     &return_value_count, &return_value);

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Power state change notify for invalid flag value should fail */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 2] Enable notification with invalid flag value");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameter[param_count++] = val_power_domain_get_info(0, NUM_POWER_DOMAIN);
    parameter[param_count++] = 0xF; /* Invalid flag */
    return_value = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_POWER_DOMAIN, POWER_STATE_CHANGE_REQUESTED_NOTIFY,
                              COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameter, &rsp_msg_hdr, &status,
                     &return_value_count, &return_value);

    if (val_compare_status(status, SCMI_INVALID_PARAMETERS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
