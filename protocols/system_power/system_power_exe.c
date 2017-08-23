/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <protocol_common.h>
#include <test_execute.h>
#include <system_power_common.h>
#include <system_power_exports.h>
#include <system_power_expected.h>


/* Variable to hold global data for system power management protocol */
struct arm_scmi_system_global_data arm_scmi_system_power_protocol_data;
/* Variable to hold expected data */
static struct arm_scmi_system_power_expected *system_expected =
		&(struct arm_scmi_system_power_expected){};
/* Variable to hold the expected flags mask */
static uint32_t expected_flags_mask;

/*!
 * @brief Function used to fill up expected test case return data from
 * expected protocol return data.
 *
 *  The function is called by the test engine just before sending
 *  the command in the test case and testing the received data. It also
 *  sets other hooks depending on the message id.
 *
 *  @param test case Pointer to the test case being executed in the test engine.
 *
 */
static void system_power_fill_expected_return_values(arm_scmi_test_case_t *test_case)
{
	return_value_desc_t *expected_ret;

	switch (test_case->message_id)
	{
		case SYSTEM_POWER_PROTO_VER_MSG_ID:
			expected_ret = &test_case->expected_returns[
				OFFSET_RET(struct arm_scmi_system_power_protocol_version,
				version)];
			expected_ret->value.integer = system_expected->protocol_version;
			expected_ret->flags = arm_scmi_print_or_check(
				SYSTEM_POWER_PROTOCOL_VERSION, expected_flags_mask);
			break;
		case SYSTEM_POWER_PROTO_ATTRIB_MSG_ID:
			expected_ret = &test_case->expected_returns[OFFSET_RET(
				struct arm_scmi_system_power_protocol_message_attributes,
				attributes)];
			expected_ret->flags = CHECK;
			break;
		case SYSTEM_POWER_PROTO_MSG_ATTRIB_MSG_ID:
			/* Custom tester */
			break;
		case SYSTEM_POWER_STATE_SET_MSG_ID:
			/* Custom tester */
			break;
	}
}

/*!
 * @brief Setup function for system power management tests execution.
 *
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param expected Pointer to the expected values for the system power protocol.
 *
 */
static void setup_system_power(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		struct arm_scmi_system_power_expected *expected)
{
	uint8_t index;
	struct arm_scmi_test_suite_report *tsr =
		arm_scmi_system_power_protocol_report.test_suite_report_table;

	/* If no expected values are provided (i.e. NULL) it is assigned an
	 * empty compound literal in order to avoid if != NULL in every
	 * base_expected value.
	 */
	if (expected == NULL) {
		expected_flags_mask = 0;
	} else {
		system_expected = expected;
		expected_flags_mask = expected->flags_mask;
	};
	arm_scmi_init_protocol_execution_context(protocol_execution_context,
		system_power_fill_expected_return_values,
		&arm_scmi_system_power_protocol_report);
	arm_scmi_set_test_suite_table(protocol_execution_context,
		arm_scmi_system_power_test_suites,
		arm_scmi_system_power_test_suites_size);
	for (index = 0; index < arm_scmi_system_power_test_suites_size; ++index)
		arm_scmi_system_power_test_suites[index].name =
		(tsr + index)->test_suite_name;
	arm_scmi_skip_test_suite(protocol_execution_context, TS_SYSTEM_DISCOVERY,
		false);
	arm_scmi_log_protocol_name(&protocol_execution_context->test_stats,
		"System Power");
}

/*!
 * @brief Function used to check if a command is supported.
 *
 *  The function is called by the test engine after testing the
 *  PROTOCOL_MESSAGE_ATTRIBUTES message id in order to check if a test suite
 *  must be skipped.
 *  And let test engine know that if platform supports warm reset or system
 *  suspend for SYSTEM_POWER_STATE_SET.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the test case being executed in the test engine.
 *  @param received Pointer to the received message structure.
 *
 */
void arm_scmi_post_processing_system_power_protocol_message_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case, struct arm_scmi_message *received)
{
	uint32_t mask;
	bool must_be_supported = true;
	bool must_test_suite;
	int index = -1;
	uint32_t attributes;
	struct arm_scmi_message r;
	uint32_t message_id = test_case->parameters[OFFSET_PARAM(
		struct arm_scmi_system_power_protocol_message_attributes, message_id)];
	uint32_t rcv_u;

	/*check skip value*/
	switch (message_id)
	{
		case SYSTEM_POWER_STATE_GET_MSG_ID:
			mask =  ALL_BITS_SET;
			index = TS_SYSTEM_POWER_STATE_GET;
			break;
		case SYSTEM_POWER_STATE_NOTIFY_MSG_ID:
			mask = LEFT_SHIFT(SYSTEM_POWER_STATE_NOTIFY_CMD_SUPPORTED);
			/* No test suite defined yet until notification support */
			index = -1;
			must_be_supported = false;
			break;
		case SYSTEM_POWER_STATE_SET_MSG_ID:
			r = *received;
			attributes = RETURN_VAL(r,
			struct arm_scmi_system_power_protocol_message_attributes, attributes);
			/* Print system warm reset support */
			rcv_u = arm_scmi_get_norm_bits(attributes,
				SYSTEM_POWER_MSG_PROTO_ATTRIB_WARMREST,
				SYSTEM_POWER_MSG_PROTO_ATTRIB_WARMREST);
			arm_scmi_check_and_report_dec(&protocol_execution_context->test_stats,
				ALL_BITS_SET, PRINT, 0, rcv_u, "SYSTEM WARM RESET SUPPORT");
			/* Print system suspend support */
			rcv_u = arm_scmi_get_norm_bits(attributes,
					SYSTEM_POWER_MSG_PROTO_ATTRIB_SUSPEND,
					SYSTEM_POWER_MSG_PROTO_ATTRIB_SUSPEND);
			arm_scmi_check_and_report_dec(&protocol_execution_context->test_stats,
				ALL_BITS_SET, PRINT, 0, rcv_u, "SYSTEM SUSPEND SUPPORT");
			/* Check reserved bits */
			rcv_u = arm_scmi_get_norm_bits(attributes,
					SYSTEM_POWER_MSG_PROTO_ATTRIB_RESERVED_HIGH,
					SYSTEM_POWER_MSG_PROTO_ATTRIB_RESERVED_LOW);
			arm_scmi_check_and_report_dec(&protocol_execution_context->test_stats,
				ALL_BITS_SET, CHECK, RESERVED, rcv_u, "RESERVED");
			break;

	}
	must_test_suite = arm_scmi_check_suite_skipped(
			&protocol_execution_context->test_stats, mask,
			expected_flags_mask, must_be_supported, message_id,
			received->status);
	if (index > 0)
		arm_scmi_skip_test_suite(protocol_execution_context, index,
		!must_test_suite);
}

/*!
 * @brief Executor function that runs the test suites/cases for the system
 * power management protocol.
 *
 *  @details The function iterates through the test suites and test
 *  cases defined for the base protocol (in the header file)
 *  and fills the global variable with information about
 *  protocols and agents discovered from the platform.
 *
 *  @param expected Pointer to the expected return values structure.
 *  @param protocol_report Pointer to a table of protocol pointers.
 *
 */
void arm_scmi_system_power_execute(const void *expected,
		struct arm_scmi_protocol_report **protocol_report)
{
	static struct arm_scmi_protocol_execution_context protocol_execution_context;

	*protocol_report = &arm_scmi_system_power_protocol_report;
	setup_system_power(&protocol_execution_context,
			(struct arm_scmi_system_power_expected *) expected);
	arm_scmi_run(&protocol_execution_context, &received);

}
