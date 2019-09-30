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

#ifndef __POWER_DOMAIN_TESTLIST_H__
#define __POWER_DOMAIN_TESTLIST_H__

uint32_t power_domain_query_protocol_version(void);
uint32_t power_domain_query_protocol_attributes(void);
uint32_t power_domain_query_mandatory_command_support(void);
uint32_t power_domain_query_domain_attributes(void);
uint32_t power_domain_set_power_state_check(void);
uint32_t power_domain_get_power_state_check(void);
uint32_t power_domain_power_state_notify_check(void);
uint32_t power_domain_invalid_messageid_call(void);
#ifndef SCMI_VERSION_1
uint32_t power_domain_power_state_change_requested_notify_check(void);
#endif

#endif
