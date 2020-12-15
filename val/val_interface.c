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

#define MAX_STRCPY_SIZE 100

static uint64_t g_test_pass;
static uint64_t g_test_fail;
static uint64_t g_test_skip;

static uint32_t g_protocol_list;

/**
  @brief   This function forms the command message header
           1. Caller       -  ACK.
  @param   protocol_id protocol identifier
  @param   msg_id      command idenitifer
  @param   msg_type    message type
  @return  32 bit message header
**/
uint32_t val_msg_hdr_create(uint32_t protocol_id, uint32_t msg_id, uint32_t msg_type)
{
    uint32_t msg_hdr = 0;

    msg_hdr = msg_hdr | (protocol_id << 10);
    msg_hdr = msg_hdr | (msg_type << 8);
    msg_hdr = msg_hdr | msg_id;

    return msg_hdr;
}

/**
  @brief   This function forms the command message header
           1. Caller       -  ACK.
  @param   none
  @return  none
**/
uint32_t val_initialize_system(void *info)
{
    g_test_pass = 0;
    g_test_fail = 0;
    g_test_skip = 0;
    return pal_initialize_system(info);
}

/**
  @brief   This function initialize test
           1. Caller       -  ACK.
  @param   test_num  test number
  @param   test_desc test description
  @return  test status
**/
uint32_t val_test_initialize(uint32_t test_num, char *test_desc)
{
    val_print(VAL_PRINT_ERR, "\n%3d: %s ", test_num, test_desc);
    return VAL_STATUS_PASS;
}

/**
  @brief   This is val print function
           1. Caller       -  ACK.
  @param   print_level  filter for print statements
  @param   format       print data and format
  @return  none
**/
void val_print(uint32_t print_level, const char *format, ...)
{
    va_list args;

    if (print_level <= VERBOSE_LEVEL) {
        va_start(args, format);
        pal_print(print_level, format, args);
        va_end(args);
    }
}

/**
  @brief   This is val memset function
           1. Caller       -  ACK.
  @param   ptr     address
  @param   value   value to be filled in address
  @param   length  length of buffer in bytes
  @return  none
**/
void val_memset(void *ptr, int value, size_t length)
{
    uint8_t *dest = (uint8_t *)ptr;
    int32_t i;

    if (dest != NULL) {
        for (i = 0; i < length; i++)
            dest[i] = value;
    }
}

/**
  @brief   This is strcpy function
           1. Caller       -  ACK.
  @param   dst_ptr  destination pointer
  @param   src_ptr  source pointer
  @return  status
**/
uint32_t val_strcpy(uint8_t *dst_ptr, uint8_t *src_ptr)
{
    uint8_t index = 0;

    if ((dst_ptr != NULL) && (src_ptr != NULL)) {
        while (('\0' != src_ptr[index]) &&  (index < MAX_STRCPY_SIZE)) {
            dst_ptr[index] = src_ptr[index];
            ++index;
        }
        dst_ptr[index] = '\0';
        return PAL_STATUS_PASS;
    }

    return PAL_STATUS_FAIL;
}

/**
  @brief   This is strcmp function
           1. Caller       -  ACK.
  @param   dst_ptr  destination pointer
  @param   src_ptr  source pointer
  @param   len      length
  @return  status   string matching result
**/
uint32_t val_strcmp(uint8_t *dst_ptr, uint8_t *src_ptr, uint32_t len)
{
    uint8_t index = 0;

    if ((dst_ptr == NULL) || (src_ptr == NULL))
        return PAL_STATUS_FAIL;

    while (index < len) {
        if ((dst_ptr[index] == '\0') || (src_ptr[index]  == '\0'))
                break;
        if (dst_ptr[index] != src_ptr[index])
                return PAL_STATUS_FAIL;
        ++index;
    }
    return PAL_STATUS_PASS;
}

/**
  @brief   This function checks if msg header received matches cmd msg header
           1. Caller       - Test Suite.
  @param   rcvd_msg_hdr  message header received from platform
  @param   sent_msg_hsr  message header sent to platform
  @return  status
**/
uint32_t val_compare_msg_hdr(uint32_t sent_msg_hdr, uint32_t rcvd_msg_hdr)
{
    if (rcvd_msg_hdr != sent_msg_hdr) {
        val_print(VAL_PRINT_ERR, "\n       CHECK HEADER   : FAILED");
        val_print(VAL_PRINT_ERR, "\n         SENT       : 0x%08x                ", sent_msg_hdr);
        val_print(VAL_PRINT_ERR, "\n         RECEIVED   : 0x%08x                ", rcvd_msg_hdr);
        return VAL_STATUS_FAIL;
    } else
        val_print(VAL_PRINT_TEST, "\n       CHECK HEADER   : PASSED [0x%08x]        ",
                                                                                    rcvd_msg_hdr);

    return VAL_STATUS_PASS;
}

/**
  @brief   This function compares int value recieve by SCMI protocol with expected value
           1. Caller       - Test Suite.
  @param   parameter name
  @param   rcvd_val  value recievd using SCMI protocol
  @param   exp_val   expected value using pal expected file
  @return  status
**/
uint32_t val_compare(char *parameter, uint32_t rcvd_val, uint32_t exp_val)
{
    if (rcvd_val != exp_val) {
        val_print(VAL_PRINT_ERR, "\n       CHECK %s: FAILED", parameter);
        val_print(VAL_PRINT_ERR, "\n         EXPECTED   : 0x%08x                ", exp_val);
        val_print(VAL_PRINT_ERR, "\n         RECEIVED   : 0x%08x                ", rcvd_val);
        return VAL_STATUS_FAIL;
    } else
        val_print(VAL_PRINT_TEST, "\n       CHECK %s: PASSED [0x%08x]     ", parameter, rcvd_val);

    return VAL_STATUS_PASS;
}

/**
  @brief   This function compares string value recieve by SCMI protocol with expected value
           1. Caller       - Test Suite.
  @param   parameter name
  @param   rcvd_val  value recievd using SCMI protocol
  @param   exp_val   expected value using pal expected file
  @param   len       length of string
  @return  status
**/
uint32_t val_compare_str(char *parameter, char *rcvd_val, char *exp_val, uint32_t len)
{
    if (val_strcmp((uint8_t *)rcvd_val, (uint8_t *)exp_val, len)) {
        val_print(VAL_PRINT_ERR, "\n       CHECK %s: FAILED", parameter);
        val_print(VAL_PRINT_ERR, "\n         EXPECTED   : %s                ", exp_val);
        val_print(VAL_PRINT_ERR, "\n         RECEIVED   : %s                ", rcvd_val);
        return VAL_STATUS_FAIL;
    } else
        val_print(VAL_PRINT_TEST, "\n       CHECK %s: PASSED [%s]          ", parameter, rcvd_val);

    return VAL_STATUS_PASS;
}

/**
  @brief   This function is used to print ret values for debugging
           1. Caller       - Test Suite.
  @param   count   number of ret values
  @param   values  return values
  @return  none
**/
void val_print_return_values(uint32_t count, uint32_t *values)
{
    uint32_t i;

    val_print(VAL_PRINT_DEBUG, "\n       RETURN COUNT   : %d                          ", count);
    for (i = 0; i < count; i++) {
        val_print(VAL_PRINT_DEBUG, "\n       RETURN[%02d]     : 0x%08x", i, values[i]);
    }

}

/**
  @brief   This function checks if reserved_bits received are zero
           1. Caller       - Test Suite.
  @param   reserved_bits  reserved bits value received from platform
  @return  status
**/
uint32_t val_reserved_bits_check_is_zero(uint32_t reserved_bits)
{
    if (reserved_bits != VAL_RESERVED_BYTE_ZERO) {
        val_print(VAL_PRINT_ERR, "\n       CHECK RSVD BITS: FAILED [0x%08x]", reserved_bits);
        return VAL_STATUS_FAIL;
    } else
        val_print(VAL_PRINT_TEST, "\n       CHECK RSVD BITS: PASSED                     ");

    return VAL_STATUS_PASS;
}

/**
  @brief   This API is used to convert test result to string
  @param   test_status  test status value
  @return  string       test status string
**/
uint32_t val_report_status(uint32_t status)
{
    switch (status)
    {
        case VAL_STATUS_PASS:
              g_test_pass++;
              val_print(VAL_PRINT_ERR, "          : CONFORMANT");
              break;
        case VAL_STATUS_FAIL:
              g_test_fail++;
              val_print(VAL_PRINT_ERR, "          : NON CONFORMANT");
              break;
        case VAL_STATUS_SKIP:
              g_test_skip++;
              val_print(VAL_PRINT_ERR, "          : SKIPPED");
              break;
    }
    return status;
}

/**
  @brief   This API is used to get test status string
           1. Caller       -  Test Suite.
           2. Prerequisite -  Base protocol info table.
  @param   test_status  test status value
  @return  string       test status string
**/
char *val_get_result_string(uint32_t test_status)
{
    switch (test_status)
    {
    case VAL_STATUS_PASS:
        return "PASSED";
    case VAL_STATUS_FAIL:
        return "FAILED";
    case VAL_STATUS_SKIP:
        return "SKIPPED";
    default:
        val_print(VAL_PRINT_ERR, "\nInvalid test status %d", test_status);
        return "UNKNOWN";
    }
}

/**
  @brief   This API is used to get test status string
           1. Caller       -  Test Suite.
           2. Prerequisite -  Base protocol info table.
  @param   test_status  status value
  @return  string       status string
**/
char *val_get_status_string(int32_t status)
{
    switch (status)
    {
    case SCMI_SUCCESS:
        return "SCMI_STATUS_SUCCES";
    case SCMI_NOT_SUPPORTED:
        return "SCMI_NOT_SUPPORTED";
    case SCMI_INVALID_PARAMETERS:
        return "SCMI_INVALID_PARAM";
    case SCMI_DENIED:
        return "SCMI_DENIED_ERROR";
    case SCMI_NOT_FOUND:
        return "SCMI_NOT_FOUND_ERR";
    case SCMI_OUT_OF_RANGE:
        return "SCMI_OUT_OF_RANGE";
    case SCMI_BUSY:
        return "SCMI_BUSY_ERROR";
    case SCMI_COMMS_ERROR:
        return "SCMI_COMMS_ERROR";
    case SCMI_GENERIC_ERROR:
        return "SCMI_GENERIC_ERROR";
    case SCMI_HARDWARE_ERROR:
        return "SCMI_HARDWARE_ERR";
    case SCMI_PROTOCOL_ERROR:
        return "SCMI_PROTOCOL_ERR";
    case SCMI_RESERVED:
        return "SCMI_RESERVED_ERR";
    default:
        val_print(VAL_PRINT_ERR, "\nInvalid status %d", status);
        return "SCMI_NOT_SPECIFIED";
    }
}


/**
  @brief   This API is used to compare expected status with return status
           1. Caller       -  Test Suite.
           2. Prerequisite -  Base protocol info table.
  @param   status  status returned by platform
  @param   expected_status  expected status
  @return  comparision result
**/
uint32_t val_compare_status(int32_t status, int32_t expected_status)
{
    if (status != expected_status) {
        val_print(VAL_PRINT_ERR, "\n       CHECK STATUS   : FAILED");
        val_print(VAL_PRINT_ERR, "\n           EXPECTED   : %s   ",
                val_get_status_string(expected_status));
        val_print(VAL_PRINT_ERR, "\n           RECEIVED   : %s         ",
                val_get_status_string(status));
        return VAL_STATUS_FAIL;
    } else {
        val_print(VAL_PRINT_TEST, "\n       CHECK STATUS   : PASSED [%s] ",
                    val_get_status_string(status));
        return VAL_STATUS_PASS;
    }
}

/**
  @brief   This function checks if expected & received return values counts matches
           1. Caller       - Test Suite.
  @param   received  received return values count
  @param   expected  expected return values count
  @return  status
**/
uint32_t val_compare_return_count(uint32_t received, uint32_t expected)
{
    if (received != expected) {
        val_print(VAL_PRINT_ERR, "\n       CHECK COUNT    : FAILED");
        val_print(VAL_PRINT_ERR, "\n         EXPECTED   : %d                ", expected);
        val_print(VAL_PRINT_ERR, "\n         RECEIVED   : %d                ", received);
        return VAL_STATUS_FAIL;
    } else
        val_print(VAL_PRINT_TEST, "\n       CHECK COUNT    : PASSED [%d]       ", received);

    return VAL_STATUS_PASS;
}

/**
  @brief   This function is used to send command data to platform and receive response
           1. Caller       - Test Suite.
  @param   msg_hdr          Command message
  @param   num_parameter    Number of parameter sent to platform
  @param   parameter_buffer Buffer of sent parameters
  @param   rcvd_msg_hdr     message header received from platform
  @param   status           Status of command processed
  @param   rcvd_buffer_size Size of return buffer
  @param   rcvd_buffer      return values buffer
  @return  none
**/
void val_send_message(uint32_t msg_hdr, size_t num_parameter, uint32_t *parameter_buffer,
                      uint32_t *rcvd_msg_hdr, int32_t *status, size_t *rcvd_buffer_size,
                      uint32_t *rcvd_buffer)
{
    uint32_t i;

    val_print(VAL_PRINT_DEBUG, "\n       MSG HDR        : 0x%08x", msg_hdr);
    val_print(VAL_PRINT_DEBUG, "\n       NUM PARAM      : %d", num_parameter);

    for (i = 0; i < num_parameter; i++)
    {
        val_print(VAL_PRINT_DEBUG, "\n       PARAMETER[%02d]  : 0x%08x", i, parameter_buffer[i]);
    }

    pal_send_message(msg_hdr, num_parameter, parameter_buffer, rcvd_msg_hdr, status,
                     rcvd_buffer_size, rcvd_buffer);
}

/**
  @brief   This API is used to receive notifications from platform
           1. Caller       -  Test Suite.
  @param   message header received
  @param   return values count
  @param   return values list
**/
void val_receive_notification(uint32_t *message_header_rcv, size_t *return_values_count,
                              uint32_t *return_values)
{
    pal_receive_notification(message_header_rcv, return_values_count, return_values);
}

/**
  @brief   This API is used to receive delayed response
           1. Caller       -  Test Suite.
  @param   message header received
  @param   status of command processed
  @param   return values count
  @param   return values list
**/
void val_receive_delayed_response(uint32_t *message_header_rcv, int32_t *status,
        size_t *return_values_count, uint32_t *return_values)
{
    pal_receive_delayed_response(message_header_rcv, status, return_values_count, return_values);
}

/**
  @brief   This API is used return accesible device for given agent id
           1. Caller       -  Test Suite.
  @param   agent_id  agent id
  @return  device id
**/
uint32_t val_agent_get_accessible_device(uint32_t agent_id)
{
    return pal_agent_get_accessible_device(agent_id);
}

/**
  @brief   This API is used return inaccesible device for given agent id
           1. Caller       -  Test Suite.
  @param   agent_id  agent id
  @return  device id
**/
uint32_t val_agent_get_inaccessible_device(uint32_t agent_id)
{
    return pal_agent_get_inaccessible_device(agent_id);
}

/**
  @brief   This API is used return accesible protocol for given device id
           1. Caller       -  Test Suite.
  @param   device_id  device id
  @return  protocol id
**/
uint32_t val_device_get_accessible_protocol(uint32_t device_id)
{
    return pal_device_get_accessible_protocol(device_id);
}

/**
  @brief   This API is used to set supported protocols
           1. Caller       -  Test Suite.
  @param   protocol_list
**/
void val_agent_set_supported_protocol_list(uint32_t protocol_list)
{
     g_protocol_list = protocol_list;
}

/**
  @brief   This API is used to get protocols
           1. Caller       -  Test Suite.
  @param   protocol
  @return  protocol id is supported or not
**/
uint32_t val_agent_check_protocol_support(uint32_t protocol)
{
    return ((g_protocol_list >> protocol) & 1);
}

/**
  @brief   This API is used to get whether an agent is supported or not
           1. Caller       -  Test Suite.
  @param  agent_id
  @return agent is trusted or not
**/
uint32_t val_check_trusted_agent(uint32_t agent_id)
{
    return pal_check_trusted_agent(agent_id);
}

/**
  @brief   This API returns num of test passed
  @return num of test passed
**/
uint32_t val_get_test_passed(void)
{
    return g_test_pass;
}

/**
  @brief   This API returns num of test failed
  @return num of test failed
**/
uint32_t val_get_test_failed(void)
{
    return g_test_fail;
}

/**
  @brief   This API returns num of test skipped
  @return num of test skipped
**/
uint32_t val_get_test_skipped(void)
{
    return g_test_skip;
}
