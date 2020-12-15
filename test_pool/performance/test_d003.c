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
#include "val_performance.h"

#define TEST_NUM  (SCMI_PERFORMANCE_TEST_NUM_BASE + 3)
#define TEST_DESC "Perf msg attributes mandatory commands check "

uint32_t performance_query_mandatory_command_support(void)
{
    int32_t  status;
    uint32_t rsp_msg_hdr;
    uint32_t cmd_msg_hdr;
    size_t   param_count;
    size_t   return_value_count;
    uint32_t return_values[MAX_RETURNS_SIZE];
    uint32_t message_id, attributes;

    if (val_test_initialize(TEST_NUM, TEST_DESC) != VAL_STATUS_PASS)
        return VAL_STATUS_SKIP;

    val_print(VAL_PRINT_TEST, "\n     [Check 1] PERFORMANCE DOMAIN ATTRIBUTES support");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = PERFORMANCE_DOMAIN_ATTRIBUTES;
    param_count++;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_PROTOCOL_MESSAGE_ATTRIBUTES,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    attributes = return_values[ATTRIBUTE_OFFSET];
    if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attributes, 1, 31)) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Should be zero for PERFORMANCE DOMAIN ATTRIBUTES*/
    if (val_compare("MSG FASTCH SUPPORT", VAL_EXTRACT_BITS(attributes, 0, 0), 0))
        return VAL_STATUS_FAIL;

    val_print(VAL_PRINT_TEST, "\n     [Check 2] PERFORMANCE DESCRIBE LEVELS support");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = PERFORMANCE_DESCRIBE_LEVELS;
    param_count++;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_PROTOCOL_MESSAGE_ATTRIBUTES,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    attributes = return_values[ATTRIBUTE_OFFSET];
    if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attributes, 1, 31)) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    /* Should be zero for PERFORMANCE DESCRIBE LEVELS*/
    if (val_compare("MSG FASTCH SUPPORT", VAL_EXTRACT_BITS(attributes, 0, 0), 0))
        return VAL_STATUS_FAIL;

    val_print(VAL_PRINT_TEST, "\n     [Check 3] PERFORMANCE LIMITS SET support");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = PERFORMANCE_LIMITS_SET;
    param_count++;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_PROTOCOL_MESSAGE_ATTRIBUTES,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    attributes = return_values[ATTRIBUTE_OFFSET];
    if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attributes, 1, 31)) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print(VAL_PRINT_DEBUG, "\n       MESSAGE FASTCH SUPPORT : %d",
    		                    VAL_EXTRACT_BITS(attributes, 0, 0));
    val_performance_save_info(PERF_MESSAGE_FAST_CH_SUPPORT, message_id,
                               VAL_EXTRACT_BITS(attributes, 0, 0));

    val_print(VAL_PRINT_TEST, "\n     [Check 4] PERFORMANCE LIMITS GET support");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = PERFORMANCE_LIMITS_GET;
    param_count++;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_PROTOCOL_MESSAGE_ATTRIBUTES,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    attributes = return_values[ATTRIBUTE_OFFSET];
    if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attributes, 1, 31)) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print(VAL_PRINT_DEBUG, "\n       MESSAGE FASTCH SUPPORT : %d",
                                VAL_EXTRACT_BITS(attributes, 0, 0));
    val_performance_save_info(PERF_MESSAGE_FAST_CH_SUPPORT, message_id,
                              VAL_EXTRACT_BITS(attributes, 0, 0));

    val_print(VAL_PRINT_TEST, "\n     [Check 5] PERFORMANCE LEVEL SET support");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = PERFORMANCE_LEVEL_SET;
    param_count++;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_PROTOCOL_MESSAGE_ATTRIBUTES,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    attributes = return_values[ATTRIBUTE_OFFSET];
    if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attributes, 1, 31)) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print(VAL_PRINT_DEBUG, "\n       MESSAGE FASTCH SUPPORT : %d",
                                VAL_EXTRACT_BITS(attributes, 0, 0));
    val_performance_save_info(PERF_MESSAGE_FAST_CH_SUPPORT, message_id,
                              VAL_EXTRACT_BITS(attributes, 0, 0));

    val_print(VAL_PRINT_TEST, "\n     [Check 6] PERFORMANCE LEVEL GET support");

    VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, return_value_count, status);
    message_id = PERFORMANCE_LEVEL_GET;
    param_count++;
    cmd_msg_hdr = val_msg_hdr_create(PROTOCOL_PERFORMANCE, PERFORMANCE_PROTOCOL_MESSAGE_ATTRIBUTES,
                                     COMMAND_MSG);
    val_send_message(cmd_msg_hdr, param_count, &message_id, &rsp_msg_hdr, &status,
                     &return_value_count, return_values);

    if (val_compare_status(status, SCMI_SUCCESS) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    if (val_compare_msg_hdr(cmd_msg_hdr, rsp_msg_hdr) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print_return_values(return_value_count, return_values);

    attributes = return_values[ATTRIBUTE_OFFSET];
    if (val_reserved_bits_check_is_zero(VAL_EXTRACT_BITS(attributes, 1, 31)) != VAL_STATUS_PASS)
        return VAL_STATUS_FAIL;

    val_print(VAL_PRINT_DEBUG, "\n       MESSAGE FASTCH SUPPORT : %d     ",
                                VAL_EXTRACT_BITS(attributes, 0, 0));
    val_performance_save_info(PERF_MESSAGE_FAST_CH_SUPPORT, message_id,
                              VAL_EXTRACT_BITS(attributes, 0, 0));

    return VAL_STATUS_PASS;
}
