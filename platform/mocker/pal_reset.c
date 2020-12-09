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
#include <pal_reset_expected.h>

/*-----------  Common RESET API's across platforms ----------*/

uint32_t pal_reset_get_expected_num_domains(void)
{
    return num_reset_domains;
}

uint8_t *pal_reset_get_expected_name(uint32_t domain_id)
{
    return (uint8_t *)reset_domain_names[domain_id];
}

uint32_t pal_reset_get_expected_async_support(uint32_t domain_id)
{
    return async_reset_support[domain_id];
}

uint32_t pal_reset_get_expected_notify_support(uint32_t domain_id)
{
    return reset_notify_support[domain_id];
}

uint32_t pal_reset_get_expected_latency(uint32_t domain_id)
{
    return reset_latency[domain_id];
}
