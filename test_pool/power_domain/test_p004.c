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

#define TEST_NUM  (SCMI_POWER_DOMAIN_TEST_NUM_BASE + 4)
#define TEST_DESC "Power domain attributes check                          "
#define MAX_RETURN_VALUE_SIZE 5

uint32_t power_domain_query_domain_attributes(void)
{
    int32_t status;
    uint32_t rsp_msg_hdr, cmd_msg_hdr;
    uint32_t param_count, domain_id, num_domains;
    uint32_t return_value_count, return_values[MAX_RETURN_VALUE_SIZE], attribute;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Check power domain attributes for valid domain */

    num_domains = val_power_domain_get_info(0, NUM_POWER_DOMAIN);
    val_print(VAL_PRINT_DEBUG, "\n\t[Check 1] Query with valid power domains");

    for (domain_id = 0; domain_id < num_domains; domain_id++)
    {
        val_print(VAL_PRINT_DEBUG, "\n\tPOWER_DOMAIN ID %d", domain_id);

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        param_count++;
        val_memset_zero(return_values, MAX_RETURN_VALUE_SIZE);
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_POWER_DOMAIN, POWER_DOMAIN_ATTRIBUTES,
                                         COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, &domain_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        attribute = return_values[0];
        if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attribute, 0, 28)) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print(VAL_PRINT_DEBUG, "\n\tPOWER STATE CHANGE NOTIFICATIONS SUPPORT: %d",
                              VAL_EXTRACT_BITS(attribute, 31, 31));
        val_print(VAL_PRINT_DEBUG, "\n\tPOWER ASYNCHRONOUS SUPPORT: %d",
                                VAL_EXTRACT_BITS(attribute, 30, 30));
        val_print(VAL_PRINT_DEBUG, "\n\tPOWER SYNCHRONOUS SUPPORT: %d",
                                VAL_EXTRACT_BITS(attribute, 29, 29));
        val_print(VAL_PRINT_DEBUG, "\n\tDOMAIN NAME: %s", (uint8_t *)&return_values[1]);
    }

    /* Power domain attributes for invalid power domain should fail */

    domain_id = val_power_domain_get_info(0, NUM_POWER_DOMAIN) + 1;
    val_print(VAL_PRINT_DEBUG, "\n\t[Check 2] Query with invalid domain id %d", domain_id);

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    param_count++;
    val_memset_zero(return_values, MAX_RETURN_VALUE_SIZE);
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_POWER_DOMAIN, POWER_DOMAIN_ATTRIBUTES, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &domain_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_status(status, SCMI_NOT_FOUND) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    return VAL_STATUS_PASS;
}
