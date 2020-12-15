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

#include "pal_interface.h"
#include "pal_base_expected.h"

/**
  @brief   This API is used to check vendor name
  @param   none
  @return  vendor name
**/
char *pal_base_get_expected_vendor_name(void)
{
    return vendor_name;
}

/**
  @brief   This API is used for checking subvendor name
  @param   none
  @return  subvendor name
**/
char *pal_base_get_expected_subvendor_name(void)
{
    return subvendor_name;
}

/**
  @brief   This API is used for checking implementation version
  @param   none
  @return  implementation version
**/
uint32_t pal_base_get_expected_implementation_version(void)
{
    return implementation_version;
}

/**
  @brief   This API is used for checking num of agents
  @param   none
  @return  num of agents
**/
uint32_t pal_base_get_expected_num_agents(void)
{
    return NUM_ELEMS(agents);
}

/**
  @brief   This API is used for checking num of protocols agent have access
  @param   none
  @return  num of protocols
**/
uint32_t pal_base_get_expected_num_protocols(void)
{
    return NUM_ELEMS(supported_protocols);
}

/**
  @brief   This API is used for checking if agent is trusted
  @param   agent id
  @return  true/false
**/
uint32_t pal_check_trusted_agent(uint32_t agent_id)
{
    return UNTRUSTED;
}

/**
  @brief   This API is used to get device which agent can access
  @param   agent id
  @return  device id
**/
uint32_t pal_agent_get_accessible_device(uint32_t agent_id)
{
    return sgm_agent_get_accessible_device(agent_id);
}

/**
  @brief   This API is used to get device which agent cannot access
  @param   agent id
  @return  device id
**/
uint32_t pal_agent_get_inaccessible_device(uint32_t agent_id)
{
    return sgm_agent_get_inaccessible_device(agent_id);
}

/**
  @brief   This API is used to get protocol by which given device can be access
  @param   device id
  @return  protocol id
**/
uint32_t pal_device_get_accessible_protocol(uint32_t device_id)
{
    return sgm_device_get_accessible_protocol(device_id);
}
