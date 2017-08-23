/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef POWER_EXPECTED_H_
#define POWER_EXPECTED_H_

#include <inttypes.h>
#include <stdbool.h>
#include <arm_scmi_constants.h>

struct arm_scmi_power_expected {
	/*
	 * This enum specifies the bit position in the flags_mask field
	 * indicating if the expected value is given (bit set) or ignored
	 * (bit unset).
	 */
	enum {
		POWER_PROTOCOL_VERSION,
		STATISTICS_ADDRESS_LOW,
		STATISTICS_ADDRESS_HIGH,
		STATISTICS_LEN,
		NUM_POWER_DOMAINS,
		POWER_STATE_NOTIFICATION_SUPPORT,
		POWER_SYNCHRONOUS_SUPPORT,
		POWER_ASYNCHRONOUS_SUPPORT,
		POWER_DOMAIN_NAMES,
		POWER_STATE_NOTIFY_CMD_SUPPORTED
	} FLAGS;
	/*
	 * The expectation is that there is no more
	 * than 32 elements specified via this expected results data structure.
	 * This mask is used to indicate which of the structure members hold valid
	 * data.
	 */
	uint32_t flags_mask;
	uint32_t protocol_version;
	/* Relevant only when statistics supported is set as True */
	uint32_t statistics_address_low;
	uint32_t statistics_address_high;
	uint32_t statistics_length;
	uint32_t num_power_domains;
	/*This should have notification support info for each power domain in this
	 * platform bounded by the num_power_domains variable*/
	uint8_t *power_state_notification_support;
	/* This should have power synchronous support info for each power domain in
	 * this platform bounded by the num_power_domains variable */
	uint8_t *power_synchronous_support;
	/* This should have power asynchronous support info for each power domain in
	 * this platform bounded by the num_power_domains variable */
	uint8_t *power_asynchronous_support;
	/* This should have the list of power domain names in the platform bounded
	 * by the num_power_domains variable */
	char **power_domain_names;
	/* This specifies if the power state notify command is expected to be
	 * supported in the platform or not available to the agent */
	bool power_state_notify_cmd_supported;
};

#endif /* POWER_EXPECTED_H_ */
