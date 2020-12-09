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

#ifndef __PAL_PERFORMANCE_EXPECTED_H__
#define __PAL_PERFORMANCE_EXPECTED_H__

/* Expected PERFORMANCE parameters */
#ifdef PERFORMANCE_PROTOCOL
static uint32_t num_perf_domains = 0x03;

static uint32_t perf_num_performance_levels[] = {
    5, /* LITTLE */
    5, /* BIG */
    5  /* GPU */
};

static char *performance_domain_names[] = {
    "CPU_GROUP_LITTL",
    "CPU_GROUP_BIG",
    "GPU"
};

uint32_t statistics_address_low_perf = 0;
uint32_t statistics_address_len_perf = 0;
#endif

#endif /* __PAL_PERFORMANCE_EXPECTED_H__ */
