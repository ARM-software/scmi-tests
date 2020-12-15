/** @file
 * Copyright (c) 2017-2020, Arm Limited or its affiliates. All rights reserved.
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

#ifndef __PAL_SYSTEM_POWER_EXPECTED_H__
#define __PAL_SYSTEM_POWER_EXPECTED_H__

#include <inttypes.h>
#include <stdbool.h>

struct arm_scmi_system_power_expected {
    /*
     * This enum specifies the bit position in the flags_mask field
     * indicating if the expected value is given (bit set) or ignored
     * (bit unset).
     */
    enum {
        SYSTEM_POWER_PROTOCOL_VERSION,
        SYSTEM_POWER_STATE_NOTIFY_CMD_SUPPORTED
    } FLAGS;
    /*
     * The expectation is that there is no more
     * than 32 elements specified via this expected results data structure.
     * This mask is used to indicate which of the structure members hold valid
     * data.
     */
    uint32_t flags_mask;
    uint32_t protocol_version;

    bool system_power_state_notify_cmd_supported;
};

#endif /* __PAL_SYSTEM_POWER_EXPECTED_H__ */
