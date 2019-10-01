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

#define TEST_NUM  (SCMI_POWER_DOMAIN_TEST_NUM_BASE + 5)
#define TEST_DESC "Power state set check                                  "
#define PARAMETER_SIZE 3

uint32_t power_domain_set_power_state_check(void)
{
    int32_t status;
    uint32_t rsp_msg_hdr, cmd_msg_hdr;
    uint32_t param_count, parameters[PARAMETER_SIZE];
    uint32_t return_value_count, return_value;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Power state set cmd with invalid flags value should return status INVALID PARAMETERS */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 1] Set power set with invalid flag value");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = 0xF;
    parameters[param_count++] = val_power_domain_get_info(0, NUM_POWER_DOMAIN) - 1;
    parameters[param_count++] = 0; /* ON state */
    return_value = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_POWER_DOMAIN, POWER_STATE_SET, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, &return_value);

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_status(status, SCMI_INVALID_PARAMETERS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Setting invalid power state should fail */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 2] Set invalid power state");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = 0;
    parameters[param_count++] = val_power_domain_get_info(0, NUM_POWER_DOMAIN);
    parameters[param_count++] = 1 < 31; /* Invalid state */
    return_value = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_POWER_DOMAIN, POWER_STATE_SET, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, &return_value);

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_status(status, SCMI_INVALID_PARAMETERS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

   /* Setting power state for invalid power domain should return status INVALID PARAMETERS */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 3] Set power state for invalid domain");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters[param_count++] = 0;
    parameters[param_count++] = val_power_domain_get_info(0, NUM_POWER_DOMAIN) + 1; /* invalid pd */
    parameters[param_count++] = 0; /* ON state */
    return_value = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_POWER_DOMAIN, POWER_STATE_SET, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, &return_value);

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
