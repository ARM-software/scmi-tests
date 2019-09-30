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

#ifndef __VAL_POWER_DOMAIN_H__
#define __VAL_POWER_DOMAIN_H__

#define MAX_POWER_DOMAINS 10

#ifdef SCMI_VERSION_1
#define POWER_DOMAIN_VERSION 0x00010000
#else
#define POWER_DOMAIN_VERSION 0x00020000
#endif

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

#endif
