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

#include <power_domain_protocol.h>
#include <power_domain_common.h>
#include <pal_power_domain_expected.h>

struct arm_scmi_power_protocol power_protocol;
extern bool device_access_permitted (void);
extern bool protocol_access_permitted (uint32_t protocol_id);

void fill_power_protocol()
{
    power_protocol.protocol_version = POWER_DOMAIN_VERSION;
    power_protocol.num_power_domains = num_power_domains;
    power_protocol.statistics_address_low = statistics_address_low_pow;
    power_protocol.statistics_length = statistics_address_len_pow;
    power_protocol.statistics_address_high =
    power_protocol.statistics_address_low + power_protocol.statistics_length;
    power_protocol.power_state_notify_cmd_supported = true;
    power_protocol.power_state_change_requested_notify_cmd_supported = true;
}

void power_send_message(uint32_t message_id, uint32_t parameter_count,
        const uint32_t *parameters, int32_t *status,
        size_t *return_values_count, uint32_t *return_values)
{

    uint32_t parameter_idx, return_idx;
    char * str;
    int num_power_domains = 3;
    int stats_low = 0x1234, stats_high = stats_low + 0xff;

    if(device_access_permitted() == false ||
       protocol_access_permitted(POWER_DOMAIN_PROTOCOL_ID) == false) {
        *status = SCMI_STATUS_DENIED;
        return;
    }
    switch(message_id)
    {
        case PWR_PROTO_VER_MSG_ID:
            *status = SCMI_STATUS_SUCCESS;
            *return_values_count = 1;
            return_values[0] = power_protocol.protocol_version;
            break;
        case PWR_PROTO_ATTR_MSG_ID:
            *status = SCMI_STATUS_SUCCESS;
            *return_values_count = 1;
            return_idx = OFFSET_RET(struct arm_scmi_power_protocol_attributes,
                    attributes);
            return_values[return_idx] =
                    (num_power_domains << POWER_PROT_ATTR_NUMBER_DOMAINS_LOW);
            return_idx = OFFSET_RET(struct arm_scmi_power_protocol_attributes,
                    statistics_address_low);
            return_values[return_idx] = stats_low;
            return_idx = OFFSET_RET(struct arm_scmi_power_protocol_attributes,
                    statistics_address_high);
            return_values[return_idx] = stats_high;
            return_idx = OFFSET_RET(struct arm_scmi_power_protocol_attributes,
                    statistics_len);
            return_values[return_idx] = stats_high - stats_low;
            break;
        case PWR_PROTO_MSG_ATTR_MSG_ID:
            parameter_idx = OFFSET_PARAM(
            struct arm_scmi_power_protocol_message_attributes, message_id);
            if(parameters[parameter_idx] > PWR_STATE_CHANGE_REQUESTED_NOTIFY_MSG_ID) {
                *status = SCMI_STATUS_NOT_FOUND;
            } else {
                *status = SCMI_STATUS_SUCCESS;
                *return_values_count = 1;
                return_values[OFFSET_RET(
                struct arm_scmi_power_protocol_message_attributes, attributes)] = 0x0;
            }
            break;
        case PWR_DOMAIN_ATTRIB_MSG_ID:
            *status = (parameters[OFFSET_PARAM(
                    struct arm_scmi_power_domain_attributes,
                    domain_id)] > num_power_domains ?
                    SCMI_STATUS_NOT_FOUND: SCMI_STATUS_SUCCESS);
            *return_values_count = 3;
            if(parameters[OFFSET_PARAM(
                    struct arm_scmi_power_domain_attributes,
                    domain_id)] == 0)
            {
                return_values[OFFSET_RET(
                        struct arm_scmi_power_domain_attributes,
                        attributes)] =
                        (1 << POWER_DOMAIN_ATTR_SYNC_LOW) +
                        (1 << POWER_DOMAIN_ATTR_ASYNC_LOW) +
                        (0 << POWER_DOMAIN_ATTR_STATE_NOTIFICATIONS_LOW);
            }
            else
            {
                return_values[OFFSET_RET(
                        struct arm_scmi_power_domain_attributes,
                        attributes)] =
                        (1 << POWER_DOMAIN_ATTR_SYNC_LOW) +
                        (1 << POWER_DOMAIN_ATTR_ASYNC_LOW) +
                        (1 << POWER_DOMAIN_ATTR_STATE_NOTIFICATIONS_LOW);
            }
            str = (char *)
            (&return_values[OFFSET_RET(
                    struct arm_scmi_power_domain_attributes, name)]);
            sprintf(str, "Domain_%d", parameters[
            OFFSET_PARAM(struct arm_scmi_power_domain_attributes,
                    domain_id)]);
            break;
        case PWR_STATE_SET_MSG_ID:
            if ((parameters[OFFSET_PARAM(struct arm_scmi_power_state_set,
                    power_state)] & (1 << POWER_STATE_SET_RESERVED_HIGH)) != 0) {
                *status = SCMI_STATUS_INVALID_PARAMETERS;
                break;
            }
            if (parameters[OFFSET_PARAM(struct arm_scmi_power_state_set,
                    flags)] > 1) {
                *status = SCMI_STATUS_INVALID_PARAMETERS;
                break;
            }
            if (parameters[OFFSET_PARAM(struct arm_scmi_power_state_set,
                    domain_id)] >= num_power_domains) {
                *status = SCMI_STATUS_NOT_FOUND;
                break;
            }
            *status = SCMI_STATUS_SUCCESS;
            break;
        case PWR_STATE_GET_MSG_ID:
            if (parameters[OFFSET_PARAM(
                    struct arm_scmi_power_state_get, domain_id)] >=
                    num_power_domains) {
                *status = SCMI_STATUS_NOT_FOUND;
                break;
            }
            *status = SCMI_STATUS_SUCCESS;
            *return_values_count = 1;
            return_values[OFFSET_RET(struct arm_scmi_power_state_get,
                    power_state)] = 0xf;
            break;
        case PWR_STATE_NOTIFY_MSG_ID:
            if (parameters[OFFSET_PARAM(
                    struct arm_scmi_power_state_notify,
                    notify_enable)] > 1) {
                *status = SCMI_STATUS_INVALID_PARAMETERS;
                break;
            }
            if (parameters[OFFSET_PARAM(
                    struct arm_scmi_power_state_notify,
                    domain_id)] >= num_power_domains) {
                *status = SCMI_STATUS_NOT_FOUND;
                break;
            }
            if (parameters[OFFSET_PARAM(
                    struct arm_scmi_power_state_notify,
                    domain_id)] == 0) {
                *status = SCMI_STATUS_NOT_SUPPORTED;
                break;
            }
            *status = SCMI_STATUS_SUCCESS;
            break;
        case PWR_STATE_CHANGE_REQUESTED_NOTIFY_MSG_ID:
            if (parameters[OFFSET_PARAM(
                    struct arm_scmi_power_state_change_requested_notify,
                    notify_enable)] > 1) {
                *status = SCMI_STATUS_INVALID_PARAMETERS;
                break;
            }
            if (parameters[OFFSET_PARAM(
                    struct arm_scmi_power_state_change_requested_notify,
                    domain_id)] >= num_power_domains) {
                *status = SCMI_STATUS_NOT_FOUND;
                break;
            }
            *status = SCMI_STATUS_SUCCESS;
            break;
        default:
            *status = SCMI_STATUS_NOT_FOUND;
    }
}
