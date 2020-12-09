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

#ifndef _CLOCK_PROTOCOL_H_
#define _CLOCK_PROTOCOL_H_

#include <inttypes.h>
#include <stdbool.h>

#define CLOCK_VERSION 0x00010000

#define CLK_PROTO_ID                0x14
#define CLK_PROTO_VER_MSG_ID        0x0
#define CLK_PROTO_ATTRIB_MSG_ID     0x1
#define CLK_PROTO_MSG_ATTRIB_MSG_ID 0x2
#define CLK_ATTRIB_MSG_ID           0x3
#define CLK_DESC_RATE_MSG_ID        0x4
#define CLK_RATE_SET_MSG_ID         0x5
#define CLK_RATE_GET_MSG_ID         0x6
#define CLK_CONFIG_SET_MSG_ID       0x7

#define MAX_NUM_CLOCK           16
#define MAX_NUM_RATES_SUPPORTED 16
#define THREE_TUPLE_RATE_FORMAT 1
#define LOWEST_RATE             0
#define HIGHEST_RATE            1
#define STEP_SIZE               2
#define CLOCK_ENABLED           1
#define CLOCK_DISABLED          0

/* Enum to use constants instead of indexes for test suites */
enum clock_test_suite_index {
    TS_CLOCK_DISCOVERY,
    TS_CLOCK_ATTRIBUTE,
    TS_CLOCK_DESC_RATE,
    TS_CLOCK_RATE_SET,
    TS_CLOCK_RATE_GET,
    TS_CLOCK_CONFIG_SET
};

struct arm_scmi_clock_rate {
    uint32_t lower;
    uint32_t upper;
};

struct arm_scmi_clock_protocol {
    /*
     * This enum is to specify the flag_mask and have to be ensured that
     *  it is in the same order as the below structure elements after
     *  flags_mask member.
     */
    enum {
        CLOCK_PROTOCOL_VERSION,
        NUMBER_CLOCKS,
        MAX_NUM_PENDING_ASYNC_RATE_CHG_SUPPORTED,
        RATE_FORMAT_SUPPORTED,
        THREE_TUPLE_CLOCK_RATES,
        DISCRETE_CLOCK_RATES,
        CLOCK_NUM_RATES,
        CLOCK_NAMES
    } FLAGS;
    /*
     * The expectation is that there is no more
     * than 32 elements specified via this expected results data structure.
     * This mask is used to indicate which of the structure members hold valid
     * data.
     */
    uint32_t flags_mask;
    uint32_t protocol_version;
    uint32_t number_clocks;
    uint32_t max_num_pending_async_rate_chg_supported;

    /*
     * 1 for this field specifies if the rate format returned is a 3 tuple
     * (low, high, num_of_rates), 0 are discrete clock rates.
     */
    uint32_t *rate_format_supported;

    /*
     * This should have the low clock rate of the 3 tuple value for each clock
     * in this platform bounded by the num_clocks variable. This is valid only
     * when three_tuple_rate_format_supported is set.
     */
    struct arm_scmi_clock_rate *low_clock_rate;

    /*
     * This should have the high clock rate of the 3 tuple value for each clock
     * in this platform bounded by the num_clocks variable. This is valid only
     * when three_tuple_rate_format_supported is set.
     */
    struct arm_scmi_clock_rate *high_clock_rate;

    /*
     * This should have the step clock rate of the 3 tuple value for each clock
     * in this platform bounded by the num_clocks variable. This is valid only
     * when three_tuple_rate_format_supported is set.
     */
    struct arm_scmi_clock_rate *step_clock_rate;

    /*
     * This should have the number of discrete supported clock rates
     * for each clock in this platform bounded by the num_clocks variable. This
     * is only valid when three_tuple_rate_format_supported is set.
     */
    struct arm_scmi_clock_rate **discrete_clock_rates;

    /*
     * This should have the number of rates info for each clock in this platform
     * bounded by the num_clocks variable.
     */
    uint32_t *num_rates;

    /*
     * This should have the list of clock names in the platform bounded by the
     * num_clocks variable.
     */
    const char **clock_name;

};

#endif /* CLOCK_EXPECTED_H_ */
