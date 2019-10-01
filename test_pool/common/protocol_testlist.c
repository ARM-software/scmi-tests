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

#include "val_interface.h"
#include "protocol_testlist.h"

test_fn_ptr *val_get_testlist(uint32_t protocol_identifier, uint32_t *num_tests)
{
    switch (protocol_identifier)
    {
    case PROTOCOL_BASE:
        return val_base_get_testlist(num_tests);
        break;
#ifdef POWER_DOMAIN_PROTOCOL
    case PROTOCOL_POWER_DOMAIN:
        return val_power_domain_get_testlist(num_tests);
        break;
#endif
#ifdef SYSTEM_POWER_PROTOCOL
    case PROTOCOL_SYSTEM_POWER:
        return val_system_power_get_testlist(num_tests);
        break;
#endif
#ifdef CLOCK_PROTOCOL
    case PROTOCOL_CLOCK:
        return val_clock_get_testlist(num_tests);
        break;
#endif
#ifdef SENSOR_PROTOCOL
    case PROTOCOL_SENSOR:
        return val_sensor_get_testlist(num_tests);
        break;
#endif
#ifdef PERFORMANCE_PROTOCOL
    case PROTOCOL_PERFORMANCE:
        return val_performance_get_testlist(num_tests);
        break;
#endif
    default:
        *num_tests = 0;
        return NULL;
    }
}

/**
  @brief   This API is used to get protocol name
           1. Caller       -  Test Suite.
           2. Prerequisite -  Base protocol info table.
  @param   protocol_id  protocol identifier
  @return  string       protocol name
**/
char *val_get_protocol_str(uint32_t protocol_id)
{
    switch (protocol_id)
    {
    case PROTOCOL_BASE:
        return "BASE";
        break;
    case PROTOCOL_POWER_DOMAIN:
        return "POWER DOMAIN";
        break;
    case PROTOCOL_SYSTEM_POWER:
        return "SYSTEM POWER";
        break;
    case PROTOCOL_PERFORMANCE:
        return "PERFORMANCE";
        break;
     case PROTOCOL_SENSOR:
        return "SENSOR";
        break;
    case PROTOCOL_CLOCK:
        return "CLOCK";
        break;
    default:
        val_print(VAL_PRINT_WARN, "\n\tUnkown protocol id %d", protocol_id);
        return "UNKNOWN";
    }
}
