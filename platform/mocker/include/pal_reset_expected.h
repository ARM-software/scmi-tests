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

#ifndef __PAL_RESET_EXPECTED_H__
#define __PAL_RESET_EXPECTED_H__

/* Expected RESET parameters */

/* Rest Mocker defines*/
#define RESET_ASYNC_SUPPORTED       1
#define RESET_ASYNC_NOT_SUPPORTED   0

#define RESET_NOTIFY_SUPPORTED      1
#define RESET_NOTIFY_NOT_SUPPORTED  0

/* Expected RESET parameters */
static uint32_t num_reset_domains = 0x3;

static char* reset_domain_names[] =
{
    "Reset_0",
    "Reset_1",
    "Reset_2"
};

static uint32_t async_reset_support[] =
{
    RESET_ASYNC_SUPPORTED,
    RESET_ASYNC_NOT_SUPPORTED,
    RESET_ASYNC_SUPPORTED
};

static uint32_t reset_notify_support[] =
{
    RESET_NOTIFY_NOT_SUPPORTED,
    RESET_NOTIFY_SUPPORTED,
    RESET_NOTIFY_SUPPORTED
};

static uint32_t reset_latency[] =
{
    10000,
    100000,
    2000000,
};

#endif /* __PAL_RESET_EXPECTED_H__ */
