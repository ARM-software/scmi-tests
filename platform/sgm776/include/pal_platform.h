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

#ifndef __PAL_PLATFORM_H__
#define __PAL_PLATFORM_H__

#include <inttypes.h>

#define LOG_FILE "arm_scmi_test_log.txt"

int32_t sgm_send_message(uint32_t message_header_send, uint32_t parameter_count,
        const uint32_t *parameters, uint32_t *message_header_rcv, int32_t *status,
        uint32_t *return_values_count, uint32_t *return_values);

uint32_t sgm_agent_get_accessible_device(uint32_t agent_id);
uint32_t sgm_agent_get_inaccessible_device(uint32_t agent_id);
uint32_t sgm_device_get_accessible_protocol(uint32_t device_id);

#endif
