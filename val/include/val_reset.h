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

#ifndef __VAL_RESET_H__
#define __VAL_RESET_H__

#define MAX_NUM_OF_DOMAINS                 5

#define VERSION_OFFSET                           0
#define ATTRIBUTE_OFFSET                         0
#define LATANCY_OFFSET                           1
#define NAME_OFFSET                              2

#define ARCH_COLD_RESET                          0
#define EXPLICIT_SYNC_DEASSERT_RESET             0
#define INVALID_FLAG_VAL                         0xF8
#define RESET_FLAG_ASYNC_RESET_BIT               0x2
#define RESET_FLAG_AUTONOMOUS_RESET_BIT          0x0
#define RESET_TYPE_BIT                           31
#define RESET_TYPE_ARCHITECTURAL                 0x0
#define RESET_ARCHITECTURAL_INVALID_ID           0xFF
#define RESET_NOTIFY_DISABLE                     0x0
#define RESET_INVALID_NOTIFY_ENABLE              0xF


typedef struct {
    uint32_t async_support;
    uint32_t notify_support;
} RESET_DOMAIN_INFO_s;

typedef struct {
    uint32_t num_domains;
    RESET_DOMAIN_INFO_s domain_info[MAX_NUM_OF_DOMAINS];
} RESET_INFO_s;

/* Common Tests */
uint32_t reset_query_protocol_version(uint32_t *version);
uint32_t reset_query_protocol_attributes(void);
uint32_t reset_query_mandatory_command_support(void);
uint32_t reset_invalid_messageid_call(void);
uint32_t reset_query_domain_attributes(void);
uint32_t reset_query_domain_attributes_invalid_id(void);
uint32_t reset_query_reset_command_invalid_id(void);
uint32_t reset_query_reset_command_invalid_flag(void);
uint32_t reset_query_reset_command_invalid_reset_state(void);
uint32_t reset_query_notify_invalid_id(void);
uint32_t reset_query_invalid_notify_enable(void);

uint32_t val_reset_get_expected_num_domains(void);
uint8_t *val_reset_get_expected_name(uint32_t domain_id);

#endif /*__VAL_RESET_H__*/
