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

#ifndef __PAL_PLATFORM_H__
#define __PAL_PLATFORM_H__

#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>

#define LOG_FILE "arm_scmi_test_log.txt"

#define VERSION_ENCODE32(MAJOR, MINOR, PATCH) (((MAJOR & 0xff) << 24) | \
                                                ((MINOR & 0xff) << 16)| \
                                                (PATCH & 0xffff))
#define BASE_PROTOCOL_ID              0x10
#define PWR_DOMAIN_PROTOCOL_ID        0x11
#define SYSTEM_POWER_PROTOCOL_ID      0x12
#define PERFORMANCE_PROTOCOL_ID       0x13
#define CLOCK_PROTOCOL_ID             0x14
#define SENSOR_PROTOCOL_ID            0x15
#define RESET_PROTOCOL_ID             0x16
#define APCORE_PROTOCOL_ID            0x09

#define TIMEOUT  100

int32_t sgm_send_message(uint32_t message_header_send, size_t parameter_count,
        const uint32_t *parameters, uint32_t *message_header_rcv, int32_t *status,
        size_t *return_values_count, uint32_t *return_values);

int sgm_wait_for_notification(uint32_t *message_header_rcv,
        size_t *return_values_count, uint32_t *return_values,
        uint32_t timeout);

int sgm_wait_for_response(uint32_t *message_header_rcv,
        int32_t *status, size_t *return_values_count, uint32_t *return_values,
        bool *message_ready, uint32_t timeout);

uint32_t sgm_agent_get_accessible_device(uint32_t agent_id);
uint32_t sgm_agent_get_inaccessible_device(uint32_t agent_id);
uint32_t sgm_device_get_accessible_protocol(uint32_t device_id);

#endif
