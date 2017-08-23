/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef SENSOR_EXPECTED_H_
#define SENSOR_EXPECTED_H_

struct arm_scmi_sensor_expected {
	/*
	 * This enum is to specify the flag_mask and have to be ensured that
	 *  it is in the same order as the below structure elements after
	 *  flags_mask member.
	 */
	enum {
		SENSOR_PROTOCOL_VERSION = 1,
		NUMBER_SENSORS,
		MAX_NUM_PENDING_ASYNC_CMDS_SUPPORTED,
		SENSOR_REG_ADDRESS_LOW,
		SENSOR_REG_ADDRESS_HIGH,
		SENSOR_REG_LENGTH,
		SENSOR_ASYNCHRONOUS_READ_SUPPORT,
		SENSOR_NUMBER_OF_TRIP_POINTS_SUPPORTED,
		SENSOR_TYPE,
		SENSOR_TRIP_POINT_NOTIFY_SUPPORTED,
		SENSOR_TRIP_POINT_CONFIG_SUPPORTED
	} FLAGS;
	/*
	 * The expectation is that there is no more
	 * than 32 elements specified via this expected results data structure.
	 * This mask is used to indicate which of the structure members hold valid
	 * data.
	 */
	uint32_t flags_mask;
	uint32_t protocol_version;

	uint32_t number_sensors;
	uint32_t max_num_pending_async_cmds_supported;

	/*Lower sensor register shared memory address*/
	uint32_t sensor_reg_address_low;
	/*Higher sensor register shared memory address*/
	uint32_t sensor_reg_address_high;
	/*length of the sensor register shared memory*/
	uint32_t sensor_reg_length;

	/*
	 * This should have the Asynchronous sensor read support in this platform
	 * bounded by the number of sensor descriptors.
	 */
	uint8_t *asynchronous_sensor_read_support;

	/*
	 * This should have the Number of trip points supported in this platform
	 * bounded by the number of sensor descriptors.
	 */
	uint8_t *number_of_trip_points_supported;

	/*
	 * This should have the type of sensor and the measurement system it
	 * implements in this platform bounded by the number of sensor descriptors.
	 */
	uint8_t *sensor_type;

	/*
	 * This should have the names of the sensors in this platform bounded by the
	 * number of sensor descriptors.
	 */
	char  **sensor_name;

	/*Platform support for each command in sensor protocol*/
	bool sensor_trip_point_notify_supported;

	/*Platform support for each command in sensor protocol*/
	bool sensor_trip_point_config_supported;
};

#endif /* SENSOR_EXPECTED_H_ */
