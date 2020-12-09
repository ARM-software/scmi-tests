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

#ifndef __VAL_POWER_DOMAIN_H__
#define __VAL_POWER_DOMAIN_H__

#define MAX_POWER_DOMAINS 10

#define VERSION_OFFSET                         0
#define ATTRIBUTE_OFFSET                       0
#define STATS_ADDR_LOW_OFFSET                  1
#define STATS_ADDR_HIGH_OFFSET                 2
#define STATS_LENGTH_OFFSET                    3
#define DOMAIN_NAME_OFFSET                     1
#define POWER_STATE_OFFSET                     0

#define INVALID_SYNC_FLAG                      0xF
#define INVALID_ASYNC_FLAG                     0xE
#define POWER_STATE_ON                         0
#define INVALID_POWER_STATE                    0xFF000000
#define SYNC_POWER_STATE_CHANGE                0
#define NOTIFY_ENABLE                          1
#define INVALID_NOTIFY_ENABLE                  0xF

typedef struct {
    uint32_t num_power_domain;
    uint32_t pd_stats_addr_low;
    uint32_t pd_stats_addr_high;
    uint32_t pd_stats_addr_len;
    uint32_t pd_noti_support[MAX_POWER_DOMAINS];
    uint32_t pd_async_support[MAX_POWER_DOMAINS];
    uint32_t pd_sync_support[MAX_POWER_DOMAINS];
    uint8_t  pd_name[MAX_POWER_DOMAINS][SCMI_NAME_STR_SIZE];
} POWER_DOMAIN_INFO_s;

/* Common Tests */
uint32_t power_domain_query_protocol_version(uint32_t *version);
uint32_t power_domain_query_protocol_attributes(void);
uint32_t power_domain_query_mandatory_command_support(void);
uint32_t power_domain_invalid_messageid_call(void);
uint32_t power_domain_query_domain_attributes(void);
uint32_t power_domain_query_domain_attributes_invalid_doamin(void);
uint32_t power_domain_set_power_state_check(void);
uint32_t power_domain_set_power_state_unsupported_domain_check(void);
uint32_t power_domain_set_power_state_check_invalid_domain(void);
uint32_t power_domain_get_power_state_check(void);
uint32_t power_domain_get_power_state_check_invalid_domain(void);
uint32_t power_domain_power_state_notify_check(void);
uint32_t power_domain_power_state_notify_unspported_domain_check(void);
uint32_t power_domain_power_state_notify_invalid_domain_check(void);

/* v2 Tests */
uint32_t power_domain_power_state_change_requested_notify_check(void);
uint32_t power_domain_power_state_change_requested_notify_invalid_domain_check(void);

uint32_t val_power_get_expected_num_domains(void);
uint32_t val_power_get_expected_stats_addr_low(void);
uint32_t val_power_get_expected_stats_addr_high(void);
uint32_t val_power_get_expected_stats_addr_len(void);

#endif
