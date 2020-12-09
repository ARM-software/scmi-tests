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

#include "val_interface.h"
#include "val_power_domain.h"

#define TEST_NUM  (SCMI_POWER_DOMAIN_TEST_NUM_BASE + 5)
#define TEST_DESC "Power domain attributes check                "

uint32_t power_domain_query_domain_attributes(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t domain_id, num_domains, attribute;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    num_domains = val_power_domain_get_info(0, NUM_POWER_DOMAIN);
    if (num_domains == 0) {
        val_print(VAL_PRINT_ERR, "\n       No power domains found                      ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM DOMAINS    : %d", num_domains);

    for (domain_id = 0; domain_id < num_domains; domain_id++)
    {
        /* Check power domain attributes for valid domain */
        val_print(VAL_PRINT_TEST, "\n     POWER DOMAIN ID: %d", domain_id);
        val_print(VAL_PRINT_TEST, "\n     [Check 1] Query power domain attribute");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        param_count++;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_POWER_DOMAIN, POWER_DOMAIN_ATTRIBUTES,
                                         COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, &domain_id, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        attribute = return_values[ATTRIBUTE_OFFSET];
        if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attribute, 0, 28)) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print(VAL_PRINT_DEBUG, "\n     POWER STATE CHANGE NOTIFICATIONS SUPPORT: %d",
                                    VAL_EXTRACT_BITS(attribute, 31, 31));
        val_print(VAL_PRINT_DEBUG, "\n     POWER ASYNCHRONOUS SUPPORT: %d",
                                    VAL_EXTRACT_BITS(attribute, 30, 30));
        val_print(VAL_PRINT_DEBUG, "\n     POWER SYNCHRONOUS SUPPORT: %d",
                                    VAL_EXTRACT_BITS(attribute, 29, 29));
        val_print(VAL_PRINT_DEBUG, "\n     DOMAIN NAME: %s",
                                   (uint8_t *)&return_values[DOMAIN_NAME_OFFSET]);
    }

    return VAL_STATUS_PASS;

}
