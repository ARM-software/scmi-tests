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

#ifndef __PAL_TARGET_H__
#define __PAL_TARGET_H__

/***** This file contains platform specific data which is needed during test verification ****/

#define BASE_PROTOCOL_ID        0x10
#define POWER_DOMAIN_PROTOCOL_ID  0x11

static uint32_t supported_protocols[] = {
    BASE_PROTOCOL_ID,
    POWER_DOMAIN_PROTOCOL_ID,
};

static char *agents[] = {
    "OSPM", "PSCI"
};

/* Expected BASE parameters */

static char *vendor_name = "mocker";
static char *subvendor_name = "mocker";
static uint32_t implementation_version = 1;

/* Expected POWER DOMAIN parameters */

static uint32_t num_power_domains = 3;

/* Expected SENSOR parameters */

static uint32_t num_sensors = 2;

/* Expected PERFORMANCE parameters */

static uint32_t num_performance_domains = 4;

/* Expected CLOCK parameters */

static uint32_t num_clocks = 8;

#endif /* PAL_TARGET_H_ */
