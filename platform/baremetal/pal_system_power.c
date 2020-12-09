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

#ifdef SYSTEM_POWER_PROTOCOL

#include "pal_interface.h"
#include "pal_system_power_expected.h"

extern void pal_system_power_set_expected(const void *);
static struct arm_scmi_system_power_expected *g_system_power_info;

/**
  @brief   Sets system power protocol info
  @param   platform system power  protocol info
  @return  none
**/
void pal_system_power_set_expected(const void *info)
{
    if (info == NULL)
        return;
    g_system_power_info = (struct arm_scmi_system_power_expected *)info;
}


/*-----------  Common PAL API's across platforms ----------*/

#endif
