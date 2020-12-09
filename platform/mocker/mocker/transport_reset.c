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

#include <reset_protocol.h>
#include <reset_common.h>
#include <pal_reset_expected.h>

struct arm_scmi_reset_protocol reset_protocol;

void fill_reset_protocol()
{
    reset_protocol.protocol_version = RESET_VERSION;
    reset_protocol.number_domains = num_reset_domains;
    reset_protocol.reset_domain_name = reset_domain_names;
    reset_protocol.asynchronous_reset_supported = async_reset_support;
    reset_protocol.reset_notify_supported = reset_notify_support;
    reset_protocol.reset_latency = reset_latency;
}

void reset_send_message(uint32_t message_id, uint32_t parameter_count,
        const uint32_t *parameters, int32_t *status,
        size_t *return_values_count, uint32_t *return_values)
{

    uint32_t parameter_idx, domain_id, return_idx;
    char * str;
    uint32_t reset_flag, reset_state , notify_en;

    switch(message_id)
    {
    case RESET_PROTO_VER_MSG_ID:
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 1;
        return_values[0] = reset_protocol.protocol_version;
        break;
    case RESET_PROTO_ATTRIB_MSG_ID:
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 1;
        return_values[OFFSET_RET(
                struct arm_scmi_reset_protocol_attributes,
                attributes)] =
                        (reset_protocol.number_domains <<
                                RESET_NUMBER_DOMAIN_LOW);
        break;
    case RESET_PROTO_MSG_ATTRIB_MSG_ID:
        parameter_idx = OFFSET_PARAM(
                struct arm_scmi_reset_protocol_message_attributes, message_id);
        if (parameters[parameter_idx] > RESET_PROTO_NOTIFY_MSG_ID)
        {
            *status = SCMI_STATUS_NOT_FOUND;
        }
        else
        {
            *status = SCMI_STATUS_SUCCESS;
            *return_values_count = 1;
            return_values[OFFSET_RET(
                    struct arm_scmi_reset_protocol_message_attributes, attributes)] = 0x0;
        }
        break;
    case RESET_PROTO_DOMAIN_ATTRIB_MSG_ID:
        /* get domain id */
        parameter_idx = OFFSET_PARAM(
                struct arm_scmi_reset_protocol_domain_attributes, domain_id);
        domain_id = parameters[parameter_idx];
        if (domain_id >= reset_protocol.number_domains)
        {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }

        /* Domain attributes*/
        return_idx = OFFSET_RET(struct arm_scmi_reset_protocol_domain_attributes,
                attributes);
        return_values[return_idx] =
                reset_protocol.asynchronous_reset_supported[domain_id] << RESET_ASYNC_SUPPORT_BIT |
                reset_protocol.reset_notify_supported[domain_id] << RESET_NOTIFY_SUPPORT_BIT;

        /* Reset Latency*/
        return_idx = OFFSET_RET(struct arm_scmi_reset_protocol_domain_attributes,
                latency);
        return_values[return_idx] = reset_protocol.reset_latency[domain_id];

        /* Reset domain name*/
        return_idx = OFFSET_RET(struct arm_scmi_reset_protocol_domain_attributes,
                domain_name);

        str = (char *)&return_values[return_idx];
        sprintf(str, "Reset_%d", domain_id);

        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = (return_idx + (SCMI_NAME_STR_SIZE/4)) + 1;
        break;
    case RESET_PROTO_RESET_MSG_ID:
        /* get domain id */
        parameter_idx = OFFSET_PARAM(
                struct arm_scmi_reset_protocol_reset, domain_id);
        domain_id = parameters[parameter_idx];
        if (domain_id >= reset_protocol.number_domains)
        {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        /* get reset flag */
        parameter_idx = OFFSET_PARAM(
             struct arm_scmi_reset_protocol_reset, flags);
        reset_flag = parameters[parameter_idx];
        /* Only 3 bits are valid*/
        if (reset_flag > 0x7)
        {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
            break;
        }
        if (((reset_flag >> RESET_FLAG_ASYNC_RESET_BIT) & 0x1) &&
                reset_protocol.asynchronous_reset_supported[domain_id] == 0)
        {
            *status = SCMI_STATUS_NOT_SUPPORTED;
            break;
        }
        /* get reset state */
        parameter_idx = OFFSET_PARAM(
               struct arm_scmi_reset_protocol_reset, reset_state);
        reset_state = parameters[parameter_idx];
        if(reset_state > 0) /* Mocker supported only cold reset */
        {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
            break;
        }
        *status = SCMI_STATUS_SUCCESS;
        break;
    case RESET_PROTO_NOTIFY_MSG_ID:
        /* get domain id */
        parameter_idx = OFFSET_PARAM(
                struct arm_scmi_reset_protocol_notify_enable, domain_id);
        domain_id = parameters[parameter_idx];
        if (domain_id >= reset_protocol.number_domains)
        {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        /* get notify_enable */
        parameter_idx = OFFSET_PARAM(
               struct arm_scmi_reset_protocol_notify_enable, notify_enable);
        notify_en = parameters[parameter_idx];
        if(notify_en > 1) /* invalid notify enable */
        {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
            break;
        }
        *status = SCMI_STATUS_SUCCESS;
        break;
    default:
        *status = SCMI_STATUS_NOT_FOUND;
        break;
    }
}
