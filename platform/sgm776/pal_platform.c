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

#include "pal_platform.h"
#include "pal_interface.h"
#include "pal_expected.h"

void pal_send_message(uint32_t message_header_send, uint32_t parameter_count,
        const uint32_t *parameters, uint32_t *message_header_rcv, int32_t *status,
        uint32_t *return_values_count, uint32_t *return_values)
{
    sgm_send_message(message_header_send, parameter_count, parameters,
                        message_header_rcv, status, return_values_count,
                        return_values);
}

void pal_initialize_system(void)
{
    return;
}

void pal_print(uint32_t print_level, const char *format, va_list args)
{
    FILE *file_ptr = fopen(LOG_FILE, "a");
    if (file_ptr)
    {
        vfprintf(file_ptr, format, args);
        fclose(file_ptr);
    }
    else
        printf("ERROR: Log File opening failed");
}

char *pal_base_get_protocol_vendor_name(void)
{
    return vendor_name;
}

char *pal_base_get_expected_subvendor_name(void)
{
    return subvendor_name;
}

uint32_t pal_base_get_expected_implementation_version(void)
{
    return implementation_version;
}

uint32_t pal_base_get_expected_num_agents(void)
{
    return NUM_ELEMS(agents);
}

uint32_t pal_pwr_domain_get_expected_num_domains(void)
{
    return num_power_domains;
}

uint32_t pal_get_expected_num_supported_protocols(void)
{
    return NUM_ELEMS(supported_protocols);
}

uint32_t pal_agent_get_accessible_device(uint32_t agent_id)
{
    return sgm_agent_get_accessible_device(agent_id);
}

uint32_t pal_agent_get_inaccessible_device(uint32_t agent_id)
{
    return sgm_agent_get_inaccessible_device(agent_id);
}

uint32_t pal_device_get_accessible_protocol(uint32_t device_id)
{
    return sgm_device_get_accessible_protocol(device_id);
}
