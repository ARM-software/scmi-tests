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

#ifndef __PAL_SENSOR_EXPECTED_H__
#define __PAL_SENSOR_EXPECTED_H__

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

#endif /* __PAL_SENSOR_EXPECTED_H__ */
