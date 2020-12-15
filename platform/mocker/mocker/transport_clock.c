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

#include <clock_protocol.h>
#include <clock_common.h>
#include <pal_clock_expected.h>

struct arm_scmi_clock_protocol clock_protocol;
static unsigned int clock_status[MAX_NUM_CLOCK];
struct arm_scmi_clock_rate clock_rate[MAX_NUM_CLOCK];

void fill_clock_protocol()
{
    clock_protocol.protocol_version = CLOCK_VERSION;
    clock_protocol.number_clocks = num_clocks;
    clock_protocol.max_num_pending_async_rate_chg_supported =
            max_num_pending_async_rate_chg_supported;
    clock_protocol.num_rates = num_of_clock_rates;
}

void clock_send_message(uint32_t message_id, uint32_t parameter_count,
        const uint32_t *parameters, int32_t *status,
        size_t *return_values_count, uint32_t *return_values)
{

    uint32_t parameter_idx, return_idx, clock_id;
    char * str;
    int i, j;

    switch(message_id)
    {
    case CLK_PROTO_VER_MSG_ID:
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 1;
        return_values[0] = clock_protocol.protocol_version;
        break;
    case CLK_PROTO_ATTRIB_MSG_ID:
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 1;
        return_idx = OFFSET_RET(struct arm_scmi_clock_protocol_attributes,
                attributes);
        return_values[return_idx] = (clock_protocol.number_clocks << CLK_NUMBER_CLOCKS_LOW) |
                        (clock_protocol.max_num_pending_async_rate_chg_supported <<
                        CLK_MAX_NUM_PENDING_ASYNC_RATE_CHGS_SPRT_LOW);
        break;
    case CLK_PROTO_MSG_ATTRIB_MSG_ID:
        parameter_idx = OFFSET_PARAM(
                struct arm_scmi_clock_protocol_message_attributes,
                message_id);
        if(parameters[parameter_idx] > CLK_CONFIG_SET_MSG_ID)
        {
            *status = SCMI_STATUS_NOT_FOUND;
            return;
        }
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 1;
        return_values[OFFSET_RET(
                struct arm_scmi_clock_protocol_message_attributes,
                attributes)] = 0x0;
        break;
    case CLK_ATTRIB_MSG_ID:
        parameter_idx = OFFSET_PARAM(struct arm_scmi_clock_attributes,
                clock_id);
        clock_id = parameters[parameter_idx];
        if (clock_id >= clock_protocol.number_clocks) {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 6;
        return_idx = OFFSET_RET(struct arm_scmi_clock_attributes, attributes);
        return_values[return_idx] = clock_status[clock_id] & 0x1;
        str = (char *)
                (&return_values[OFFSET_RET(
                        struct arm_scmi_clock_attributes,
                        clock_name)]);
        sprintf(str, "CLOCK_%d", parameters[0]);
        break;
    case CLK_DESC_RATE_MSG_ID:
        clock_id = parameters[OFFSET_PARAM(struct arm_scmi_clock_describe_rates,
                   clock_id)];
        if (clock_id >= clock_protocol.number_clocks) {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        if (parameters[OFFSET_PARAM(struct arm_scmi_clock_describe_rates,
                rate_index)] >= clock_protocol.num_rates[clock_id])
        {
            *status = SCMI_STATUS_OUT_OF_RANGE;
            break;
        }
        *status = SCMI_STATUS_SUCCESS;
        j = 100;
        return_values[OFFSET_RET(
                struct arm_scmi_clock_describe_rates,
                num_rates_flags)] = (clock_protocol.num_rates[clock_id] <<
                        CLK_DESC_RATE_NUM_RATES_RETURNED_LOW);
        for (i = 0; i < clock_protocol.num_rates[clock_id]; ++i) {
            return_values[OFFSET_RET(
                    struct arm_scmi_clock_describe_rates,
                    rates) + 2 * i] = j * (i + 1);
            return_values[OFFSET_RET(
                    struct arm_scmi_clock_describe_rates,
                    rates) + 2 * i + 1] = j * (i + 1) + 50;
        }
        break;
    case CLK_RATE_SET_MSG_ID:
        clock_id = parameters[OFFSET_PARAM(struct arm_scmi_clock_rate_set, clock_id)];
        if (clock_id >= clock_protocol.number_clocks) {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        j = 100;
        for (i = 0; i < clock_protocol.num_rates[clock_id]; ++i) {
            if ((parameters[OFFSET_PARAM(struct arm_scmi_clock_rate_set, rate)] ==
                (j * (i + 1))) &&
                (parameters[OFFSET_PARAM(struct arm_scmi_clock_rate_set, rate) + 1] ==
                (j * (i + 1) + 50)))
                break;
        }
        if(i == clock_protocol.num_rates[clock_id]) {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
            break;
        }
        clock_rate[clock_id].lower = parameters[OFFSET_PARAM(
                                     struct arm_scmi_clock_rate_set, rate)];
        clock_rate[clock_id].upper = parameters[OFFSET_PARAM(
                                     struct arm_scmi_clock_rate_set, rate) + 1];
        *status = SCMI_STATUS_SUCCESS;
        break;
    case CLK_RATE_GET_MSG_ID:
        clock_id = parameters[OFFSET_PARAM(struct arm_scmi_clock_rate_get, clock_id)];
        if (clock_id >= clock_protocol.number_clocks) {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        *status = SCMI_STATUS_SUCCESS;
        if (clock_rate[clock_id].lower == 0) {
            j = 100;
            return_values[OFFSET_RET(
                    struct arm_scmi_clock_rate_get,
                    rate)] = j * (clock_id + 1);
            return_values[OFFSET_RET(
                    struct arm_scmi_clock_rate_get,
                    rate) + 1] = j * (clock_id + 1) + 50;
        }
        else {
            return_values[OFFSET_RET(
                    struct arm_scmi_clock_rate_get,
                    rate)] = clock_rate[clock_id].lower;
            return_values[OFFSET_RET(
                    struct arm_scmi_clock_rate_get,
                    rate) + 1] = clock_rate[clock_id].upper;
        }
        break;
    case CLK_CONFIG_SET_MSG_ID:
        parameter_idx = OFFSET_PARAM(struct arm_scmi_clock_config_set, clock_id);
        clock_id = parameters[parameter_idx];
        if (clock_id >= clock_protocol.number_clocks)
        {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        parameter_idx = OFFSET_PARAM(struct arm_scmi_clock_config_set, attributes);
        if (parameters[parameter_idx] > 1)
        {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
            break;
        }
        *status = SCMI_STATUS_SUCCESS;
        clock_status[clock_id] = parameters[parameter_idx];
        break;
    default:
        *status = SCMI_STATUS_NOT_FOUND;
    }
}
