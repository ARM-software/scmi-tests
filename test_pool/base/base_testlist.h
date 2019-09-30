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

#ifndef __BASE_TESTLIST_H__
#define __BASE_TESTLIST_H__

uint32_t base_query_protocol_version(void);
uint32_t base_query_protocol_attributes(void);
uint32_t base_query_mandatory_command_support(void);
uint32_t base_query_vendor_name(void);
uint32_t base_query_implementation_version(void);
uint32_t base_query_subvendor_info(void);
#ifdef SCMI_VERSION_1
uint32_t base_discover_agent_v1(void);
#else
uint32_t base_discover_agent(void);
#endif
uint32_t base_query_notify_error_support(void);
uint32_t base_query_protocol_list(void);
uint32_t base_invalid_messageid_call(void);
#ifndef SCMI_VERSION_1
uint32_t base_set_protocol_permissions_check(void);
uint32_t base_set_device_permissions_check(void);
uint32_t base_reset_agent_configuration_check(void);
uint32_t base_deny_restore_protocol_access(void);
uint32_t base_restore_protocol_access_with_reset_agent_configuration(void);
uint32_t base_deny_restore_device_access(void);
uint32_t base_restore_device_access_with_reset_agent_configuration(void);
#endif

#endif
