/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <common.h>
#include <test_agent_exports.h>
#include <base_expected.h>
#ifdef POWER_PROTOCOL
#include <power_expected.h>
#endif
#ifdef SYSTEM_POWER_PROTOCOL
#include <system_power_expected.h>
#endif
#ifdef PERFORMANCE_PROTOCOL
#include <performance_expected.h>
#endif
#ifdef CLOCK_PROTOCOL
#include <clock_expected.h>
#endif
#ifdef SENSOR_PROTOCOL
#include <sensor_expected.h>
#endif

#define SCMI_BASE_PROTOCOL_ID 0x10
enum scmi_protocol_ids {
	SCMI_POWER_PROTOCOL_ID=0x11,
	SCMI_SYSTEM_POWER_PROTOCOL_ID=0x12,
	SCMI_PERF_PROTOCOL_ID=0x13,
	SCMI_CLOCK_PROTOCOL_ID=0x14,
	SCMI_SENSOR_PROTOCOL_ID=0x15
};

static struct arm_scmi_base_expected base_expected;
#ifdef POWER_PROTOCOL
static struct arm_scmi_power_expected power_expected;
#endif
#ifdef SYSTEM_POWER_PROTOCOL
static struct arm_scmi_system_power_expected system_power_expected;
#endif
#ifdef CLOCK_PROTOCOL
static struct arm_scmi_clock_expected clock_expected;
#endif
#ifdef SENSOR_PROTOCOL
static struct arm_scmi_sensor_expected sensor_expected;
#endif
static uint32_t supported_protocols[] =
{
	SCMI_POWER_PROTOCOL_ID,
	SCMI_SYSTEM_POWER_PROTOCOL_ID,
	SCMI_PERF_PROTOCOL_ID,
	SCMI_CLOCK_PROTOCOL_ID,
	SCMI_SENSOR_PROTOCOL_ID
};
static char *agent_names[] = {"agent"};


void fill_base_expected()
{
	base_expected.flags_mask = (1 << BASE_PROTOCOL_VERSION) +
			(1 << NUMBER_AGENTS) +
			(1 << NUM_PROTOCOLS_IMPLEMENTED) +
			(1 << VENDOR_IDENTIFIER) +
			(1 << SUBVENDOR_IDENTIFIER) +
			(1 << IMPLEMENTATION_VERSION) +
			(1 << SUPPORTED_PROTOCOLS) +
			(1 << AGENT_NAMES) +
			(1 << ERROR_NOTIFICATION_SUPPORTED) +
			(1 << DISCOVER_AGENT_CMD_SUPPORTED) +
			(1 << SUBVENDOR_CMD_SUPPORTED);
	base_expected.protocol_version = 0x10000;
	base_expected.num_agents = 1;
	base_expected.num_protocols_implemented = NUM_ELEMS(supported_protocols);
	sprintf(base_expected.vendor_identifier, "VENDOR 1");
	sprintf(base_expected.subvendor_identifier, "SUBVENDOR 1");
	base_expected.implementation_version = 0x1;
	base_expected.supported_protocols = supported_protocols;
	base_expected.agent_names = agent_names;
	base_expected.notify_error_cmd_supported = true;
	base_expected.subvendor_cmd_supported = true;
	base_expected.discover_agent_cmd_supported = true;
}

#ifdef POWER_PROTOCOL
void fill_power_expected()
{
	power_expected.flags_mask = (1 << POWER_PROTOCOL_VERSION) +
			(1 << NUM_POWER_DOMAINS) +
			(1 << STATISTICS_ADDRESS_LOW) +
			(1 << STATISTICS_ADDRESS_HIGH) +
			(1 << STATISTICS_LEN) +
			(1 << POWER_STATE_NOTIFY_CMD_SUPPORTED);
	power_expected.protocol_version = 0x10000;
	power_expected.num_power_domains = 3;
	power_expected.statistics_address_low = 0x1234;
	power_expected.statistics_length = 0xff;
	power_expected.statistics_address_high =
		power_expected.statistics_address_low + power_expected.statistics_length;
	power_expected.power_state_notify_cmd_supported = true;
}
#endif

int main()
{
	struct arm_scmi_platform_info platform_details;
	struct arm_scmi_test_report test_report;

	fill_base_expected();
	struct arm_scmi_platform_protocol_info protocols[] =
		{
			{
				.protocol_id = SCMI_BASE_PROTOCOL_ID,
				.expected_return_values = (void *)&base_expected
			},
#ifdef POWER_PROTOCOL
			{
				.protocol_id = SCMI_POWER_PROTOCOL_ID,
				.expected_return_values = (void *)&power_expected
			},
#endif
#ifdef SYSTEM_POWER_PROTOCOL
			{
				.protocol_id = SCMI_SYSTEM_POWER_PROTOCOL_ID,
				.expected_return_values = (void *)&system_power_expected
			},
#endif
#ifdef PERFORMANCE_PROTOCOL
			{
				.protocol_id = SCMI_PERF_PROTOCOL_ID,
				.expected_return_values = NULL
			},
#endif
#ifdef CLOCK_PROTOCOL
			{
				.protocol_id = SCMI_CLOCK_PROTOCOL_ID,
				.expected_return_values = NULL
			},
#endif
#ifdef SENSOR_PROTOCOL
			{
				.protocol_id = SCMI_SENSOR_PROTOCOL_ID,
				.expected_return_values = NULL
			},
#endif
		};

	platform_details.platform_protocol_info_table = protocols;
	platform_details.platform_protocol_info_count =
		sizeof(protocols) / sizeof(protocols[0]);
	arm_scmi_agent_execute(&platform_details, NULL, &test_report);
	return NO_ERROR;
}

