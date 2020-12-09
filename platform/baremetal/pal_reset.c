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

#ifdef RESET_PROTOCOL

#include "pal_interface.h"
#include "pal_reset_expected.h"

extern void pal_reset_set_expected(const void *);
static struct arm_scmi_reset_expected *g_reset_info;

/**
  @brief   Sets reset domain protocol info
  @param   platform reset domain protocol info
  @return  none
**/
void pal_reset_set_expected(const void *info)
{
    if (info == NULL)
        return;
    g_reset_info = (struct arm_scmi_reset_expected *)info;
}

/*-----------  Common PAL API's across platforms ----------*/

/**
  @brief   This API is used for checking num of reset domain
  @param   none
  @return  num of reset domain
**/
uint32_t pal_reset_get_expected_num_domains(void)
{
    return g_reset_info->number_domains;
}

/**
  @brief   This API is used for checking reset domain name
  @param   domain id
  @return  reset domain name
**/
uint8_t *pal_reset_get_expected_name(uint32_t domain_id)
{
    return (uint8_t *)g_reset_info->reset_domain_name[domain_id];
}

#endif
