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

#define TEST_NUM  (SCMI_BASE_TEST_NUM_BASE + 15)
#define TEST_DESC "Base reset agent configuration check         "
#define PARAMETER_SIZE 2

uint32_t base_reset_agent_configuration_check(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t parameters[PARAMETER_SIZE];
    uint32_t message_id, agent_id;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    agent_id = val_base_get_info(BASE_TEST_AGENT_ID);

    /* If agent is not trusted , skip the test */
    if (val_check_trusted_agent(agent_id) == 0) {
        val_print(VAL_PRINT_ERR, "\n       Calling agent is untrusted agent            ");
        return VAL_STATUS_SKIP;
    }

    /* If BASE RESET AGENT CONFIGURATION not supported, skip the test */
    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = BASE_RESET_AGENT_CONFIGURATION;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_PROTOCOL_MESSAGE_ATTRIBUTES, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (status == SCMI_NOT_FOUND) {
        val_print(VAL_PRINT_ERR, "\n       BASE_RESET_AGENT_CONFIG not supported       ");
        return VAL_STATUS_SKIP;
    }

    /* Setting permissions for invalid agent should fail */
    val_print(VAL_PRINT_TEST, "\n     [Check 1] Set permissions for invalid agent id");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = val_base_get_info(BASE_NUM_AGENTS) + 1; /* invalid agent id */
    parameters[param_count++] = FLAG_ACCESS_ALLOW; /* Allow access */
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_RESET_AGENT_CONFIGURATION, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    /* Calling RESET AGENT CONFIGURATION cmd with invalid flag value should fail */
    val_print(VAL_PRINT_TEST, "\n     [Check 2] Set permissions with invalid flag value");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = val_base_get_info(BASE_TEST_AGENT_ID);
    parameters[param_count++] = INVALID_FLAG_VAL; /* Invalid flag */
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_RESET_AGENT_CONFIGURATION, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_INVALID_PARAMETERS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    return VAL_STATUS_PASS;
}
