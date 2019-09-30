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

#define TEST_NUM  (SCMI_BASE_TEST_NUM_BASE + 14)
#define TEST_DESC "Base deny and restore protocol access                  "
#define MAX_RETURN_VALUE_SIZE 4
#define PARAMETER_SIZE 4

uint32_t base_deny_restore_protocol_access(void)
{
    int32_t status;
    uint32_t rsp_msg_hdr, cmd_msg_hdr;
    uint32_t param_count, message_id, parameters[PARAMETER_SIZE], *parameter;
    uint32_t return_value_count, return_values[MAX_RETURN_VALUE_SIZE];
    uint32_t protocol_id, agent_id, device_id, protocol_version = 0x0;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* If SET PROTOCOL PERMISSIONS not supported, skip the test */

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = BASE_SET_PROTOCOL_PERMISSIONS;
    val_memset_zero(return_values, MAX_RETURN_VALUE_SIZE);
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_PROTOCOL_MESSAGE_ATTRIBUTES, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (status == SCMI_NOT_FOUND) {
        val_print(VAL_PRINT_TEST, "\n\tBASE_SET_PROTOCOL_PERMISSION not supported            ");
        return VAL_STATUS_SKIP;
    }

    agent_id = val_base_get_info(BASE_TEST_AGENT_ID);
    device_id = val_agent_get_accessible_device(agent_id);
    protocol_id = val_device_get_accessible_protocol(device_id);

    /* Deny agent access to a valid protocol */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 1] Deny access to valid protocol %d", protocol_id);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = agent_id;
    parameters[param_count++] = device_id;
    parameters[param_count++] = protocol_id;
    parameters[param_count++] = 0; /* Deny access */
    val_memset_zero(return_values, MAX_RETURN_VALUE_SIZE);
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_SET_PROTOCOL_PERMISSIONS, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Accessing command of denied protocol should fail */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 2] Access denied protocol commands");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameter = NULL;
    val_memset_zero(return_values, MAX_RETURN_VALUE_SIZE);
    cmd_msg_hdr = val_msg_hdr_create(protocol_id, protocol_version, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameter, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (status == SCMI_SUCCESS) {
        val_print(VAL_PRINT_ERR, "\n\tCHECK DENIED PROTOCOL ACCESS: FAILED");
        val_print(VAL_PRINT_ERR, "\n\tAble to access denied protocol");
        return VAL_STATUS_FAIL;
    } else
        val_print(VAL_PRINT_DEBUG, "\n\tCHECK DENIED PROTOCOL ACCESS: PASSED");

    /* Restore the agent access to denied protocol */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 3] Restore denied protocol access");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = agent_id;
    parameters[param_count++] = device_id;
    parameters[param_count++] = protocol_id;
    parameters[param_count++] = 1; /* Allow access */
    val_memset_zero(return_values, MAX_RETURN_VALUE_SIZE);
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_SET_PROTOCOL_PERMISSIONS, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Agent should be able to access the protocol commands */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 2] Access protocol command after permissions restored");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameter = NULL;
    val_memset_zero(return_values, MAX_RETURN_VALUE_SIZE);
    cmd_msg_hdr = val_msg_hdr_create(protocol_id, protocol_version, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameter, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
