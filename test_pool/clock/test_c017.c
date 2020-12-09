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
#include "val_clock.h"

#define TEST_NUM  (SCMI_CLOCK_TEST_NUM_BASE + 17)
#define TEST_DESC "Clock config set check                       "

#define MAX_PARAMETER_SIZE 2

/********* TEST ALGO ********************
 * Find a new attribute from previously saved value
 * Set new attribute
 * Get the attribute again and compare with newly set attribute
 * Restore the default attribute
*****************************************/

uint32_t clock_config_set_check(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE], attributes;
    uint32_t clock_id, num_clocks, parameters[MAX_PARAMETER_SIZE];
    uint32_t run_flag = 0;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    num_clocks = val_clock_get_info(NUM_CLOCKS, 0);
    if (num_clocks == 0) {
        val_print(VAL_PRINT_ERR, "\n       No Clock found                              ");
        return VAL_STATUS_SKIP;
    }
    val_print(VAL_PRINT_DEBUG, "\n       NUM CLOCKS     : %d", num_clocks);

    for (clock_id = 0; clock_id < num_clocks; clock_id++)
    {

        val_print(VAL_PRINT_TEST, "\n     CLOCK ID: %d", clock_id);

        /* Skip if always on clock*/
        if (val_check_clock_config_change_support(clock_id) == 1)
            continue;

        run_flag = 1;
        /* Find new attribute from previously stored value */
        attributes = val_clock_get_info(CLOCK_ATTRIBUTE, clock_id);
        if (attributes == CONFIG_SET_DISABLE)
            attributes = CONFIG_SET_ENABLE;
        else
            attributes = CONFIG_SET_DISABLE;

        /* CLOCK_CONFIG_SET with new attribute */
        val_print(VAL_PRINT_TEST, "\n     [Check 1] Set config with attributes :%d", attributes);

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = clock_id;
        parameters[param_count++] = attributes;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_CONFIG_SET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        /* Get attribute and compare with new value */
        val_print(VAL_PRINT_TEST, "\n     [Check 2] Verify the changed attribute");

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        param_count++;
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_ATTRIBUTES, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, &clock_id, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_reserved_bits_check_is_zero(
                VAL_EXTRACT_BITS(return_values[ATTRIBUTE_OFFSET], 1, 31) != VAL_STATUS_PASS))
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);

        if (val_compare("CLOCK STATUS ",
                         VAL_EXTRACT_BITS(return_values[ATTRIBUTE_OFFSET], 0, 0), attributes))
            return VAL_STATUS_FAIL;

        /* Restore the default attribute value*/
        val_print(VAL_PRINT_TEST, "\n     [Check 3] Restore the original attributes :%d",
                                   val_clock_get_info(CLOCK_ATTRIBUTE, clock_id));

        VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
        parameters[param_count++] = clock_id;
        parameters[param_count++] = val_clock_get_info(CLOCK_ATTRIBUTE, clock_id);
        cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_CLOCK, CLOCK_CONFIG_SET, COMMAND_MSG);
        val_send_message(cmd_msg_hdr, param_count, parameters, &rsp_msg_hdr, &status,
                         &return_value_count, return_values);

        if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
            return VAL_STATUS_FAIL;

        val_print_return_values(return_value_count, return_values);
    }
    if (run_flag == 0)
        return VAL_STATUS_SKIP;

    return VAL_STATUS_PASS;
}
