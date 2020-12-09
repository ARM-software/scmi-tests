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

#include <pal_platform.h>
#include <pal_interface.h>

void pal_send_message(uint32_t message_header_send, size_t parameter_count,
        const uint32_t *parameters, uint32_t *message_header_rcv, int32_t *status,
        size_t *return_values_count, uint32_t *return_values)
{
    int message_id;
    int protocol_id;

    *message_header_rcv = message_header_send;
    protocol_id = SCMI_EXRACT_BITS(message_header_send,
            PROTOCOL_ID_HIGH, PROTOCOL_ID_LOW);
    message_id = SCMI_EXRACT_BITS(message_header_send,
            MESSAGE_ID_HIGH, MESSAGE_ID_LOW);

    switch (protocol_id)
    {
    case BASE_PROTOCOL_ID:
        base_send_message(message_id, parameter_count, parameters, status,
                return_values_count, return_values);
        break;
    case POWER_DOMAIN_PROTOCOL_ID:
        power_send_message(message_id, parameter_count, parameters, status,
                return_values_count, return_values);
        break;
    case SYSTEM_POWER_PROTOCOL_ID:
        system_power_send_message(message_id, parameter_count, parameters, status,
                return_values_count, return_values);
        break;
    case PERFORMANCE_PROTOCOL_ID:
        performance_send_message(message_id, parameter_count, parameters, status,
                return_values_count, return_values);
        break;
    case CLOCK_PROTOCOL_ID:
        clock_send_message(message_id, parameter_count, parameters, status,
                return_values_count, return_values);
        break;
    case SENSOR_PROTOCOL_ID:
        sensor_send_message(message_id, parameter_count, parameters, status,
                return_values_count, return_values);
        break;
    case RESET_PROTOCOL_ID:
        reset_send_message(message_id, parameter_count, parameters, status,
                return_values_count, return_values);
        break;
    default:
        printf("\nProtocol: %d\n", message_id);
        assert(!"\nUnknown protocol id\n");
        break;
    }
}

void pal_receive_delayed_response(uint32_t *message_header_rcv, int32_t *status,
        size_t *return_values_count, uint32_t *return_values)
{
    return ;
}

void pal_receive_notification(uint32_t *message_header_rcv, size_t *return_values_count,
       uint32_t *return_values)
{
    return ;
}

uint32_t pal_initialize_system(void *info)
{
    fill_base_protocol();
    fill_power_protocol();
    fill_performance_protocol();
    fill_sensor_protocol();
    fill_clock_protocol();
    fill_reset_protocol();

    return PAL_STATUS_PASS;
}

void pal_print(uint32_t print_level, const char *format, va_list args)
{
    vprintf(format, args);
}

void *pal_memcpy(void *dest, const void *src, size_t size)
{
    if (dest == NULL || src == NULL || size == 0)
        return NULL;
    return memcpy(dest, src, size);
}
