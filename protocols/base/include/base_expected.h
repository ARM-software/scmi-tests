/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef BASE_EXPECTED_H_
#define BASE_EXPECTED_H_

#include <inttypes.h>
#include <stdbool.h>
#include <arm_scmi_constants.h>

#define VENDOR_ID_SIZE 16
#define SUB_VENDOR_ID_SIZE 16

struct arm_scmi_base_expected {
	/*
	 * This enum specifies the bit position in the flags_mask field
	 * indicating if the expected value is given (bit set) or ignored
	 * (bit unset).
	 */
	enum {
		BASE_PROTOCOL_VERSION,
		VENDOR_IDENTIFIER,
		SUBVENDOR_IDENTIFIER,
		IMPLEMENTATION_VERSION,
		NUM_PROTOCOLS_IMPLEMENTED,
		SUPPORTED_PROTOCOLS,
		NUMBER_AGENTS,
		AGENT_NAMES,
		ERROR_NOTIFICATION_SUPPORTED,
		SUBVENDOR_CMD_SUPPORTED,
		DISCOVER_AGENT_CMD_SUPPORTED
	} FLAGS;
	/*
	 * The expectation is that there is no more
	 * than 32 elements specified via this expected results data structure.
	 * This mask is used to indicate which of the structure members hold valid
	 * data.
	 */
	uint32_t flags_mask;
	uint32_t protocol_version;
	/* The name of the vendor as a string */
	char vendor_identifier[VENDOR_ID_SIZE];
	/* The name of the sub vendor as a string */
	char subvendor_identifier[SUB_VENDOR_ID_SIZE];
	uint32_t implementation_version;
	uint32_t num_protocols_implemented;
	/*
	 * This should have the list of protocol_ids for the commands expected to be
	 * supported in this platform limited by the num_protocols_implemented
	 * variable.
	 */
	uint32_t *supported_protocols;
	uint32_t num_agents;
	/*
	 * This should have the list of agent names for the all the agents in the
	 * platform limited by the num_agents variable, if agent_discoverable is
	 * set to non-zero value
	 */
	char **agent_names;
	/*
	 * This specifies if the error notification is expected to be supported in
	 * the platform.
	 */
	bool notify_error_cmd_supported;
	/*
	 * This specifies if the sub vendor command is expected to be supported in
	 * the platform.
	 */
	bool subvendor_cmd_supported;
	/*
	 * This specifies if the discover agent command is expected to be supported
	 * in the platform.
	 */
	bool discover_agent_cmd_supported;
};

#endif /* BASE_EXPECTED_H_ */
