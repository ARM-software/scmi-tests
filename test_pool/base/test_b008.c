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

#define TEST_NUM  (SCMI_BASE_TEST_NUM_BASE + 8)
#define TEST_DESC "Base query protocol list                     "

uint32_t base_query_protocol_list(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t num_protocols_returned, total_num_protocols;
    uint32_t i, protocol_list = 0;
    uint32_t skip = 0;
    uint32_t *protocols = NULL;
    uint8_t prot;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Reset protocol list, if test failed or skipped*/
    val_agent_set_supported_protocol_list(0);

    total_num_protocols = val_base_get_info(BASE_NUM_PROTOCOLS);
    if (total_num_protocols == 0) {
        val_print(VAL_PRINT_ERR, "\n       No protocols supported for agent            ");
        return VAL_STATUS_SKIP;
    }

    val_print(VAL_PRINT_TEST, "\n     [Check 1] Query with valid skip values ");

    /* Query list of all supported protocols */
    do
    {
        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        param_count++;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_DISCOVER_LIST_PROTOCOLS, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, &skip, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        num_protocols_returned = return_values[NUM_OF_PROTOCOLS_OFFSET];
        val_print(VAL_PRINT_DEBUG, "\n       NUM OF PROTOCOLS: %d", num_protocols_returned);

        protocols = &return_values[PROTOCOL_LIST_OFFSET];
        for (i = 0; i < num_protocols_returned; i++) {
            prot = (uint8_t)(protocols[i/4] >> ((i % 4) * 8));
            /* Add to protocol list */
            protocol_list |= (1 << prot);
            skip++;
        }
    } while (skip < total_num_protocols);

    /* Save supported protocol list*/
    val_print(VAL_PRINT_ERR, "\n       PROTOCOL LIST  : 0x%08x                 ", protocol_list);
    val_agent_set_supported_protocol_list(protocol_list);

    /* DISCOVER_LIST_PROTOCOLS with invalid skip value should return INVALID PARAMETERS status */
    val_print(VAL_PRINT_TEST, "\n     [Check 2] Send invalid skip value : %d", skip);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    param_count++;
    skip = (PROTOCOL_MAX - PROTOCOL_BASE) + 1; /* skip value is set > total num of protocols */
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_BASE, BASE_DISCOVER_LIST_PROTOCOLS, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &skip, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_INVALID_PARAMETERS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
