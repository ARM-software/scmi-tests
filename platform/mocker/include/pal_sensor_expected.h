/** @file
 * Copyright (c) 2020, Arm Limited or its affiliates. All rights reserved.
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

/* Expected SENSOR parameters */
#define NUM_OF_SENSORS_SUPPORTED        2
#define SENOR_ASYNC_READ_SUPPORTED      1
#define SENOR_ASYNC_READ_NOT_SUPPORTED  0
/*
 * This should have the Asynchronous sensor read support in this platform
 * bounded by the number of sensor descriptors.
 */
static uint8_t async_sensor_read_support[] =
{
    SENOR_ASYNC_READ_NOT_SUPPORTED,
    SENOR_ASYNC_READ_NOT_SUPPORTED
};

/*
 * This should have the Number of trip points supported in this platform
 * bounded by the number of sensor descriptors.
 */
static uint8_t number_of_trip_points_supported[] =
{
    0,
    2
};

static uint32_t num_sensors = NUM_OF_SENSORS_SUPPORTED;

static uint32_t statistics_address_low_snsr = 0x1234;
static uint32_t statistics_address_len_snsr = 0xFF;

#endif /* __PAL_SENSOR_EXPECTED_H__ */
