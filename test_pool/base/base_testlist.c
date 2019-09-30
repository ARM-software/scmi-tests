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
#include "base_testlist.h"

test_fn_ptr base_test[] = {
    base_query_protocol_version,
    base_query_protocol_attributes,
    base_query_mandatory_command_support,
    base_query_vendor_name,
    base_query_implementation_version,
    base_query_subvendor_info,
#ifdef SCMI_VERSION_1
    base_discover_agent_v1,
#else
    base_discover_agent,
#endif
    base_query_notify_error_support,
    base_query_protocol_list,
    base_invalid_messageid_call,
#ifndef SCMI_VERSION_1
    base_set_protocol_permissions_check,
    base_set_device_permissions_check,
    base_reset_agent_configuration_check,
    base_deny_restore_protocol_access,
    base_restore_protocol_access_with_reset_agent_configuration,
    base_deny_restore_device_access,
    base_restore_device_access_with_reset_agent_configuration
#endif
};

test_fn_ptr *val_base_get_testlist(uint32_t *num_tests)
{
   *num_tests = NUM_ELEMS(base_test);
    return base_test;
}
