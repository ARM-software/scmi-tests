/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef SYSTEM_POWER_EXPECTED_H_
#define SYSTEM_POWER_EXPECTED_H_

#include <inttypes.h>
#include <stdbool.h>
#include <arm_scmi_constants.h>
#include <test_agent_exports.h>

struct arm_scmi_system_power_expected {
	/*
	 * This enum specifies the bit position in the flags_mask field
	 * indicating if the expected value is given (bit set) or ignored
	 * (bit unset).
	 */
	enum {
		SYSTEM_POWER_PROTOCOL_VERSION,
		SYSTEM_POWER_STATE_NOTIFY_CMD_SUPPORTED
	} FLAGS;
	/*
	 * The expectation is that there is no more
	 * than 32 elements specified via this expected results data structure.
	 * This mask is used to indicate which of the structure members hold valid
	 * data.
	 */
	uint32_t flags_mask;
	uint32_t protocol_version;

	bool system_power_state_notify_cmd_supported;
};

#endif /* SYSTEM_POWER_EXPECTED_H_ */
