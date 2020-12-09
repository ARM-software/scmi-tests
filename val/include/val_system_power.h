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

#ifndef __VAL_SYSTEM_POWER_H__
#define __VAL_SYSTEM_POWER_H__

#define VERSION_OFFSET                0
#define ATTRIBUTE_OFFSET              0

#define INVALID_FLAG_VAL                    0xF
#define INVALID_NOTIFY_EN_VAL               0xF

typedef struct {
    uint32_t system_power_reset_support;
    uint32_t system_power_suspend_support;
} SYSTEM_POWER_INFO_s;

/* Common Tests */
uint32_t system_power_query_protocol_version(uint32_t *version);
uint32_t system_power_query_protocol_attributes(void);
uint32_t system_power_query_mandatory_command_support(void);
uint32_t system_power_invalid_messageid_call(void);
uint32_t system_power_state_get_check(void);
uint32_t system_power_state_set_invalid_parameters(void);
uint32_t system_power_state_notify_invalid_parameters(void);

#endif
