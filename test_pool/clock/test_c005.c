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
#include "val_clock.h"

#define TEST_NUM  (SCMI_CLOCK_TEST_NUM_BASE + 5)
#define TEST_DESC "Clock attributes check                       "

uint32_t clock_attributes_check(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t clock_id, num_clocks, attributes;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    /* Discover clock attributes for all clocks */
    num_clocks = val_clock_get_info(NUM_CLOCKS, 0);
    if (num_clocks == 0) {
        val_print(VAL_PRINT_ERR, "\n       No Clock found                              ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM CLOCKS     : %d", num_clocks);

    for (clock_id = 0; clock_id < num_clocks; clock_id++)
    {
        val_print(VAL_PRINT_TEST, "\n     CLOCK ID: %d", clock_id);
        val_print(VAL_PRINT_TEST, "\n     [Check 1] Query clock attribute");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        param_count++;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_ATTRIBUTES,
                                         COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, &clock_id, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        attributes = return_values[ATTRIBUTE_OFFSET];
        if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attributes, 1, 31) !=
                                            VAL_STATUS_PASS))
            return VAL_STATUS_FAIL;

        val_clock_save_info(CLOCK_ATTRIBUTE, clock_id, attributes);
        val_print(VAL_PRINT_INFO, "\n    CLOCK_ENABLED: %d", VAL_EXTRACT_BITS(attributes, 0, 0));
        val_print(VAL_PRINT_INFO, "\n    CLOCK_NAME: %s                                           ",
                                   (uint8_t *) &return_values[NAME_OFFSET]);
    }

    return VAL_STATUS_PASS;
}
