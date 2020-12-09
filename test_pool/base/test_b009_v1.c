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
#include"val_base.h"

#define TEST_NUM  (SCMI_BASE_TEST_NUM_BASE + 9)
#define TEST_DESC "Base discover agent check                    "

uint32_t base_discover_agent_v1(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t message_id, agent_id, num_agents;
    uint8_t *name;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* If BASE DISCOVER AGENT not supported, skip the test */
    val_print(VAL_PRINT_TEST, "\n     [Check 1] Check discover agent cmd support");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = BASE_DISCOVER_AGENT;
    param_count++;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_PROTOCOL_MESSAGE_ATTRIBUTES, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (status == SCMI_NOT_FOUND) {
        val_print(VAL_PRINT_ERR, "\n       BASE_DISCOVER_AGENT not supported           ");
        return VAL_STATUS_SKIP;
    }

    /* Agent name for platform agent id should start with "platform" */
    val_print(VAL_PRINT_TEST, "\n     [Check 2] Query with platform agent id %d", 0);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    agent_id = 0; /* agent id 0 is for platform */
    param_count++;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_DISCOVER_AGENT, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &agent_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    name = (uint8_t *) &return_values[AGENT_NAME_v1_OFFSET];
    if (val_strcmp((uint8_t *)"platform", name, 8) != VAL_STATUS_PASS) {
        val_print(VAL_PRINT_ERR, "\n       CHECK NAME      : FAILED");
        val_print(VAL_PRINT_ERR, "\n         EXPECTED      : %s", "platform*");
        val_print(VAL_PRINT_ERR, "\n         RECEIVED      : %s", name);
        return VAL_STATUS_FAIL;
    }
    val_print(VAL_PRINT_TEST, "\n       CHECK NAME       : PASSED [%s]", name);

    /* base discover agent cmd for invalid agent id should return NOT_FOUND status */
    agent_id = 0;
    num_agents = val_base_get_info(BASE_NUM_AGENTS);
    while (++agent_id <= num_agents)
    {
        val_print(VAL_PRINT_TEST, "\n     [Check 3] Query with agent id %x", agent_id);
        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        param_count++;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_DISCOVER_AGENT, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, &agent_id, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        name = (uint8_t *) &return_values[AGENT_NAME_v1_OFFSET];
        val_print(VAL_PRINT_INFO, "\n       AGENT_NAME       : %s", name);
    }

    val_print(VAL_PRINT_TEST, "\n     [Check 4] Query invalid agent id %d", agent_id);
    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    param_count++;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_DISCOVER_AGENT, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &agent_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
