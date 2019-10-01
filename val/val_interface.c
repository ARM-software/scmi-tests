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

#include "val_interface.h"

#define MAX_STRCPY_SIZE 100

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
void val_initialize_system(void)
{
    pal_initialize_system();
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
    val_print(VAL_PRINT_ERR, "\n%4d : %s ", test_num, test_desc);

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
  @param   length  length of buffer
  @param   buffer  buffer pointer
  @return  none
**/
void val_memset_zero(uint32_t *buffer, uint32_t length)
{
    if (buffer == NULL)
        return;

    for (int i = 0; i < length; i++)
    {
        buffer[i] = 0;
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
        val_print(VAL_PRINT_ERR, "\n\tMSG HEADER CHECK: FAILED");
        val_print(VAL_PRINT_ERR, "\n\tMessage header sent %x%08x", sent_msg_hdr);
        val_print(VAL_PRINT_ERR, "\n\tMessage header received %x", rcvd_msg_hdr);

        return VAL_STATUS_FAIL;
    } else
        val_print(VAL_PRINT_DEBUG, "\n\tCHECK HEADER: PASSED [0x%08x]                       ",
                    rcvd_msg_hdr);

    return VAL_STATUS_PASS;
}

/**
  @brief   This function checks if correct protocol version is implemented
           1. Caller       - Test Suite.
  @param   protocol_identifier protocol identifier
  @param   version  scmi implemented version received from platform
  @return  status
**/
uint32_t val_protocol_version_check(uint32_t expected_version, uint32_t version)
{

    if (expected_version != version) {
        val_print(VAL_PRINT_ERR, "\n\tPROTOCOL VERSION CHECK: FAILED");
        val_print(VAL_PRINT_ERR, "\n\tProtocol version expected %x", expected_version);
        val_print(VAL_PRINT_ERR, "\n\tProtocol version received %x                        ",
                                version);
        return VAL_STATUS_FAIL;
    } else {
        val_print(VAL_PRINT_DEBUG, "\n\tPROTOCOL VERSION CHECK: PASSED [%x]                 ",
                   version);
    }
    return VAL_STATUS_PASS;
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
        val_print(VAL_PRINT_ERR, "\n\tRESERVED BITS CHECK: FAILED");
        val_print(VAL_PRINT_ERR, "\n\tReserved bits value received %x", reserved_bits);
        return VAL_STATUS_FAIL;
    } else
        val_print(VAL_PRINT_DEBUG, "\n\tRESERVED BITS CHECK: PASSED                            ");

    return VAL_STATUS_PASS;
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
        val_print(VAL_PRINT_WARN, "\n\tInvalid test status %d", test_status);
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
        return "SCMI_SUCCESS           ";
    case SCMI_NOT_SUPPORTED:
        return "SCMI_NOT_SUPPORTED     ";
    case SCMI_INVALID_PARAMETERS:
        return "SCMI_INVALID_PARAMETERS";
    case SCMI_DENIED:
        return "SCMI_DENIED            ";
    case SCMI_NOT_FOUND:
        return "SCMI_NOT_FOUND         ";
    case SCMI_OUT_OF_RANGE:
        return "SCMI_OUT_OF_RANGE      ";
    case SCMI_BUSY:
        return "SCMI_BUSY              ";
    case SCMI_COMMS_ERROR:
        return "SCMI_COMMS_ERROR       ";
    case SCMI_GENERIC_ERROR:
        return "SCMI_GENERIC_ERROR     ";
    case SCMI_HARDWARE_ERROR:
        return "SCMI_HARDWARE_ERROR    ";
    case SCMI_PROTOCOL_ERROR:
        return "SCMI_PROTOCOL_ERROR    ";
    case SCMI_RESERVED:
        return "SCMI_RESERVED          ";
    default:
        val_print(VAL_PRINT_WARN, "\n\tInvalid status %d", status);
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
        val_print(VAL_PRINT_ERR, "\n\tCHECK STATUS: FAILED");
        val_print(VAL_PRINT_ERR, "\n\tStatus expected %s",
                                    val_get_status_string(expected_status));
        val_print(VAL_PRINT_ERR, "\n\tStatus received %s\t               ",
                    val_get_status_string(status));
        return VAL_STATUS_FAIL;
    } else {
        val_print(VAL_PRINT_DEBUG, "\n\tCHECK STATUS: PASSED %s\t       ",
                    val_get_status_string(status));
        return VAL_STATUS_PASS;
    }
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
void val_send_message(uint32_t msg_hdr, uint32_t num_parameter, uint32_t *parameter_buffer,
                      uint32_t *rcvd_msg_hdr, int32_t *status, uint32_t *rcvd_buffer_size,
                      uint32_t *rcvd_buffer)
{
    uint32_t i;

    val_print(VAL_PRINT_INFO, "\n\t MSG HDR: %x", msg_hdr);
    val_print(VAL_PRINT_INFO, "\n\t NUM PARAM: %d", num_parameter);
    for (i = 0; i < num_parameter; i++)
    {
        val_print(VAL_PRINT_DEBUG, "\n\t PARAMETER_%d = %x", i, parameter_buffer[i]);
    }

    pal_send_message(msg_hdr, (size_t) num_parameter, parameter_buffer, rcvd_msg_hdr, status,
                     rcvd_buffer_size, rcvd_buffer);
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
