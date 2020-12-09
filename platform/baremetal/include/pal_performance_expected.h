/** @file
 * Copyright (c) 2017-2020, Arm Limited or its affiliates. All rights reserved.
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

struct arm_scmi_performance_expected {
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
    uint8_t *performance_limit_notification_support;
    /*This should confirm if this agent is capable of setting performance level
     * change notifications on the given performance domain in this platform
     * bounded by the num_performance_domains variable*/
    uint8_t *performance_level_notification_support;
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
    /*This specifies if the fast channel is expected to be supported by messages
     *bounded by the number of messages */
    uint8_t *performance_message_fast_channel_support;
    /*This specifies if the fast channel is expected to be supported by platform
     *bounded by the number of domains */
    uint8_t *performance_domain_fast_channel_support;
    /*This should have door bell support of each command in
     * this platform bounded by the number of domain*/
    uint8_t **door_bell_support;
    /*This should have doorbell data width of each command in
     * this platform bounded by the number of domain*/
    uint8_t **door_bell_data_width;
    /*This should have fast channel rate limit of each command in
     * this platform bounded by the number of domain*/
    uint32_t **fast_channel_rate_limit;
    /*This should have Lower 32 bits of the FastChannel address of each command in
     * this platform bounded by the number of domain*/
    uint32_t **fast_chan_addr_low;
    /*This should have upper 32 bits of the FastChannel address of each command in
     * this platform bounded by the number of domain*/
    uint32_t **fast_chan_addr_high;
    /*This should have size of FastChannel in bytes for each command in
     * this platform bounded by the number of domain*/
    uint32_t **fast_chan_size;
    /* Lower 32 bits of the doorbell address. This field is not used if
     * doorbell is not supported, bounded by the number of domain*/
    uint32_t **doorbell_addr_low;
    /* Upper 32 bits of the doorbell address. This field is not used if
     * doorbell is not supported, bounded by the number of domain*/
    uint32_t **doorbell_addr_high;
    /* Contains a mask of lower 32 bits to set when writing to the doorbell register.
     * If the doorbell register width, n, is less than 32 bits, then only n
     * lower bits are considered from this mask*/
    uint32_t **doorbell_set_mask_low;
    /* Contains a mask of upper 32 bits to set when writing to the doorbell register.
     * This field is only valid if the doorbell register width is 64 bits.*/
    uint32_t **doorbell_set_mask_high;
    /* Contains a mask of lower 32 bits to preserve when writing to the doorbell register.
     * If the doorbell register width, n, is less than 32 bits, then only n lower bits are
     * considered from this mask*/
    uint32_t **doorbell_preserve_mask_low;
    /* Contains a mask of higher 32 bits to preserve when writing to the doorbell register.
     * This field is only valid if the doorbell register width is 64 bits. */
    uint32_t **doorbell_preserve_mask_high;
};

#endif /* __PAL_PERFORMANCE_EXPECTED_H__ */
