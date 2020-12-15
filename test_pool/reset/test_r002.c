/** @file
 * Copyright (c) 2020, Arm Limited or its affiliates. All rights reserved.
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
#include "val_reset.h"

#define TEST_NUM  (SCMI_RESET_TEST_NUM_BASE + 2)
#define TEST_DESC "Reset protocol attributes check              "

#define RETURN_VALUE_COUNT 1

uint32_t reset_query_protocol_attributes(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t *parameters;
    uint32_t attributes, num_domains;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Query reset protocol and check num of domains */
    val_print(VAL_PRINT_TEST, "\n     [Check 1] Query protocol attributes");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters = NULL; /* No parameters for this command */
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_RESET, RESET_PROTOCOL_ATTRIBUTES, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    attributes = return_values[ATTRIBUTE_OFFSET];
    if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attributes, 16, 31) != VAL_STATUS_PASS))
        return VAL_STATUS_FAIL;

    if (val_compare_return_count(return_value_count, RETURN_VALUE_COUNT))
        return VAL_STATUS_FAIL;

    /* Save the number of domains */
    num_domains = VAL_EXTRACT_BITS(attributes, 0, 15);
    if (val_compare("NUM DOMAINS", num_domains, val_reset_get_expected_num_domains()))
        return VAL_STATUS_FAIL;

    val_reset_save_info(NUM_RESET_DOMAINS, 0x0, num_domains);

    return VAL_STATUS_PASS;
}
