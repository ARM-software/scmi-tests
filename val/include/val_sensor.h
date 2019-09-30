/** @file
 * Copyright (c) 2019, Arm Limited or its affiliates. All rights reserved.
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

#ifndef __VAL_SENSOR_H__
#define __VAL_SENSOR_H__

#define SENSOR_VERSION 0x00010000

typedef struct {
    uint32_t num_sensors;
    uint32_t sensor_stats_addr_low;
    uint8_t  sensor_stats_addr_high;
    uint8_t  sensor_stats_addr_len;
} SENSOR_INFO_s;

#endif
