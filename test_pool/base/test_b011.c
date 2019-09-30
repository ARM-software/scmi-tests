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

#define TEST_NUM  (SCMI_BASE_TEST_NUM_BASE + 11)
#define TEST_DESC "Base set protocol permissions check                    "
#define PARAMETER_SIZE 4

uint32_t base_set_protocol_permissions_check(void)
{
    int32_t status;
    uint32_t rsp_msg_hdr, cmd_msg_hdr;
    uint32_t param_count, message_id, parameters[PARAMETER_SIZE];
    uint32_t return_value_count, return_value;
    uint32_t device_id, agent_id, protocol_id;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* If SET PROTOCOL PERMISSIONS not supported, skip the test */

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = BASE_SET_PROTOCOL_PERMISSIONS;
    return_value = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_PROTOCOL_MESSAGE_ATTRIBUTES, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, &return_value);

    if (status == SCMI_NOT_FOUND) {
        val_print(VAL_PRINT_TEST, "\n\tSET_PROTOCOL_PERMISSIONS not supported                ");
        return VAL_STATUS_SKIP;
    }

    /* Setting protocol permissions for invalid agent should fail */

    agent_id = val_base_get_info(BASE_TEST_AGENT_ID);
    device_id = val_agent_get_accessible_device(agent_id);
    protocol_id = val_device_get_accessible_protocol(device_id);

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 1] Send invalid agent id");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = val_base_get_info(BASE_NUM_AGENTS) + 1; /* invalid agent id */
    parameters[param_count++] = device_id;
    parameters[param_count++] = protocol_id;
    parameters[param_count++] = 1; /* Allow access */
    return_value = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_SET_PROTOCOL_PERMISSIONS, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, &return_value);

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Disabling access to BASE PROTOCOL should fail */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 2] Try to remove access for base protocol id");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = agent_id;
    parameters[param_count++] = device_id;
    parameters[param_count++] = PROTOCOL_BASE;
    parameters[param_count++] = 0; /* Deny access */
    return_value = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_SET_PROTOCOL_PERMISSIONS, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, &return_value);

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Calling PROTOCOL PERMISSIONS with invalid flag should fail */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 3] Send invalid flags");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = agent_id;
    parameters[param_count++] = device_id;
    parameters[param_count++] = protocol_id;
    parameters[param_count++] = 4; /* Invalid flag */
    return_value = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_SET_PROTOCOL_PERMISSIONS, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, &return_value);

    if (val_compare_status(status, SCMI_INVALID_PARAMETERS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Setting protocol permissions fr device which agent don't have access should fail */

    device_id = val_agent_get_inaccessible_device(agent_id);
    val_print(VAL_PRINT_DEBUG, "\n\t[Check 4] Try to remove access for invalid device id");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = agent_id;
    parameters[param_count++] = device_id;
    parameters[param_count++] = protocol_id;
    parameters[param_count++] = 0; /* Deny access */
    return_value = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_SET_PROTOCOL_PERMISSIONS, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, &return_value);

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
