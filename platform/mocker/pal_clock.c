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

#include <pal_interface.h>
#include <pal_clock_expected.h>

/*-----------  Common CLOCK API's across platforms ----------*/

uint32_t pal_clock_get_expected_num_clocks(void)
{
    return num_clocks;
}

uint32_t pal_clock_get_expected_max_async_cmd(void)
{
    return max_num_pending_async_rate_chg_supported;
}

uint32_t pal_clock_get_expected_number_of_rates(uint32_t clock_id)
{
    return num_of_clock_rates[clock_id];
}

uint32_t pal_check_clock_config_change_support(uint32_t clock_id)
{
    return 0;
}
