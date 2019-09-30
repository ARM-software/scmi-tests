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

#ifndef PERFORMANCE_EXPECTED_H_
#define PERFORMANCE_EXPECTED_H

#ifdef SCMI_VERSION_1
#define PERFORMANCE_VERSION 0x00010000
#else
#define PERFORMANCE_VERSION 0x00020000
#endif

#define PERF_MGMT_PROTO_ID                  0x13
#define PERF_MGMT_PROTO_VER_MSG_ID          0x0
#define PERF_MGMT_PROTO_ATTRIB_MSG_ID       0x1
#define PERF_MGMT_PROTO_MSG_ATTRIB_MSG_ID   0x2
#define PERF_DOMAIN_ATTRIB_MSG_ID           0x3
#define PERF_DESC_LVL_MSG_ID                0x4
#define PERF_LIMIT_SET_MSG_ID               0x5
#define PERF_LIMIT_GET_MSG_ID               0x6
#define PERF_LVL_SET_MSG_ID                 0x7
#define PERF_LVL_GET_MSG_ID                 0x8
#define PERF_NOTIFY_LIMIT_MSG_ID            0x9
#define PERF_NOTIFY_LVL_MSG_ID              0xA

#define MAX_PERFORMANCE_DOMAIN_COUNT        10

struct arm_scmi_performance_protocol {
    /*This enum is to easily specify the flag_mask and have to be ensured that
     * it is in the same order as the below structure elements after flags_mask
     * member
     * */
    enum {
        PERFORMANCE_PROTOCOL_VERSION,
        PERFORMANCE_STATISTICS_SUPPORTED,
        PERFORMANCE_STATISTICS_ADDRESS_LOW,
        PERFORMANCE_STATISTICS_ADDRESS_HIGH,
        PERFORMANCE_STATISTICS_LENGTH,
        PERFORMANCE_POWER_VALUE_IN_MW,
        PERFORMANCE_NUM_PERFORMANCE_DOMAINS,
        PERFORMANCE_SET_LIMIT_CAPABLE,
        PERFORMANCE_SET_PERFORMANCE_LEVEL_CAPABLE,
        PERFORMANCE_LIMIT_NOTIFICATION_SUPPORT,
        PERFORMANCE_LEVEL_NOTIFICATION_SUPPORT,
        PERFORMANCE_RATE_LIMIT,
        PERFORMANCE_SUSTAINED_FREQ,
        PERFORMANCE_SUSTAINED_LEVEL,
        PERFORMANCE_DOMAIN_NAMES,
        PERFORMANCE_NUM_PERFORMANCE_LEVELS,
        PERFORMANCE_LEVEL_VALUE,
        PERFORMANCE_LEVEL_POWER_COST,
        PERFORMANCE_LEVEL_WORST_LATENCY,
        PERFORMANCE_LIMIT_NOTIFICATION_SUPPORTED,
        PERFORMANCE_LEVEL_NOTIFICATION_SUPPORTED
    } FLAGS;
     /* The expectation is that there is no more than 32 elements specified via
      * this expected results data structure. This mask is used to indicate
      * which of the structure members hold valid data
      * */
    uint32_t flags_mask;
    uint32_t protocol_version;
    /* Set to non-zero value to indicate statistics shared memory region
     * supported
     * */
    uint8_t statistics_supported;
    /*Relevant only when statistics supported is set as True*/
    uint32_t statistics_address_low;
    uint32_t statistics_address_high;
    uint32_t statistics_length;
    /* Set to non-zero value to indicate power value is reported in mW units */
    uint8_t power_value_in_milliwatts;

    uint32_t num_performance_domains;
    /*This should confirm if this agent is capable of setting limit on the given
     * performance domain in this platform bounded by the num_performance_domains
     * variable*/
    uint8_t *set_limit_capable;
    /*This should confirm if this agent is capable of setting performance level
     * on the given performance domain in this platform bounded by the
     * num_performance_domains variable*/
    uint8_t *set_performance_level_capable;
    /*This should confirm if this agent is capable of setting performance limit
     * change notifications on the given performance domain in this platform
     * bounded by the num_performance_domains variable*/
    bool performance_limit_notification_support;
    /*This should confirm if this agent is capable of setting performance level
     * change notifications on the given performance domain in this platform
     * bounded by the num_performance_domains variable*/
    bool performance_level_notification_support;
    /*This should have the list of performance rate limits in the platform
     * bounded by the num_performance_domains variable*/
    uint32_t *performance_rate_limit;
    /*This should have the list of performance sustained freqs in the platform
     * bounded by the num_performance_domains variable*/
    uint32_t *performance_sustained_freq;
    /*This should have the list of performance sustained levels in the platform
     * bounded by the num_performance_domains variable*/
    uint32_t *performance_sustained_level;
    /*This should have the list of performance domain names in the platform
     * bounded by the num_performance_domains variable*/
    char **performance_domain_names;
    /*This should have the list of performance levels in the platform
     * bounded by the num_performance_domains variable*/
    uint32_t *num_performance_levels;
    /*This should have performance value info for each performance level in this
     * platform bounded by the num_performance_levels variable*/
    uint32_t **performance_level_value;
    /*This should have power cost info for each performance level in this
     * platform bounded by the num_performance_levels variable*/
    uint32_t **performance_level_power_cost;
    /*This should have worst case latency info for each performance level in
     * this platform bounded by the num_performance_levels variable*/
    uint32_t **performance_level_worst_latency;
    /*This specifies if the performance level notification is expected to be
     * supported in the platform*/
    uint8_t performance_limit_notification_supported;
    /*This specifies if the performance level notification is expected to be
     * supported in the platform*/
    uint8_t performance_level_notification_supported;
};



#endif /* PERFORMANCE_EXPECTED_H_ */
