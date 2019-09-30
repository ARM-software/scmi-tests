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

#define TEST_NUM  (SCMI_POWER_DOMAIN_TEST_NUM_BASE + 2)
#define TEST_DESC "Power domain protocol attributes check                 "
#define MAX_RETURN_VALUE_SIZE 4

uint32_t power_domain_query_protocol_attributes(void)
{
    int32_t status;
    uint32_t rsp_msg_hdr, cmd_msg_hdr;
    uint32_t param_count, *parameters;
    uint32_t return_value_count, return_values[MAX_RETURN_VALUE_SIZE];
    uint32_t attributes, stats_addr_low, stats_addr_high, stats_len;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Query protocol attributes for number of power domains */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 1] Query protocol attributes");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters = NULL; /* No parameters for this command */
    val_memset_zero(return_values, MAX_RETURN_VALUE_SIZE);
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_POWER_DOMAIN, PD_PROTOCOL_ATTRIBUTES, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    attributes = return_values[0];
    if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attributes, 16, 31)) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Save number of power domains */
    val_power_domain_save_info(NUM_POWER_DOMAIN, 0x00, VAL_EXTRACT_BITS(attributes, 0, 15));
    val_print(VAL_PRINT_INFO, "\n\tNUM POWER DOMAINS: %d", VAL_EXTRACT_BITS(attributes, 0, 15));
    /* Save the power domain shared memory low address */
    stats_addr_low = return_values[1];
    val_power_domain_save_info(PD_STATS_ADDR_LOW, 0x00, stats_addr_low);
    val_print(VAL_PRINT_INFO, "\n\tSTATISTICS ADDR LOW: %x", VAL_EXTRACT_BITS(attributes, 0, 15));
    /* Save the power domain shared memory high address */
    stats_addr_high = return_values[2];
    val_power_domain_save_info(PD_STATS_ADDR_HIGH, 0x00, stats_addr_high);
    val_print(VAL_PRINT_INFO, "\n\tSTATISTICS ADDR HIGH: %x", VAL_EXTRACT_BITS(attributes, 0, 15));
    /* Save the power domain shared memory length */
    stats_len = return_values[3];
    val_power_domain_save_info(PD_STATS_ADDR_LEN, 0x00, stats_len);
    val_print(VAL_PRINT_INFO, "\n\tSTATISTICS ADDR LEN: %d                                 ",
              VAL_EXTRACT_BITS(attributes, 0, 15));

    return VAL_STATUS_PASS;
}
