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
#define PERF_EXPECTED_NUM_OF_DOMAIN 4
static uint32_t num_performance_domains = PERF_EXPECTED_NUM_OF_DOMAIN;

static uint32_t statistics_address_low_perf = 0x1234;
static uint32_t statistics_address_len_perf = 0xFF;

static uint8_t  set_limit_capable[] = {1, 1, 1 ,0};
static uint8_t  set_performance_level_capable[] = {1, 1, 1 ,0};
static uint8_t  performance_level_notification_support[] = {0, 0, 0 ,0};
static uint8_t  performance_limit_notification_support[] = {0, 0, 0 ,0};
static uint32_t performance_rate_limit[] = {10, 20, 30 ,40};
static uint32_t performance_sustained_freq[] = {100, 200, 300 ,400};
static uint32_t performance_sustained_level[] = {110, 120, 130 ,140};

static char* performance_domain_names[] = {
    "Domain_0",
    "Domain_1",
    "Domain_2",
    "Domain_3"
};

static uint32_t perf_num_performance_levels[] = {
    3, /* Domain 0 */
    4, /* Domain 1 */
    5, /* Domain 2 */
    6, /* Domain 3 */
};

static uint32_t *perf_performance_level_values[] = {
    (uint32_t[]) { /* Domain 0 */
        665000000, /* 665MHz */
        998000000, /* 998MHz */
        1330000000, /* 1330MHz */
    },
    (uint32_t[]) { /* 1 */
        1313000000,  /* 1313MHz */
        1531000000,  /* 1531MHz */
        1750000000,  /* 1750MHz */
        2100000000,  /* 2100MHz */
    },
    (uint32_t[]) { /* 2 */
        450000000, /* 450MHz */
        487500000, /* 487.5MHz */
        525000000, /* 525MHz */
        562500000, /* 562.5MHz */
        600000000, /* 600MHz */
    },
    (uint32_t[]) { /* 3 */
        550000000, /* 550MHz */
        687500000, /* 687.5MHz */
        725000000, /* 725MHz */
        862500000, /* 862.5MHz */
        900000000, /* 900MHz */
        910000000, /* 910MHz */
    },
};

static uint32_t *perf_performance_level_power_costs[] = {
    (uint32_t[]) { /* 0 */
        4 * 0.110 * 665 * (800 * 800) / 1000000,
        4 * 0.110 * 998 * (850 * 850) / 1000000,
        4 * 0.110 * 1330 * (900 * 900) / 1000000,
    },
    (uint32_t[]) { /* 1 */
        4 * 0.453 * 1313 * (800 * 800) / 1000000,
        4 * 0.453 * 1531 * (850 * 850) / 1000000,
        4 * 0.453 * 1750 * (900 * 900) / 1000000,
        4 * 0.453 * 2100 * (950 * 950) / 1000000,
    },
    (uint32_t[]) { /* 2 */
        16 * 0.640 * 450 * (800 * 800) / 1000000,
        16 * 0.640 * 487 * (850 * 850) / 1000000,
        16 * 0.640 * 525 * (900 * 900) / 1000000,
        16 * 0.640 * 562 * (950 * 950) / 1000000,
        16 * 0.640 * 600 * (1000 * 1000) / 1000000,
    },
    (uint32_t[]) { /* 3 */
        8 * 0.640 * 450 * (800 * 800) / 1000000,
        8 * 0.640 * 487 * (850 * 850) / 1000000,
        8 * 0.640 * 525 * (900 * 900) / 1000000,
        8 * 0.640 * 562 * (950 * 950) / 1000000,
        8 * 0.640 * 600 * (1000 * 1000) / 1000000,
        8 * 0.453 * 2100 * (950 * 950) / 1000000,
    },
};

static uint32_t * perf_performance_level_worst_latency[] = {
    (uint32_t[]) {
        1100,
        1200,
        1300,
    },
    (uint32_t[]) {
        1400,
        1500,
        1600,
        1700,
    },
    (uint32_t[]) {
        100,
        110,
        120,
        130,
        140,
    },
    (uint32_t[]) {
        150,
        160,
        170,
        180,
        190,
        200,
    },
};

static uint8_t perf_msg_fast_ch_support[] = {
    0, /* Version*/
    0, /* Protocol attr*/
    0, /* Msg attr*/
    0, /* Domain attr*/
    0, /* Describe level*/
    1, /* Limit set*/
    0, /* limit get*/
    1, /* Level set*/
    0, /* Level get*/
    0, /* Notify limit*/
    0, /* Notify level*/
    0, /* Describe fast ch*/
};

static uint8_t perf_domain_fast_ch_support[] = {
    1, /* Domain 0 */
    1, /* Domain 1 */
    0, /* Domain 2 */
    1, /* Domain 3 */
};

static uint8_t * perf_performance_fast_ch_doorbell_support[] = {
    (uint8_t[]) { /* Domain 0 */
        1, /* Limit set*/
        1, /* Limit get*/
        0, /* Level set*/
        1, /* Level get*/
    },
    (uint8_t[]) { /* Domain 1 */
        0, /* Limit set*/
        1, /* Limit get*/
        0, /* Level set*/
        1, /* Level get*/
    },
    (uint8_t[]) { /* Domain 2*/
        0, /* Limit set*/
        1, /* Limit get*/
        1, /* Level set*/
        1, /* Level get*/
    },
    (uint8_t[]) { /* Domain 3 */
        0, /* Limit set*/
        1, /* Limit get*/
        0, /* Level set*/
        1, /* Level get*/
    },
};

static uint8_t * perf_performance_fast_ch_doorbell_data_width[] = {
    (uint8_t[]) { /* Domain 0 */
        0, /* Limit set*/
        0, /* Limit get*/
        0, /* Level set*/
        0, /* Level get*/
    },
    (uint8_t[]) { /* Domain 1 */
        0, /* Limit set*/
        1, /* Limit get*/
        0, /* Level set*/
        0, /* Level get*/
    },
    (uint8_t[]) { /* Domain 2*/
        0, /* Limit set*/
        0, /* Limit get*/
        2, /* Level set*/
        0, /* Level get*/
    },
    (uint8_t[]) { /* Domain 3 */
        0, /* Limit set*/
        0, /* Limit get*/
        0, /* Level set*/
        3, /* Level get*/
    },
};

static uint32_t * perf_performance_fast_chan_addr_low[] = {
    (uint32_t[]) { /* Domain 0 */
        1000, /* Limit set*/
        2000, /* Limit get*/
        3000, /* Level set*/
        4000, /* Level get*/
    },
    (uint32_t[]) { /* Domain 1 */
        5000, /* Limit set*/
        6000, /* Limit get*/
        7000, /* Level set*/
        8000, /* Level get*/
    },
    (uint32_t[]) { /* Domain 2*/
        9000,  /* Limit set*/
        10000, /* Limit get*/
        11000, /* Level set*/
        12000, /* Level get*/
    },
    (uint32_t[]) { /* Domain 3 */
        13000, /* Limit set*/
        14000, /* Limit get*/
        15000, /* Level set*/
        16000, /* Level get*/
    },
};

static uint32_t * perf_performance_fast_chan_addr_high[] = {
    (uint32_t[]) { /* Domain 0 */
        100000, /* Limit set*/
        200000, /* Limit get*/
        300000, /* Level set*/
        400000, /* Level get*/
    },
    (uint32_t[]) { /* Domain 1 */
        500000, /* Limit set*/
        600000, /* Limit get*/
        700000, /* Level set*/
        800000, /* Level get*/
    },
    (uint32_t[]) { /* Domain 2*/
        900000,  /* Limit set*/
        1000000, /* Limit get*/
        1100000, /* Level set*/
        1200000, /* Level get*/
    },
    (uint32_t[]) { /* Domain 3 */
        1300000, /* Limit set*/
        1400000, /* Limit get*/
        1500000, /* Level set*/
        1600000, /* Level get*/
    },
};

static uint32_t * perf_performance_fast_chan_size[] = {
    (uint32_t[]) { /* Domain 0 */
        10, /* Limit set*/
        20, /* Limit get*/
        30, /* Level set*/
        40, /* Level get*/
    },
    (uint32_t[]) { /* Domain 1 */
        50, /* Limit set*/
        60, /* Limit get*/
        70, /* Level set*/
        80, /* Level get*/
    },
    (uint32_t[]) { /* Domain 2*/
        90,  /* Limit set*/
        100, /* Limit get*/
        110, /* Level set*/
        120, /* Level get*/
    },
    (uint32_t[]) { /* Domain 3 */
        130, /* Limit set*/
        140, /* Limit get*/
        150, /* Level set*/
        160, /* Level get*/
    },
};

static uint32_t * perf_performance_fast_chan_rate_limit[] = {
    (uint32_t[]) { /* Domain 0 */
        1500, /* Limit set*/
        2500, /* Limit get*/
        3500, /* Level set*/
        4500, /* Level get*/
    },
    (uint32_t[]) { /* Domain 1 */
        5500, /* Limit set*/
        6500, /* Limit get*/
        7500, /* Level set*/
        8500, /* Level get*/
    },
    (uint32_t[]) { /* Domain 2*/
        9500,  /* Limit set*/
        10500, /* Limit get*/
        11500, /* Level set*/
        12500, /* Level get*/
    },
    (uint32_t[]) { /* Domain 3 */
        13500, /* Limit set*/
        14500, /* Limit get*/
        15500, /* Level set*/
        16500, /* Level get*/
    },
};

static uint32_t * perf_performance_fast_chan_doorbell_addr_low[] = {
    (uint32_t[]) { /* Domain 0 */
        100000, /* Limit set*/
        200000, /* Limit get*/
        300000, /* Level set*/
        400000, /* Level get*/
    },
    (uint32_t[]) { /* Domain 1 */
        500000, /* Limit set*/
        600000, /* Limit get*/
        700000, /* Level set*/
        800000, /* Level get*/
    },
    (uint32_t[]) { /* Domain 2*/
        900000,  /* Limit set*/
        1000000, /* Limit get*/
        1100000, /* Level set*/
        1200000, /* Level get*/
    },
    (uint32_t[]) { /* Domain 3 */
        1300000, /* Limit set*/
        1400000, /* Limit get*/
        1500000, /* Level set*/
        1600000, /* Level get*/
    },
};

static uint32_t * perf_performance_fast_chan_doorbell_addr_high[] = {
    (uint32_t[]) { /* Domain 0 */
        1000000, /* Limit set*/
        2000000, /* Limit get*/
        3000000, /* Level set*/
        4000000, /* Level get*/
    },
    (uint32_t[]) { /* Domain 1 */
        5000000, /* Limit set*/
        6000000, /* Limit get*/
        7000000, /* Level set*/
        8000000, /* Level get*/
    },
    (uint32_t[]) { /* Domain 2*/
        9000000,  /* Limit set*/
        10000000, /* Limit get*/
        11000000, /* Level set*/
        12000000, /* Level get*/
    },
    (uint32_t[]) { /* Domain 3 */
        13000000, /* Limit set*/
        14000000, /* Limit get*/
        15000000, /* Level set*/
        16000000, /* Level get*/
    },
};

static uint32_t * perf_performance_fast_chan_doorbell_set_mask_low[] = {
    (uint32_t[]) { /* Domain 0 */
        100001, /* Limit set*/
        200001, /* Limit get*/
        300001, /* Level set*/
        400001, /* Level get*/
    },
    (uint32_t[]) { /* Domain 1 */
        500001, /* Limit set*/
        600001, /* Limit get*/
        700001, /* Level set*/
        800001, /* Level get*/
    },
    (uint32_t[]) { /* Domain 2*/
        900001,  /* Limit set*/
        1000001, /* Limit get*/
        1100001, /* Level set*/
        1200001, /* Level get*/
    },
    (uint32_t[]) { /* Domain 3 */
        1300001, /* Limit set*/
        1400001, /* Limit get*/
        1500001, /* Level set*/
        1600001, /* Level get*/
    },
};

static uint32_t * perf_performance_fast_chan_doorbell_set_mask_high[] = {
    (uint32_t[]) { /* Domain 0 */
        1000001, /* Limit set*/
        2000001, /* Limit get*/
        3000001, /* Level set*/
        4000001, /* Level get*/
    },
    (uint32_t[]) { /* Domain 1 */
        5000001, /* Limit set*/
        6000001, /* Limit get*/
        7000001, /* Level set*/
        8000001, /* Level get*/
    },
    (uint32_t[]) { /* Domain 2*/
        9000001,  /* Limit set*/
        10000001, /* Limit get*/
        11000001, /* Level set*/
        12000001, /* Level get*/
    },
    (uint32_t[]) { /* Domain 3 */
        13000001, /* Limit set*/
        14000001, /* Limit get*/
        15000001, /* Level set*/
        16000001, /* Level get*/
    },
};

static uint32_t * perf_performance_fast_chan_doorbell_preserve_mask_low[] = {
    (uint32_t[]) { /* Domain 0 */
        100002, /* Limit set*/
        200002, /* Limit get*/
        300002, /* Level set*/
        400002, /* Level get*/
    },
    (uint32_t[]) { /* Domain 1 */
        500002, /* Limit set*/
        600002, /* Limit get*/
        700002, /* Level set*/
        800002, /* Level get*/
    },
    (uint32_t[]) { /* Domain 2*/
        900002,  /* Limit set*/
        1000002, /* Limit get*/
        1100002, /* Level set*/
        1200002, /* Level get*/
    },
    (uint32_t[]) { /* Domain 3 */
        1300002, /* Limit set*/
        1400002, /* Limit get*/
        1500002, /* Level set*/
        1600002, /* Level get*/
    },
};

static uint32_t * perf_performance_fast_chan_doorbell_preserve_mask_high[] = {
    (uint32_t[]) { /* Domain 0 */
        1000002, /* Limit set*/
        2000002, /* Limit get*/
        3000002, /* Level set*/
        4000002, /* Level get*/
    },
    (uint32_t[]) { /* Domain 1 */
        5000002, /* Limit set*/
        6000002, /* Limit get*/
        7000002, /* Level set*/
        8000002, /* Level get*/
    },
    (uint32_t[]) { /* Domain 2*/
        9000002,  /* Limit set*/
        10000002, /* Limit get*/
        11000002, /* Level set*/
        12000002, /* Level get*/
    },
    (uint32_t[]) { /* Domain 3 */
        13000000, /* Limit set*/
        14000000, /* Limit get*/
        15000000, /* Level set*/
        16000000, /* Level get*/
    },
};

#endif /* __PAL_PERFORMANCE_EXPECTED_H__ */
