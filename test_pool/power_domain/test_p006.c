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

#define TEST_NUM  (SCMI_POWER_DOMAIN_TEST_NUM_BASE + 6)
#define TEST_DESC "Power state get check                                  "

uint32_t power_domain_get_power_state_check(void)
{
    int32_t status;
    uint32_t rsp_msg_hdr, cmd_msg_hdr;
    uint32_t param_count, domain_id, num_domains;
    uint32_t return_value_count, return_value;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Get power state for valid power domain */

    num_domains = val_power_domain_get_info(0, NUM_POWER_DOMAIN);
    val_print(VAL_PRINT_DEBUG, "\n\t[Check 1] Get power state for valid domain");

    for (domain_id = 0; domain_id < num_domains; domain_id++)
    {
        val_print(VAL_PRINT_DEBUG, "\n\tPOWER_DOMAIN ID %d", domain_id);

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        param_count++;
        return_value = 0;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_POWER_DOMAIN, POWER_STATE_GET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, &domain_id, &rsp_msg_hdr, &status,
                         &return_value_count, &return_value);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(rsp_msg_hdr, cmd_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print(VAL_PRINT_DEBUG, "\n\tPOWER STATE [%x]", return_value);
    }

    /* Get power state for invalid power domain should fail */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 2] Get power state for invalid domain");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    domain_id = val_power_domain_get_info(0, NUM_POWER_DOMAIN) + 1;
    param_count++;
    return_value = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_POWER_DOMAIN, POWER_STATE_GET, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &domain_id, &rsp_msg_hdr, &status,
                     &return_value_count, &return_value);

    if (val_compare_msg_hdr(rsp_msg_hdr, cmd_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
