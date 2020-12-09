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

#include <system_power_protocol.h>
#include <system_power_common.h>

struct arm_scmi_system_power_protocol system_power_protocol;

void system_power_send_message(uint32_t message_id, uint32_t parameter_count,
        const uint32_t *parameters, int32_t *status,
        size_t *return_values_count, uint32_t *return_values)
{

    uint32_t parameter_idx;
    int warm_reset_support = 1, system_suspend_support = 1;

    switch(message_id)
    {
    case SYSTEM_POWER_PROTO_VER_MSG_ID:
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 1;
        return_values[0] = 0x00010000; //PROTOCOL_VERSION;
        break;
    case SYSTEM_POWER_PROTO_ATTRIB_MSG_ID:
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 1;
        return_values[0] = 0x0;
        break;
    case SYSTEM_POWER_PROTO_MSG_ATTRIB_MSG_ID:
        parameter_idx = OFFSET_PARAM(struct arm_scmi_system_power_protocol_message_attributes,
                message_id);
        if (parameters[parameter_idx] > SYSTEM_POWER_STATE_NOTIFY_MSG_ID) {
            *status = SCMI_STATUS_NOT_FOUND;
            return;
        }
        parameter_idx = OFFSET_PARAM(struct arm_scmi_system_power_protocol_message_attributes,
                message_id);
        if (parameters[parameter_idx] != SYSTEM_POWER_STATE_SET_MSG_ID) {
            /*except for SYSPWR_PROTO_MSG_ATTRIB_MSG_ID, return 0*/
                    *status = SCMI_STATUS_SUCCESS;
            *return_values_count = 1;
            return_values[0] = 0x0;
        }
        else {
            *status = SCMI_STATUS_SUCCESS;
            *return_values_count = 1;
            /*System warm reset and suspend are supported*/
            return_values[0] = (warm_reset_support << 31) +
                    (system_suspend_support << 30) + 0;
        }
        break;
    case SYSTEM_POWER_STATE_SET_MSG_ID:
        parameter_idx = OFFSET_PARAM(struct arm_scmi_system_power_state_set,
                system_state);
        if ((parameters[parameter_idx] >= 0x5) &&
                (parameters[OFFSET_PARAM(
                        struct arm_scmi_system_power_state_set,
                        system_state)] <= 0x7fffffff)) {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
            return;
        }
        if ((parameters[OFFSET_PARAM(
                struct arm_scmi_system_power_state_set,
                flags)] > 0x1))
        {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
            return;
        }
        *status = SCMI_STATUS_SUCCESS;
        break;
    case SYSTEM_POWER_STATE_GET_MSG_ID:
        *status = SCMI_STATUS_NOT_SUPPORTED;
        *return_values_count = 1;
        return_values[0] = 0x3;
        break;
    case SYSTEM_POWER_STATE_NOTIFY_MSG_ID:
        if ((parameters[OFFSET_PARAM(
                struct arm_scmi_system_power_state_set_notify,
                notify_enable)] > 0x1))
        {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
            return;
        }
        *status = SCMI_STATUS_SUCCESS;
        break;
    default:
        *status = SCMI_STATUS_NOT_FOUND;
    }

}
