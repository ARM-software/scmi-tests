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

#ifndef __PAL_PLATFORM__
#define __PAL_PLATFORM__

#include <inttypes.h>
#include <stddef.h>
#include <assert.h>

#define BASE_PROTOCOL_ID              0x10
#define POWER_DOMAIN_PROTOCOL_ID      0x11
#define SYSTEM_POWER_PROTOCOL_ID      0x12
#define PERFORMANCE_PROTOCOL_ID       0x13
#define CLOCK_PROTOCOL_ID             0x14
#define SENSOR_PROTOCOL_ID            0x15
#define RESET_PROTOCOL_ID             0x16

enum BITS_HEADER {
    RESERVED_LOW =      28,
    RESERVED_HIGH =     31,
    TOKEN_LOW =         18,
    TOKEN_HIGH =        27,
    PROTOCOL_ID_LOW =   10,
    PROTOCOL_ID_HIGH =  17,
    MESSAGE_TYPE_LOW =  8,
    MESSAGE_TYPE_HIGH = 9,
    MESSAGE_ID_LOW =    0,
    MESSAGE_ID_HIGH =   7
};

#define SCMI_EXRACT_BITS(data, bit_upper, bit_lower)\
        ({\
            uint32_t mask, bits;\
            mask = ((1 << (bit_upper - bit_lower + 1)) - 1) << bit_lower;\
            bits = (data & mask) >> bit_lower;\
            bits;\
        })

uint32_t agent_get_accessible_device(uint32_t agent_id);
uint32_t agent_get_inaccessible_device(uint32_t agent_id);
uint32_t device_get_accessible_protocol(uint32_t device_id);
uint32_t check_trusted_agent(uint32_t agent_id);

void base_send_message(uint32_t message_id, uint32_t parameter_count,
        const uint32_t *parameters, int32_t *status,
        size_t *return_values_count, uint32_t *return_values);

void system_power_send_message(uint32_t message_id, uint32_t parameter_count,
        const uint32_t *parameters, int32_t *status,
        size_t *return_values_count, uint32_t *return_values);

void sensor_send_message(uint32_t message_id, uint32_t parameter_count,
        const uint32_t *parameters, int32_t *status,
        size_t *return_values_count, uint32_t *return_values);

void clock_send_message(uint32_t message_id, uint32_t parameter_count,
        const uint32_t *parameters, int32_t *status,
        size_t *return_values_count, uint32_t *return_values);

void performance_send_message(uint32_t message_id, uint32_t parameter_count,
        const uint32_t *parameters, int32_t *status,
        size_t *return_values_count, uint32_t *return_values);

void power_send_message(uint32_t message_id, uint32_t parameter_count,
        const uint32_t *parameters, int32_t *status,
        size_t *return_values_count, uint32_t *return_values);

void reset_send_message(uint32_t message_id, uint32_t parameter_count,
        const uint32_t *parameters, int32_t *status,
        size_t *return_values_count, uint32_t *return_values);

void fill_base_protocol(void);
void fill_power_protocol(void);
void fill_clock_protocol(void);
void fill_sensor_protocol(void);
void fill_performance_protocol(void);
void fill_reset_protocol(void);

#endif /*__PAL_PLATFORM__*/
