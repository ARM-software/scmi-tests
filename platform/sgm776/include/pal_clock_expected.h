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

#ifndef __PAL_CLOCK_EXPECTED_H__
#define __PAL_CLOCK_EXPECTED_H__

/* Expected CLOCK parameters */
#ifdef CLOCK_PROTOCOL

#define UNSUPPORTED 1

static uint32_t num_clocks = 0x04;
static uint32_t num_of_clock_rates[] = {
    0x01, /* Clock 0 */
    0x01, /* Clock 1 */
    0x01, /* Clock 2 */
    0x01 /* Clock 3 */
};

uint32_t max_num_pending_async_rate_chg_supported = 0;

#endif

#endif /* __PAL_CLOCK_EXPECTED_H__ */
