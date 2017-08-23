/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <protocol_common.h>
#include <sensor_common.h>
#include <sensor_expected.h>

/* Variable to hold expected data */
static struct arm_scmi_sensor_expected *sensor_expected =
		&(struct arm_scmi_sensor_expected){};
/* Variable to hold the expected flags mask */
static uint32_t expected_flags_mask;

struct sensor_protocol_data {
	uint32_t num_sensors;
	uint32_t max_num_pending_async_cmds_supported;
	/*If length of shared memory is zero, it's not supported. Vice versa*/
	uint32_t sensor_reg_len;
	uint32_t snsr_trip_points_supported[MAX_NUMBER_SENSOR];
	uint32_t sensor_id[MAX_NUMBER_SENSOR];
};

/* Global variable to hold run-time sensor protocol data */
static struct sensor_protocol_data sensor_protocol_data;

/*!
 * @brief Function used to fill up expected test case return data from
 * expected protocol return data.
 *
 *  The function is called by the test engine just before sending
 *  the command in the test case and testing the received data. It also
 *  sets other hooks depending on the message id. To see the expected_returns
 *  index meaning see 'sensor_tests.c' for each specific command.
 *
 *  @param test case Pointer to the test case being executed in the test engine.
 *
 */
static void sensor_fill_expected_return_values(arm_scmi_test_case_t *test_case)
{
	return_value_desc_t *expected_ret;

	switch (test_case->message_id)
	{
		case SNSR_PROTO_VER_MSG_ID:
			expected_ret = &test_case->expected_returns[0];
			expected_ret->value.integer = sensor_expected->protocol_version;
			expected_ret->flags = arm_scmi_print_or_check(
				SENSOR_PROTOCOL_VERSION, expected_flags_mask);
			break;
		case SNSR_PROTO_ATTRIB_MSG_ID:
			expected_ret = &test_case->expected_returns[1];
			expected_ret->value.integer =
				sensor_expected->max_num_pending_async_cmds_supported;
			expected_ret->flags = arm_scmi_print_or_check(
				MAX_NUM_PENDING_ASYNC_CMDS_SUPPORTED, expected_flags_mask);
			expected_ret = &test_case->expected_returns[2];
			expected_ret->value.integer = sensor_expected->number_sensors;
			expected_ret->flags = arm_scmi_print_or_check(NUMBER_SENSORS,
				expected_flags_mask);
			break;
	}
}

/*!
 * @brief Setup function for sensor management tests execution.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param expected Pointer to the expected values for the performance protocol.
 *
 */
static void setup_sensor(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		struct arm_scmi_sensor_expected *expected)
{
	uint8_t index;
	struct arm_scmi_test_suite_report *tsr =
			arm_scmi_sensor_protocol_report.test_suite_report_table;

	/* If no expected values are provided (i.e. NULL) it is assigned an
	 * empty compound literal in order to avoid if != NULL in every
	 * base_expected value.
	 */
	if (expected == NULL) {
		expected_flags_mask = 0;
	} else {
		sensor_expected = expected;
		expected_flags_mask = expected->flags_mask;
	};
	arm_scmi_init_protocol_execution_context(protocol_execution_context,
		sensor_fill_expected_return_values,
		&arm_scmi_sensor_protocol_report);
	arm_scmi_set_test_suite_table(protocol_execution_context,
		arm_scmi_sensor_test_suites,
		arm_scmi_sensor_test_suites_size);
	for (index = 0; index < arm_scmi_sensor_test_suites_size; ++index)
		arm_scmi_sensor_test_suites[index].name = (tsr + index)->test_suite_name;
	arm_scmi_skip_test_suite(protocol_execution_context, TS_SENSOR_DISCOVERY,
		false);
	arm_scmi_log_protocol_name(&protocol_execution_context->test_stats, "Sensor");
}

/*!
 * @brief Function to fill protocol attributes.
 *
 *  The function is called by the test engine executor after testing the protocol
 *  attributes and fill the returned or expected data for global values of
 *  number of sensors and max number of pending asynchronous commands supported.
 *
 *  @param protocol_exe_handler Parameter by reference to the protocol handler.
 *  @param testcase Parameter by reference to the current test case.
 *  @param received Parameter by reference to platform response.
 *
 */
void arm_scmi_post_processing_sensor_protocol_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case,
		struct arm_scmi_message *received)
{
	uint32_t attributes;
	uint32_t sensor_register_len;
	struct arm_scmi_protocol_test_stats *test_stats =
		&protocol_execution_context->test_stats;

	attributes = received->payload[OFFSET_RET(
		struct arm_scmi_sensor_protocol_attributes, attributes)];
	sensor_register_len = received->payload[OFFSET_RET(
		struct arm_scmi_sensor_protocol_attributes, sensor_reg_len)];

	if (received->status == SCMI_STATUS_SUCCESS) {
		sensor_protocol_data.num_sensors = arm_scmi_get_norm_bits(attributes,
				SNR_NUMBER_SENSORS_HIGH, SNR_NUMBER_SENSORS_LOW);
		sensor_protocol_data.max_num_pending_async_cmds_supported =
			arm_scmi_get_norm_bits(attributes,
				SNR_MAX_NUM_PENDING_ASYNC_CMDS_SPRT_HIGH,
				SNR_MAX_NUM_PENDING_ASYNC_CMDS_SPRT_LOW);
		sensor_protocol_data.sensor_reg_len = sensor_register_len;
		arm_scmi_check_and_report_dec(test_stats, expected_flags_mask,
			LEFT_SHIFT(SENSOR_REG_LENGTH), sensor_expected->sensor_reg_length,
			received->payload[OFFSET_RET(
			struct arm_scmi_sensor_protocol_attributes, sensor_reg_len)],
			"SENSOR REGISTER ADDRESS LEN");
		if (sensor_register_len > 0) {
			arm_scmi_check_and_report_hex(test_stats, expected_flags_mask,
				LEFT_SHIFT(SENSOR_REG_ADDRESS_LOW),
				sensor_expected->sensor_reg_address_low,
				received->payload[OFFSET_RET(
				struct arm_scmi_sensor_protocol_attributes,
				sensor_reg_address_low)], "SENSOR REGISTER ADDRESS LOW");
			arm_scmi_check_and_report_hex(test_stats, expected_flags_mask,
				LEFT_SHIFT(SENSOR_REG_ADDRESS_HIGH),
				sensor_expected->sensor_reg_address_high, received->payload[
				OFFSET_RET(struct arm_scmi_sensor_protocol_attributes,
				sensor_reg_address_high)], "SENSOR REGISTER ADDRESS LOW");
		}
	}
}

/*!
 * @brief Function used to check if a command is supported.
 *
 *  The function is called by the test engine executor after testing the
 *  PROTOCOL_MESSAGE_ATTRIBUTES message id in order to set a test suite
 *  if the command is supported
 *
 *  @param testcase Parameter by reference to the test case being executed in
 *  the test engine executor.
 *  @param expected Parameter by reference where the protocol expected returned
 *  data is stored.
 *  @param received Parameter by reference to the received message structure.
 *
 */
void arm_scmi_post_processing_sensor_protocol_message_attributes(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case, struct arm_scmi_message *received)
{
	uint32_t mask;
	int index = -1;
	bool must_be_supported = true;
	bool must_test_suite = true;
	uint32_t message_id = test_case->parameters[OFFSET_PARAM(
		struct arm_scmi_sensor_protocol_message_attributes, message_id)];

	switch (message_id)
	{
		case SNSR_DESC_GET_MSG_ID:
			mask = ALL_BITS_SET;
			index = TS_SENSOR_DESC_GET;
			break;
		case SNSR_TRIP_POINT_NOTIFY_ID:
			mask= LEFT_SHIFT(SENSOR_TRIP_POINT_NOTIFY_SUPPORTED);
			index = TS_SENSOR_TRIP_POINT_NOTIFY;
			must_be_supported =
				sensor_expected->sensor_trip_point_notify_supported;
			break;
		case SNSR_TRIP_POINT_CONFIG_ID:
			mask= LEFT_SHIFT(SENSOR_TRIP_POINT_CONFIG_SUPPORTED);
			index = TS_SENSOR_TRIP_POINT_CONFIG;
			must_be_supported =
				sensor_expected->sensor_trip_point_config_supported;
			break;
		case SNSR_READING_GET_MSG_ID:
			mask = ALL_BITS_SET;
			index = TS_SENSOR_READING_GET;
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
 * @brief Function to test the sensor reading get command.
 *
 *  The function is called by the test engine before sending the
 *	sensor reading get command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_tester_sensor_reading_get(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case) {

	uint32_t idx;
	struct arm_scmi_protocol_test_stats *test_stats =
		&protocol_execution_context->test_stats;
	int error_code;

	/* For all test cases allocate space for parameters */
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	test_case->parameter_count = 2;
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
		struct arm_scmi_sensor_reading_get, sensor_id)) {
		/* Negative test case*/
		/* Here we are sending the command to an non-existent sensor id, setting
		 * flag to be synchronous*/
		test_case->parameters[OFFSET_PARAM(struct arm_scmi_sensor_reading_get,
			sensor_id)] = sensor_protocol_data.num_sensors;
		test_case->parameters[OFFSET_PARAM(struct arm_scmi_sensor_reading_get,
			flags)] = (RESERVED << SNR_READING_FLAG_RESERVED_LOW) |
					(SENSOR_SYNC << SNR_READING_FLAG_SYNC_ASYNC);
		arm_scmi_execute_and_assert_test_case(protocol_execution_context,
			test_case, &received);
	} else {
		/* Positive test case - SYNC reading*/
		/*Loop through all sensor ids*/
		for (idx = 0; idx < sensor_protocol_data.num_sensors; ++idx) {
			snprintf(description, NUM_ELEMS(description), "\n\n\t\t\tSENSOR ID %d",
				sensor_protocol_data.sensor_id[idx]);
			arm_scmi_log(test_stats, description);
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_sensor_reading_get, sensor_id)] =
				sensor_protocol_data.sensor_id[idx];
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_sensor_reading_get, flags)] =
				(RESERVED << SNR_READING_FLAG_RESERVED_LOW) |
				(SENSOR_SYNC << SNR_READING_FLAG_SYNC_ASYNC);
			error_code = arm_scmi_execute_and_assert_test_case(
					protocol_execution_context, test_case, &received);
			if (arm_scmi_skip_return_values(error_code, received.status))
						continue;
			/* Print sensor value low */
			arm_scmi_check_and_report_hex(test_stats, ALL_BITS_SET, PRINT, 0,
				RETURN_VAL(received, struct arm_scmi_sensor_reading_get,
				sensor_value_low), "SENSOR VALUE LOW");
			/* Print sensor value high */
			arm_scmi_check_and_report_hex(test_stats, ALL_BITS_SET, PRINT, 0,
				RETURN_VAL(received, struct arm_scmi_sensor_reading_get,
				sensor_value_high), "SENSOR VALUE HIGH");
		}
	}
}

/*!
 * @brief Function to test the clock describe rates command.
 *
 *  The function is called by the test engine before sending the
 *	sensor trip point config command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_tester_sensor_trip_point_config(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case) {

	uint32_t idx;
	struct arm_scmi_protocol_test_stats *test_stats =
		&protocol_execution_context->test_stats;
	int trip_idx;

	/* For all test cases allocate space for parameters */
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	test_case->parameter_count = 4;
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
			struct arm_scmi_sensor_trip_point_config, sensor_id)) {
		/* Negative test case*/
		/* Here we are sending the command to an non-existent sensor id*/
		/* Using valid values for the other three parameters*/
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_sensor_trip_point_config, sensor_id)] =
			sensor_protocol_data.sensor_id[sensor_protocol_data.num_sensors - 1]
			+ 1;
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_sensor_trip_point_config, trip_point_ev_ctrl)] =
			sensor_protocol_data.snsr_trip_points_supported[0];
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_sensor_trip_point_config, trip_point_val_low)] = 0;
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_sensor_trip_point_config, trip_point_val_high)] = 0;
		arm_scmi_execute_and_assert_test_case(protocol_execution_context,
			test_case, &received);
	} else if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
			struct arm_scmi_sensor_trip_point_config, trip_point_ev_ctrl)) {
		/* Negative test case*/
		/* Here we are using the first sensor and sending a non-existent trip
		 * point id to that sensor*/
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_sensor_trip_point_config, sensor_id)] =
			sensor_protocol_data.sensor_id[0];
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_sensor_trip_point_config, trip_point_ev_ctrl)] =
			(sensor_protocol_data.snsr_trip_points_supported[
			test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_sensor_trip_point_config, sensor_id)]] + 1) <<
			SNR_TRIP_POINT_ID_LOW;
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_sensor_trip_point_config, trip_point_val_low)] = 0;
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_sensor_trip_point_config, trip_point_val_high)] = 0;
		arm_scmi_execute_and_assert_test_case(protocol_execution_context,
			test_case, &received);
	} else {
		/*positive test case*/
		/*loop through all sensor ids*/
		for (idx = 0; idx < sensor_protocol_data.num_sensors; ++idx) {
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_sensor_trip_point_config, sensor_id)] =
				sensor_protocol_data.sensor_id[idx];
			for (trip_idx = 0; trip_idx <
			sensor_protocol_data.snsr_trip_points_supported[idx]; trip_idx++) {
				/* Assuming trip ids are sequential and start from 0 */
				snprintf(description, NUM_ELEMS(description),
				"\n\t\t\tSENSOR ID %d, TRIP ID %d",
				sensor_protocol_data.sensor_id[idx], trip_idx);
				arm_scmi_log(test_stats, description);
				test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_sensor_trip_point_config, trip_point_ev_ctrl)] =
					trip_idx << SNR_TRIP_POINT_ID_LOW;
				test_case->parameters[OFFSET_PARAM(
					struct arm_scmi_sensor_trip_point_config,
					trip_point_val_low)] = 0;
				test_case->parameters[OFFSET_PARAM(
					struct arm_scmi_sensor_trip_point_config,
					trip_point_val_high)] = 0;
				arm_scmi_execute_and_assert_test_case(protocol_execution_context,
					test_case, &received);
			}
		}
	}
}

/*!
 * @brief Function to test the clock describe rates command.
 *
 *  The function is called by the test engine before sending the
 *	sensor trip point notify command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_tester_sensor_trip_point_notify(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case) {

	uint32_t idx;
	uint8_t notification_enable = 0;

	/* For all test cases allocate space for parameters */
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	test_case->parameter_count = 2;
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
		struct arm_scmi_sensor_trip_point_notify, sensor_id)) {
		/* Negative test case*/
		/* Here we are sending "notify disable" signal to an non-existent
		 * sensor id*/
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_sensor_trip_point_notify, sensor_id)] =
			sensor_protocol_data.sensor_id[sensor_protocol_data.num_sensors - 1]
			+ 1;
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_sensor_trip_point_notify, sensor_event_control)] =
			sensor_protocol_data.num_sensors;
		arm_scmi_execute_and_assert_test_case(protocol_execution_context,
			test_case, &received);
	} else {
		/* Positive test case */
		/*Loop through all sensor ids, here we are sending signals
		 * to disable trip point notifications on all sensors*/
		for (idx = 0; idx < sensor_protocol_data.num_sensors; ++idx) {
			snprintf(description, NUM_ELEMS(description),
			"\n\t\t\tSENSOR ID %d", sensor_protocol_data.sensor_id[idx]);
			arm_scmi_log(&protocol_execution_context->test_stats, description);
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_sensor_trip_point_config, sensor_id)] =
				sensor_protocol_data.sensor_id[idx];
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_sensor_trip_point_notify, sensor_event_control)]
				= (0 << 1) + notification_enable;
			arm_scmi_execute_and_assert_test_case(protocol_execution_context,
				test_case, &received);
		}
	}
}

/*!
 * @brief Function to test the sensor description get command.
 *
 *  The function is called by the test engine before sending the
 *	sensor description get command and testing the received values.
 *
 *  @param protocol_execution_context Pointer to the protocol execution context.
 *  @param test_case Pointer to the current test case.
 *
 */
void arm_scmi_custom_tester_sensor_description_get(
		struct arm_scmi_protocol_execution_context *protocol_execution_context,
		arm_scmi_test_case_t *test_case) {

	uint32_t start_desc_index;
	uint32_t num_sensor_flags;
	uint32_t remaining_sensor_desc;
	uint32_t sensor_desc_returned;
	uint32_t rcv_u, exp_u;
	uint32_t check_or_print;
	int total_sensor_desc;
	uint32_t offset;
	char *exp_s;
	struct arm_scmi_sensor_descriptor *sensor_desc;
	uint32_t sensor_idx, current_idx;
	struct arm_scmi_protocol_test_stats *test_stats =
		&protocol_execution_context->test_stats;
	char *sensor_name; // for re-casting the sensor name in the descriptor
	struct arm_scmi_sensor_expected *se = sensor_expected;
	enum test_status res;
	int error_code;

	/* For all test cases allocate space for parameters */
	test_case->parameters = (uint32_t[MAX_PARAMETERS_SIZE]) {};
	test_case->parameter_count = 1;
	if (test_case->invalid_parameter == OFFSET_INVALID_PARAM(
		struct arm_scmi_sensor_description_get, desc_index)) {
		/* Negative test case*/
		test_case->parameters[OFFSET_PARAM(
			struct arm_scmi_sensor_description_get, desc_index)] =
			sensor_protocol_data.num_sensors;
		arm_scmi_execute_and_assert_test_case(protocol_execution_context,
			test_case, &received);
	}
	else {
		/* Positive test case */
		start_desc_index = 0;
		total_sensor_desc = 0;
		do {
			test_case->parameters[OFFSET_PARAM(
				struct arm_scmi_sensor_description_get, desc_index)] =
				start_desc_index;
			/* Check if command was successful */
			error_code = arm_scmi_execute_and_assert_test_case(
					protocol_execution_context, test_case, &received);
			if (arm_scmi_skip_return_values(error_code, received.status))
						continue;
			/* Get number of sensors returned and remaining */
			num_sensor_flags = RETURN_VAL(received,
				struct arm_scmi_sensor_description_get, num_sensor_flags);
			/* Print number of remaining sensor descriptors */
			remaining_sensor_desc = arm_scmi_get_norm_bits(num_sensor_flags,
				SNR_NUMBER_REMAIN_SNR_DESC_HIGH, SNR_NUMBER_REMAIN_SNR_DESC_LOW);
			arm_scmi_check_and_report_dec(test_stats, ALL_BITS_SET, PRINT, 0,
				remaining_sensor_desc, "NUMBER OF REMAINING SENSOR DESCRIPTORS");
			/* Check Reserved bits */
			rcv_u = arm_scmi_get_norm_bits(num_sensor_flags,
				SNR_NUM_SNR_FLAG_RESV_HIGH, SNR_NUM_SNR_FLAG_RESV_LOW);
			arm_scmi_check_and_report_dec(test_stats, ALL_BITS_SET, CHECK,
				RESERVED, rcv_u, "RESERVED BITS");
			/* Check/Print number of sensor descriptors */
			sensor_desc_returned = arm_scmi_get_norm_bits(num_sensor_flags,
				SNR_NUMBER_SNR_DESC_HIGH, SNR_NUMBER_SNR_DESC_LOW);
			check_or_print = (remaining_sensor_desc == 0) ? CHECK: PRINT;
			arm_scmi_check_and_report_dec(test_stats, expected_flags_mask,
				check_or_print, sensor_expected->number_sensors,
				sensor_desc_returned,
				"NUMBER OF SENSOR DESCRIPTORS");
			offset = OFFSET_BYTES_RET(struct arm_scmi_sensor_description_get,
				sensor_descriptors) + offsetof(struct arm_scmi_sensor_descriptor,
				sensor_id);
			sensor_desc = (struct arm_scmi_sensor_descriptor *)
				((uint8_t *)received.payload + offset);
			/*parse each descriptor*/
			for (sensor_idx = 0; sensor_idx < sensor_desc_returned;
					++sensor_idx) {
				/* It is assumed that the sensor id is equal to the sensor index
				 * descriptor */
				current_idx = total_sensor_desc + sensor_idx;
				/* Print sensor id */
				check_or_print = (sensor_desc->sensor_id == current_idx) ?
					CHECK:PRINT;
				arm_scmi_check_and_report_dec(test_stats, ALL_BITS_SET, PRINT, 0,
					sensor_desc->sensor_id, "SENSOR ID");
				sensor_protocol_data.sensor_id[current_idx] =
					sensor_desc->sensor_id;
				/* Check/print Asynchronous sensor read support */
				res = INFO;
				rcv_u = arm_scmi_get_norm_bits(
					sensor_desc->sensor_attributes_low,
					SNR_DESC_ATTRLOW_ASYNC_READ_SUPPORT,
					SNR_DESC_ATTRLOW_ASYNC_READ_SUPPORT);
				snprintf(name, NUM_ELEMS(name), "ASYNCHRONOUS SENSOR READ SUPPORT");
				if (check_or_print && (current_idx < se->number_sensors) &&
					(se->asynchronous_sensor_read_support != NULL) &&
					(expected_flags_mask &
					LEFT_SHIFT(SENSOR_ASYNCHRONOUS_READ_SUPPORT))) {
					exp_u = se->asynchronous_sensor_read_support[current_idx];
					res = (exp_u == rcv_u) ? PASSED:FAILED;
					snprintf(description, NUM_ELEMS(description),
						"Expected: %d, Received: %d", exp_u, rcv_u);
				}
				if ((res == PASSED) || (res == INFO)){
					snprintf(description, NUM_ELEMS(description), "Received: %d",
						rcv_u);
				}
				arm_scmi_log_test_result(test_stats, res, name, description);
				/* Check/print Number of trip points supported */
				res = INFO;
				rcv_u = arm_scmi_get_norm_bits(
					sensor_desc->sensor_attributes_low,
					SNR_DESC_ATTRLOW_NUM_TRIPPOINT_SPRT_HIGH,
					SNR_DESC_ATTRLOW_NUM_TRIPPOINT_SPRT_LOW);
				sensor_protocol_data.snsr_trip_points_supported[current_idx] =
					rcv_u;
				snprintf(name, NUM_ELEMS(name), "NUMBER OF TRIP POINTS SUPPORTED");
				if (check_or_print && (current_idx < se->number_sensors) &&
					(se->number_of_trip_points_supported != NULL) &&
					(expected_flags_mask &
					LEFT_SHIFT(SENSOR_NUMBER_OF_TRIP_POINTS_SUPPORTED))) {
					exp_u = se->number_of_trip_points_supported[current_idx];
					res = (exp_u == rcv_u) ? PASSED:FAILED;
					snprintf(description, NUM_ELEMS(description),
						"Expected: %d, Received: %d", exp_u, rcv_u);
				}
				if ((res == PASSED) || (res == INFO)){
					snprintf(description, NUM_ELEMS(description), "Received: %d",
						rcv_u);
				}
				arm_scmi_log_test_result(test_stats, res, name, description);
				/* Print sensor update interval/seconds */
				rcv_u = arm_scmi_get_norm_bits(
					sensor_desc->sensor_attributes_high,
					SNR_DESC_ATTRHIGH_UPDATE_INTERVAL_SEC_HIGH,
					SNR_DESC_ATTRHIGH_UPDATE_INTERVAL_SEC_LOW);
				arm_scmi_check_and_report_dec(test_stats, ALL_BITS_SET, PRINT, 0,
					rcv_u, "SENSOR UPDATE INTERVAL/sec");
				/* Print sensor update interval/mult twos */
				rcv_u = arm_scmi_get_norm_bits(
					sensor_desc->sensor_attributes_high,
					SNR_DESC_ATTRHIGH_UPDATE_INTERVAL_MULT_HIGH,
					SNR_DESC_ATTRHIGH_UPDATE_INTERVAL_MULT_LOW);
				arm_scmi_check_and_report_dec(test_stats, ALL_BITS_SET, PRINT, 0,
					rcv_u, "SENSOR UPDATE INTERVAL/mult");
				/* Print multiplier power of 10 */
				rcv_u = arm_scmi_get_norm_bits(
					sensor_desc->sensor_attributes_high,
					SNR_DESC_ATTRHIGH_MULTIPLIER_HIGH,
					SNR_DESC_ATTRHIGH_MULTIPLIER_LOW);
				arm_scmi_check_and_report_dec(test_stats, ALL_BITS_SET, PRINT, 0,
					rcv_u, "SENSOR TYPE UNIT MULTIPLIER");
				/* Check/print SensorType */
				res = INFO;
				rcv_u = arm_scmi_get_norm_bits(
					sensor_desc->sensor_attributes_high,
					SNR_DESC_ATTRHIGH_SNR_TYPE_HIGH,
					SNR_DESC_ATTRHIGH_SNR_TYPE_LOW);
				snprintf(name, NUM_ELEMS(name), "SENSOR TYPE");
				if (check_or_print && (current_idx < se->number_sensors) &&
					(se->sensor_type != NULL) &&
					(expected_flags_mask & LEFT_SHIFT(SENSOR_TYPE))) {
					exp_u = se->sensor_type[current_idx];
					res = (exp_u == rcv_u) ? PASSED:FAILED;
					snprintf(description, NUM_ELEMS(description),
						"Expected: %d, Received: %d", exp_u, rcv_u);
				}
				if ((res == PASSED) || (res == INFO)){
					snprintf(description, NUM_ELEMS(description), "%d",
						rcv_u);
				}
				arm_scmi_log_test_result(test_stats, res, name, description);
				/* Check/print Sensor name */
				res = INFO;
				sensor_name = (char *)sensor_desc->sensor_name;
				snprintf(name, NUM_ELEMS(name), "SENSOR NAME");
				if (check_or_print && (current_idx < se->number_sensors) &&
					(se->sensor_name != NULL) &&
					(expected_flags_mask & LEFT_SHIFT(SENSOR_TYPE))) {
					exp_s = se->sensor_name[current_idx];
					res = sensor_name == exp_s;
					snprintf(description, NUM_ELEMS(description),
						"Expected: '%s', Received: '%s'", exp_s, sensor_name);
				}
				if ((res == PASSED) || (res == INFO)){
					snprintf(description, NUM_ELEMS(description), "'%s'",
						sensor_name);
				}
				arm_scmi_log_test_result(test_stats, res, name, description);
				/* Move to next sensor descriptor */
				sensor_desc = (struct arm_scmi_sensor_descriptor *)
				((uint8_t *)sensor_desc->sensor_name + sizeof(
				sensor_desc->sensor_name));
				arm_scmi_log(&protocol_execution_context->test_stats, "\n");
			}
			total_sensor_desc += sensor_desc_returned;
			start_desc_index += sensor_desc_returned;
		} while (remaining_sensor_desc > 0);
	}
}

/*!
 * @brief Executor function that runs the test suites/cases for the sensor
 * management protocol.
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
void arm_scmi_sensor_execute(const void *expected,
		struct arm_scmi_protocol_report **protocol_report)
{
	static struct arm_scmi_protocol_execution_context protocol_execution_context;

	*protocol_report = &arm_scmi_sensor_protocol_report;
	setup_sensor(&protocol_execution_context,
			(struct arm_scmi_sensor_expected *) expected);
	arm_scmi_run(&protocol_execution_context, &received);
}
