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

#include "pal_platform.h"
#include "pal_interface.h"

/**
  @brief   This API is used to call platform function to send command
  @param   cmd message header
  @param   num of cmd parameter
  @param   parameter list
  @param   response message header
  @param   cmd status
  @param   return values count
  @param   return values list
  @return  vendor name
**/
void pal_send_message(uint32_t message_header_send, size_t parameter_count,
        const uint32_t *parameters, uint32_t *message_header_rcv, int32_t *status,
        size_t *return_values_count, uint32_t *return_values)
{
    sgm_send_message(message_header_send, parameter_count, parameters,
                        message_header_rcv, status, return_values_count,
                        return_values);
}

/**
  @brief   This API is used to initialize platform to run tests if needed
  @param   platform info
  @return  success / failure
**/
uint32_t pal_initialize_system(void *info)
{
    return 0; /* No initialization needed for SGM to run test */
}

/**
  @brief   This API is used to print test log
  @param   args values to be printed
  @return  none
**/
void pal_print(uint32_t print_level, const char *format, va_list args)
{
    FILE *file_ptr = fopen(LOG_FILE, "a");
    if (file_ptr)
    {
        vfprintf(file_ptr, format, args);
        fclose(file_ptr);
    }
    else
        printf("ERROR: Log File opening failed");
}

/**
   @brief   This API is used to receive delayed response
   @param   message header received
   @param   status of command processed
   @param   return values count
   @param   return values list
 **/
void pal_receive_delayed_response(uint32_t *message_header_rcv, int32_t *status,
        size_t *return_values_count, uint32_t *return_values)
{
    uint32_t timeout = TIMEOUT; /* In ms*/

    sgm_wait_for_response(message_header_rcv, status,
            return_values_count, return_values, NULL, timeout);
}

/**
   @brief   This API is used to receive notifications from platform
   @param   message header received
   @param   return values count
   @param   return values list
 **/
void pal_receive_notification(uint32_t *message_header_rcv, size_t *return_values_count,
       uint32_t *return_values)
{
    uint32_t timeout = TIMEOUT; /* In ms*/

    sgm_wait_for_notification(message_header_rcv,
            return_values_count, return_values, timeout);
}

