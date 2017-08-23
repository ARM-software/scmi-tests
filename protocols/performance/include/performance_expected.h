/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef PERFORMANCE_EXPECTED_H_
#define PERFORMANCE_EXPECTED_H_

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
};



#endif /* PERFORMANCE_EXPECTED_H_ */
