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

#include "pal_interface.h"
#include "pal_platform.h"

extern void pal_base_set_expected(const void *);
#ifdef POWER_DOMAIN_PROTOCOL
extern void pal_power_set_expected(const void *);
#endif
#ifdef SYSTEM_POWER_PROTOCOL
extern void pal_system_power_set_expected(const void *);
#endif
#ifdef CLOCK_PROTOCOL
extern void pal_clock_set_expected(const void *);
#endif
#ifdef SENSOR_PROTOCOL
extern void pal_sensor_set_expected(const void *);
#endif
#ifdef PERFORMANCE_PROTOCOL
extern void pal_performance_set_expected(const void *);
#endif
#ifdef RESET_PROTOCOL
extern void pal_reset_set_expected(const void *);
#endif

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
    arm_scmi_send_message(message_header_send, parameter_count, parameters,
                          message_header_rcv, status, return_values_count, return_values);
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
    arm_scmi_wait_for_response(message_header_rcv, status, return_values_count, return_values);
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
    arm_scmi_wait_for_notification(message_header_rcv, return_values_count, return_values);
}

/**
  @brief   This API is used to initialize platform to run tests if needed
  @param   platform info
  @return  success / failure
**/
uint32_t pal_initialize_system(void *info)
{
    struct arm_scmi_platform_info *platform_info;
    struct arm_scmi_platform_protocol_info *protocol_info;
    uint32_t index = 0;

    if (info == NULL)
        return PAL_STATUS_FAIL;
    platform_info = (struct arm_scmi_platform_info *) info;

    if (platform_info->platform_protocol_info_table == NULL)
        return PAL_STATUS_FAIL;
    protocol_info = (struct arm_scmi_platform_protocol_info *)
            platform_info->platform_protocol_info_table;

    while (index < platform_info->platform_protocol_info_count) {
         switch (protocol_info->protocol_id)
         {
         case 0x10:
             pal_base_set_expected(protocol_info->expected_return_values);
             break;
#ifdef POWER_DOMAIN_PROTOCOL
         case 0x11:
             pal_power_set_expected(protocol_info->expected_return_values);
             break;
#endif
#ifdef SYSTEM_POWER_PROTOCOL
         case 0x12:
             pal_system_power_set_expected(protocol_info->expected_return_values);
             break;
#endif
#ifdef PERFORMANCE_PROTOCOL
         case 0x13:
             pal_performance_set_expected(protocol_info->expected_return_values);
             break;
#endif
#ifdef CLOCK_PROTOCOL
         case 0x14:
             pal_clock_set_expected(protocol_info->expected_return_values);
             break;
#endif
#ifdef SENSOR_PROTOCOL
         case 0x15:
             pal_sensor_set_expected(protocol_info->expected_return_values);
             break;
#endif
#ifdef RESET_PROTOCOL
         case 0x16:
             pal_reset_set_expected(protocol_info->expected_return_values);
             break;
#endif
         }
         protocol_info++; index++;
    }
    return PAL_STATUS_PASS;
}

/**
  @brief   This API is used to print test log
  @param   args values to be printed
  @return  none
**/
void pal_print(uint32_t print_level, const char *format, va_list args)
{
    char p_str[LOG_STR_SIZE];
    vsprintf(p_str, format, args);
    arm_scmi_log_output(p_str);
}
