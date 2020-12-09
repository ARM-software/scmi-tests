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
#include <pal_platform.h>
#include <pal_base_expected.h>

/*-----------  Common PAL_BASE API's across platforms ----------*/

uint32_t pal_agent_get_accessible_device(uint32_t agent_id)
{
    return agent_get_accessible_device(agent_id);
}

uint32_t pal_agent_get_inaccessible_device(uint32_t agent_id)
{
    return agent_get_inaccessible_device(agent_id);
}

uint32_t pal_device_get_accessible_protocol(uint32_t device_id)
{
    return device_get_accessible_protocol(device_id);
}

uint32_t pal_check_trusted_agent(uint32_t agent_id)
{
    return check_trusted_agent(agent_id);
}

char *pal_base_get_expected_vendor_name(void)
{
    return &(vendor_name[0]);
}

char *pal_base_get_expected_subvendor_name(void)
{
    return &(subvendor_name[0]);
}

uint32_t pal_base_get_expected_implementation_version(void)
{
    return implementation_version;
}

uint32_t pal_base_get_expected_num_agents(void)
{
    return NUM_ELEMS(agents);
}

uint32_t pal_base_get_expected_num_protocols(void)
{
    return NUM_ELEMS(supported_protocols);
}

