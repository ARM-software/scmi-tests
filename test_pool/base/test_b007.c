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

#define TEST_NUM  (SCMI_BASE_TEST_NUM_BASE + 7)
#define TEST_DESC "Base discover agent check                              "
#define MAX_RETURN_VALUE_SIZE 5

uint32_t base_discover_agent(void)
{
    int32_t status;
    uint32_t rsp_msg_hdr, cmd_msg_hdr;
    uint32_t param_count, message_id, agent_id, num_agents;
    uint32_t return_value_count, attributes, return_value[MAX_RETURN_VALUE_SIZE];
    uint8_t *name;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* If BASE DISCOVER AGENT not supported, skip the test */

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = BASE_DISCOVER_AGENT;
    param_count++;
    attributes = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_PROTOCOL_MESSAGE_ATTRIBUTES, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, &attributes);

    if (status == SCMI_NOT_FOUND) {
        val_print(VAL_PRINT_TEST, "\n\tBASE_DISCOVER_AGENT not supported                     ");
        return VAL_STATUS_SKIP;
    }

    /* Agent name for platform agent id should start with "platform" */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 1] Query with agent id %d", 0);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    agent_id = 0; /* agent id 0 is for platform */
    param_count++;
    val_memset_zero(return_value, MAX_RETURN_VALUE_SIZE);

    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_DISCOVER_AGENT, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &agent_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_value);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (agent_id != return_value[0]) {
        val_print(VAL_PRINT_ERR, "\n\tCHECK AGENT ID: FAILED");
        val_print(VAL_PRINT_ERR, "\n\tAgent id expected: %d", agent_id);
        val_print(VAL_PRINT_ERR, "\n\tAgent id received: %d", return_value[0]);
        return VAL_STATUS_FAIL;
    } else {
        val_print(VAL_PRINT_DEBUG, "\n\tCHECK AGENT ID: PASSED [%d]", return_value[0]);
    }

    name = (uint8_t *) &return_value[1];
    if (val_strcmp((uint8_t *)"platform", name, 8) != VAL_STATUS_PASS) {
        val_print(VAL_PRINT_ERR, "\n\tCHECK AGENT NAME: FAILED");
        val_print(VAL_PRINT_ERR, "\n\tAgent name expected: %s", "platform*");
        val_print(VAL_PRINT_ERR, "\n\tAgent name received: %s", name);
        return VAL_STATUS_FAIL;
    } else {
        val_print(VAL_PRINT_DEBUG, "\n\tCHECK AGENT NAME: PASSED [%s]", name);
    }

    /* Agent id 0xFFFFFFFF should return agent own name */

    agent_id = 0xFFFFFFFF; /* agent id 0xFFFFFFFF is for self */
    val_print(VAL_PRINT_DEBUG, "\n\t[Check 2] Query with agent id %x", agent_id);
    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    param_count++;
    val_memset_zero(return_value, MAX_RETURN_VALUE_SIZE);
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_DISCOVER_AGENT, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &agent_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_value);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_base_save_info(BASE_TEST_AGENT_ID, return_value[0]);
    val_base_save_name(BASE_TEST_AGENT_NAME, (uint8_t *)&return_value[1]);
    val_print(VAL_PRINT_INFO, "\n\tTEST_AGENT_ID: %d", return_value[0]);
    val_print(VAL_PRINT_INFO, "\n\tTEST_AGENT_NAME: %s", (uint8_t *) &return_value[1]);

    /* base discover agent cmd for invalid agent id should return NOT_FOUND status */

    agent_id = 0;
    num_agents = val_base_get_info(BASE_NUM_AGENTS);
    while (++agent_id <= num_agents)
    {
        val_print(VAL_PRINT_DEBUG, "\n\t[Check 2] Query with agent id %x", agent_id);
        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        val_memset_zero(return_value, MAX_RETURN_VALUE_SIZE);
        param_count++;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_DISCOVER_AGENT, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, &agent_id, &rsp_msg_hdr, &status,
                         &return_value_count, return_value);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (agent_id != return_value[0]) {
            val_print(VAL_PRINT_ERR, "\n\tCHECK AGENT ID: FAILED");
            val_print(VAL_PRINT_ERR, "\n\tAgent id expected: %x", agent_id);
            val_print(VAL_PRINT_ERR, "\n\tAgent id received: %x", return_value[0]);
            return VAL_STATUS_FAIL;
        } else {
            val_print(VAL_PRINT_DEBUG, "\n\tCHECK AGENT ID: PASSED [%d]", return_value[0]);
        }
        val_print(VAL_PRINT_INFO, "\n\tAGENT_NAME: %s", (uint8_t *) &return_value[1]);
    }

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 3] Query invalid agent id %d", agent_id);
    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    param_count++;
    val_memset_zero(return_value, MAX_RETURN_VALUE_SIZE);
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_DISCOVER_AGENT, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &agent_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_value);

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
