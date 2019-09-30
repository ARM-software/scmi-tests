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

#define TEST_NUM  (SCMI_CLOCK_TEST_NUM_BASE + 2)
#define TEST_DESC "Clock Protocol Attributes check                        "

uint32_t clock_query_protocol_attributes(void)
{
    int32_t status;
    uint32_t rsp_msg_hdr, cmd_msg_hdr;
    uint32_t param_count, *parameters;
    uint32_t return_value_count, attributes;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Query clock protocol attributes and check number of clocks */

    val_print(VAL_PRINT_DEBUG, "\n\t[Check 1] Query protocol attributes");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    parameters = NULL; /* No parameters for this command */
    attributes = 0;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_PROTOCOL_ATTRIBUTES, COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                     &return_value_count, &attributes);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attributes, 24, 31) != VAL_STATUS_PASS))
        return VAL_STATUS_FAIL;

    /* Save the number of clocks */
    val_clock_save_info(NUM_CLOCKS, 0, VAL_EXTRACT_BITS(attributes, 0, 15));
    val_print(VAL_PRINT_INFO, "\n\tNUM CLOCKS: %d", VAL_EXTRACT_BITS(attributes, 0, 15));

    /* Save the max number of pending async clock rate changes  */
    val_clock_save_info(CLOCK_MAX_PENDING_ASYNC_CMD, 0, VAL_EXTRACT_BITS(attributes, 16, 23));
    val_print(VAL_PRINT_INFO, "\n\tMAX PENDING ASYNC CMD: %d                               ",
                            VAL_EXTRACT_BITS(attributes, 16, 23));

    return VAL_STATUS_PASS;
}
