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

#ifndef __VAL_BASE_H__
#define __VAL_BASE_H__

#ifdef SCMI_VERSION_1
#define BASE_VERSION 0x00010000
#else
#define BASE_VERSION 0x00020000
#endif

typedef struct {
    uint32_t num_agents;
    uint32_t num_protocols;
    uint32_t test_agent_id;
    uint32_t implementation_version;
    uint8_t  vendor_name[SCMI_NAME_STR_SIZE];
    uint8_t  subvendor_name[SCMI_NAME_STR_SIZE];
    uint8_t  test_agent_name[SCMI_NAME_STR_SIZE];
} BASE_INFO_s;

#endif
