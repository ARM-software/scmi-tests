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
#include <pal_power_domain_expected.h>

/*-----------  Common POWER API's across platforms ----------*/

uint32_t pal_power_get_expected_num_domains(void)
{
    return num_power_domains;
}

uint32_t pal_power_get_expected_stats_addr_low(void)
{
    return statistics_address_low_pow;
}

uint32_t pal_power_get_expected_stats_addr_high(void)
{
    return (statistics_address_len_pow + statistics_address_low_pow);
}

uint32_t pal_power_get_expected_stats_addr_len(void)
{
    return statistics_address_len_pow;
}
