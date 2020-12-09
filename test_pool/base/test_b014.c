/** @file
 * Copyright (c) 2019-2020, Arm Limited or its affiliates. All rights reserved.
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
#include "val_base.h"

#define TEST_NUM  (SCMI_BASE_TEST_NUM_BASE + 14)
#define TEST_DESC "Base deny and restore protocol access        "
#define PARAMETER_SIZE 4

uint32_t base_deny_restore_protocol_access(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t parameters[PARAMETER_SIZE];
    uint32_t message_id, agent_id, device_id, protocol_id;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    agent_id = val_base_get_info(BASE_TEST_AGENT_ID);

    /* If agent is not trusted , skip the test */
    if (val_check_trusted_agent(agent_id) == 0) {
        val_print(VAL_PRINT_ERR, "\n       Calling agent is untrusted agent            ");
        return VAL_STATUS_SKIP;
    }

    /* If SET PROTOCOL PERMISSIONS not supported, skip the test */
    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = BASE_SET_PROTOCOL_PERMISSIONS;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_PROTOCOL_MESSAGE_ATTRIBUTES, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (status == SCMI_NOT_FOUND) {
        val_print(VAL_PRINT_ERR, "\n       BASE_SET_PROTOCOL_PERMISSION not supported  ");
        return VAL_STATUS_SKIP;
    }

    device_id = val_agent_get_accessible_device(agent_id);
    protocol_id = val_device_get_accessible_protocol(device_id);

    /* Deny agent access to a valid protocol */
    val_print(VAL_PRINT_TEST, "\n     [Check 1] Deny access to valid protocol %d", protocol_id);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = agent_id;
    parameters[param_count++] = device_id;
    parameters[param_count++] = protocol_id;
    parameters[param_count++] = FLAG_ACCESS_DENY; /* Deny access */
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_SET_PROTOCOL_PERMISSIONS, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    /* Accessing command of denied protocol should fail */
    val_print(VAL_PRINT_TEST, "\n     [Check 2] Access denied protocol commands");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    cmd_msg_hdr = val_msg_hdr_create(protocol_id, 0x0, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, NULL, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (status == SCMI_SUCCESS) {
        val_print(VAL_PRINT_ERR, "\n    CHECK DENIED PROTOCOL ACCESS: FAILED");
        val_print(VAL_PRINT_ERR, "\n    Able to access denied protocol");
        return VAL_STATUS_FAIL;
    }

    if (val_compare_status(status, SCMI_DENIED) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    /* Restore the agent access to denied protocol */
    val_print(VAL_PRINT_TEST, "\n     [Check 3] Restore denied protocol access");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = agent_id;
    parameters[param_count++] = device_id;
    parameters[param_count++] = protocol_id;
    parameters[param_count++] = FLAG_ACCESS_ALLOW; /* Allow access */
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_SET_PROTOCOL_PERMISSIONS, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Agent should be able to access the protocol commands */
    val_print(VAL_PRINT_TEST, "\n     [Check 2] Access protocol after permissions restored");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    cmd_msg_hdr = val_msg_hdr_create(protocol_id, 0x0, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, NULL, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    return VAL_STATUS_PASS;
}
