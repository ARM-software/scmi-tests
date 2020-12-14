/** @file
 * Copyright (c) 2019-2020, Arm Limited or its affiliates. All rights reserved.
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

#ifndef __VAL_BASE_H__
#define __VAL_BASE_H__

#define VERSION_OFFSET                0
#define ATTRIBUTE_OFFSET              0
#define VENDOR_ID_OFFSET              0
#define IMPLEMENTATION_VERSION_OFFSET 0
#define NUM_OF_PROTOCOLS_OFFSET       0
#define PROTOCOL_LIST_OFFSET          1
#define AGENT_ID_OFFSET               0
#define AGENT_NAME_OFFSET             1

#define AGENT_NAME_v1_OFFSET          0

#define INVALID_NOTIFY_ENABLE        0xF
#define INVALID_FLAG_VAL             0xF
#define FLAG_ACCESS_ALLOW            1
#define FLAG_ACCESS_DENY             0
#define FLAG_RESET_ACCESS            1

typedef struct {
    uint32_t num_agents;
    uint32_t num_protocols;
    uint32_t test_agent_id;
    uint32_t implementation_version;
    uint8_t  vendor_name[SCMI_NAME_STR_SIZE];
    uint8_t  subvendor_name[SCMI_NAME_STR_SIZE];
    uint8_t  test_agent_name[SCMI_NAME_STR_SIZE];
} BASE_INFO_s;

/* Common Tests */
uint32_t base_query_protocol_version(uint32_t *version);
uint32_t base_query_protocol_attributes(void);
uint32_t base_query_mandatory_command_support(void);
uint32_t base_query_vendor_name(void);
uint32_t base_query_implementation_version(void);
uint32_t base_query_subvendor_info(void);
uint32_t base_query_notify_error_support(void);
uint32_t base_query_protocol_list(void);
uint32_t base_invalid_messageid_call(void);

/* v1 specific tests */
uint32_t base_discover_agent_v1(void);

/* v2 specific tests */
uint32_t base_discover_agent(void);
uint32_t base_set_device_permissions_check(void);
uint32_t base_set_protocol_permissions_check(void);
uint32_t base_reset_agent_configuration_check(void);
uint32_t base_deny_restore_protocol_access(void);
uint32_t base_restore_protocol_access_with_reset_agent_configuration(void);
uint32_t base_deny_restore_device_access(void);
uint32_t base_restore_device_access_with_reset_agent_configuration(void);

/* Expected Values */
char *val_base_get_expected_vendor_name(void);
char *val_base_get_expected_subvendor_name(void);
uint32_t val_base_get_expected_implementation_version(void);
uint32_t val_base_get_expected_num_agents(void);
uint32_t val_base_get_expected_num_protocols(void);

#endif
