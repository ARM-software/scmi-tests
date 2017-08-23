/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <test_executor.h>

/*!
 * @brief Obtain the expected return values from the platform details for the
 * protocol defined by its protocol identifier.
 *
 *  @param platform_details Pointer to the platform details structure.
 *  @param protocol_id Identifier of the protocol, the expected return values
 *  are asked for.
 *  @returns Expected return values for the protocol or NULL if the protocol
 *  identifier "protocol_id" is not found.
 *
 */
static const void *get_expected_return_values(
		const struct arm_scmi_platform_info *platform_details,
		unsigned int protocol_id)
{
	uint8_t index;
	struct arm_scmi_platform_protocol_info *protocol_info;

	for (index = 0; index < platform_details->platform_protocol_info_count;
			++index) {
		protocol_info = &platform_details->platform_protocol_info_table[index];
		if (protocol_info->protocol_id == protocol_id)
			return protocol_info->expected_return_values;
	}
	return NULL;
}

/*!
 * @brief Check if the SCMI test library supports a protocol given its protocol
 * identifier.
 *
 *  @param protocol_id Protocol identifier.
 *  @returns True if the protocol is supported false otherwise.
 *
 */
static bool is_protocol_supported(unsigned int protocol_id)
{
	uint8_t index = 0;

	/* Iterate through supported protocols */
	for (index = 0;
			index < arm_scmi_base_protocol_data.num_protocols_implemented;
			++index) {
		if (protocol_id == arm_scmi_base_protocol_data.protocols[index])
			return true;
	}
	return false;
}

/*!
 * @brief Executor function that runs the test suites/cases
 * for the implemented protocols.
 *
 *  The function iterates through the test suites and test
 *  cases defined for the protocols found by discovery in the
 *  base protocol.
 *
 *  @param platform_details Information passed to the protocols.
 *  @param run_protocols Pointer to the array of protocol ids to be run.
 *  @param test_report Pointer to test report.
 *  @returns 0 on no error, other value otherwise.
 *
 *
 */
int arm_scmi_agent_execute(
		const struct arm_scmi_platform_info *platform_details,
		const uint32_t *run_protocols,
		struct arm_scmi_test_report *test_report)
{
	const void *expected = NULL;
	bool protocol_enabled;
	uint32_t protocol_id = 0;
	uint32_t *protocol = NULL;

	arm_scmi_log_output("\n===SCMI TEST: START===\n");
	/* Search for Base Protocol expected return values */
	expected = get_expected_return_values(platform_details, BASE_PROTO_ID);
	test_report->protocol_report_count = 1;
	arm_scmi_base_execute(expected, test_report->protocol_report_table);
	if (run_protocols == NULL) {
		protocol = arm_scmi_base_protocol_data.protocols;
		arm_scmi_base_protocol_data.protocols[
			arm_scmi_base_protocol_data.num_protocols_implemented] = 0;
	} else {
		protocol = (uint32_t *)run_protocols;
	}

	while ((protocol_id = *protocol++) != 0) {
		protocol_enabled = (run_protocols == NULL) ? true:
			is_protocol_supported(protocol_id);
		if (!protocol_enabled)
			continue;
		expected = get_expected_return_values(platform_details, protocol_id);
#ifdef POWER_PROTOCOL
		if (arm_scmi_power_protocol_executor.protocol_id == protocol_id) {
			arm_scmi_power_protocol_executor.protocol_executor(expected,
				&test_report->protocol_report_table[
					test_report->protocol_report_count++]);
			continue;
		}
#endif
#ifdef SYSTEM_POWER_PROTOCOL
		if (arm_scmi_system_power_protocol_executor.protocol_id == protocol_id) {
			arm_scmi_system_power_protocol_executor.protocol_executor(expected,
				&test_report->protocol_report_table[
					test_report->protocol_report_count++]);
			continue;
		}
#endif
#ifdef PERFORMANCE_PROTOCOL
		if (arm_scmi_performance_protocol_executor.protocol_id == protocol_id) {
			arm_scmi_performance_protocol_executor.protocol_executor(expected,
				&test_report->protocol_report_table[
					test_report->protocol_report_count++]);
			continue;
		}
#endif
#ifdef CLOCK_PROTOCOL
		if (arm_scmi_clock_protocol_executor.protocol_id == protocol_id) {
			arm_scmi_clock_protocol_executor.protocol_executor(expected,
				&test_report->protocol_report_table[
					test_report->protocol_report_count++]);
			continue;
		}
#endif
#ifdef SENSOR_PROTOCOL
		if (arm_scmi_sensor_protocol_executor.protocol_id == protocol_id) {
			arm_scmi_sensor_protocol_executor.protocol_executor(expected,
				&test_report->protocol_report_table[
					test_report->protocol_report_count++]);
			continue;
		}
#endif
	}
	arm_scmi_log_output("\n===SCMI TEST: END===\n");
	return 0;
}


