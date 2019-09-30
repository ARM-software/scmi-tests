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
#include "power_domain_testlist.h"

test_fn_ptr power_domain_test[] = {
    power_domain_query_protocol_version,
    power_domain_query_protocol_attributes,
    power_domain_query_mandatory_command_support,
    power_domain_query_domain_attributes,
    power_domain_set_power_state_check,
    power_domain_get_power_state_check,
    power_domain_power_state_notify_check,
    power_domain_invalid_messageid_call,
#ifndef SCMI_VERSION_1
    power_domain_power_state_change_requested_notify_check,
#endif
};

test_fn_ptr *val_power_domain_get_testlist(uint32_t *num_tests)
{
   *num_tests = NUM_ELEMS(power_domain_test);
    return power_domain_test;
}
