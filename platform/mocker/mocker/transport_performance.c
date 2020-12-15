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

#include <performance_protocol.h>
#include <performance_common.h>
#include <pal_performance_expected.h>

struct arm_scmi_performance_protocol performance_protocol;

static unsigned int perf_level_max_limit[MAX_PERFORMANCE_DOMAIN_COUNT];
static unsigned int perf_level_min_limit[MAX_PERFORMANCE_DOMAIN_COUNT];
static unsigned int perf_level_current[MAX_PERFORMANCE_DOMAIN_COUNT];

void fill_performance_protocol()
{
    performance_protocol.protocol_version = PERFORMANCE_VERSION;
    performance_protocol.num_performance_domains = num_performance_domains;
    performance_protocol.statistics_address_low = statistics_address_low_perf;
    performance_protocol.statistics_length = statistics_address_len_perf;
    performance_protocol.statistics_address_high =
    performance_protocol.statistics_address_low + performance_protocol.statistics_length;
    performance_protocol.set_limit_capable = set_limit_capable;
    performance_protocol.set_performance_level_capable = set_performance_level_capable;
    performance_protocol.performance_level_notification_support =
            performance_level_notification_support;
    performance_protocol.performance_limit_notification_support =
            performance_limit_notification_support;
    performance_protocol.performance_rate_limit = performance_rate_limit;
    performance_protocol.performance_sustained_freq = performance_sustained_freq;
    performance_protocol.performance_sustained_level = performance_sustained_level;
    performance_protocol.performance_domain_names = performance_domain_names;
    performance_protocol.num_performance_levels = perf_num_performance_levels;
    performance_protocol.performance_level_power_cost = perf_performance_level_power_costs;
    performance_protocol.performance_level_value = perf_performance_level_values;
    performance_protocol.performance_level_worst_latency = perf_performance_level_worst_latency;
    performance_protocol.performance_domain_fast_channel_support = perf_domain_fast_ch_support;
    performance_protocol.performance_message_fast_channel_support = perf_msg_fast_ch_support;
    performance_protocol.door_bell_support = perf_performance_fast_ch_doorbell_support;
    performance_protocol.door_bell_data_width = perf_performance_fast_ch_doorbell_data_width;
    performance_protocol.fast_channel_rate_limit = perf_performance_fast_chan_rate_limit;
    performance_protocol.fast_chan_addr_low = perf_performance_fast_chan_addr_low;
    performance_protocol.fast_chan_addr_high = perf_performance_fast_chan_addr_high;
    performance_protocol.fast_chan_size = perf_performance_fast_chan_size;
    performance_protocol.doorbell_addr_low = perf_performance_fast_chan_doorbell_addr_low;
    performance_protocol.doorbell_addr_high = perf_performance_fast_chan_doorbell_addr_high;
    performance_protocol.doorbell_set_mask_low =
            perf_performance_fast_chan_doorbell_set_mask_low;
    performance_protocol.doorbell_set_mask_high =
            perf_performance_fast_chan_doorbell_set_mask_high;
    performance_protocol.doorbell_preserve_mask_low =
            perf_performance_fast_chan_doorbell_preserve_mask_low;
    performance_protocol.doorbell_preserve_mask_high =
            perf_performance_fast_chan_doorbell_preserve_mask_high;
}

void performance_send_message(uint32_t message_id, uint32_t parameter_count,
        const uint32_t *parameters, int32_t *status,
        size_t *return_values_count, uint32_t *return_values)
{

    uint32_t parameter_idx, return_idx;
    char * str;
    int i, domain_id, messageid;

    switch(message_id)
    {
    case PERF_MGMT_PROTO_VER_MSG_ID:
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 1;
        return_values[0] = performance_protocol.protocol_version;
        break;
    case PERF_MGMT_PROTO_ATTRIB_MSG_ID:
        *status = SCMI_STATUS_SUCCESS;

        return_idx = OFFSET_RET(struct arm_scmi_performance_protocol_attributes,
                attributes);
        return_values[return_idx] = (1 << PERFORMANCE_PROT_POWER_VALUES_LOW) +
                (performance_protocol.num_performance_domains <<
                    PERFORMANCE_PROT_ATTR_NUMBER_DOMAINS_LOW);

        return_idx = OFFSET_RET(struct arm_scmi_performance_protocol_attributes,
                statistics_address_low);
        return_values[return_idx] = performance_protocol.statistics_address_low;

        return_idx = OFFSET_RET(struct arm_scmi_performance_protocol_attributes,
                statistics_address_high);
        return_values[return_idx] = performance_protocol.statistics_address_high;

        return_idx = OFFSET_RET(struct arm_scmi_performance_protocol_attributes,
                statistics_len);
        return_values[return_idx] = performance_protocol.statistics_address_high -
                               performance_protocol.statistics_address_low;
        *return_values_count = 4;
        break;
    case PERF_MGMT_PROTO_MSG_ATTRIB_MSG_ID:
        parameter_idx = OFFSET_PARAM(
            struct arm_scmi_performance_protocol_message_attributes, message_id);
        if(parameters[parameter_idx] > PERF_DESCRIBE_FASTCHANNEL) {
            *status = SCMI_STATUS_NOT_FOUND;
        } else {
            *status = SCMI_STATUS_SUCCESS;
            *return_values_count = 1;
            return_values[OFFSET_RET(
                struct arm_scmi_performance_protocol_message_attributes,
                 attributes)] =
                     performance_protocol.
                     performance_message_fast_channel_support
                     [parameters[parameter_idx]]
                  << PERFORMANCE_PROT_MSG_ATTR_FASTCHANNEL_ENABLED;
            *return_values_count = 1;
        }
        break;
    case PERF_DOMAIN_ATTRIB_MSG_ID:
        *status = (parameters[OFFSET_PARAM(
                struct arm_scmi_performance_domain_attributes,
                domain_id)] >= performance_protocol.num_performance_domains ?
                SCMI_STATUS_NOT_FOUND: SCMI_STATUS_SUCCESS);
        domain_id = parameters[OFFSET_PARAM(
                    struct arm_scmi_performance_domain_attributes, domain_id)];
        return_values[OFFSET_RET(
                struct arm_scmi_performance_domain_attributes, attributes)] =
                (performance_protocol.set_limit_capable[domain_id] <<
                     PERFORMANCE_DOMAIN_ATTR_LIMIT_SET) |
                (performance_protocol.set_performance_level_capable[domain_id] <<
                     PERFORMANCE_DOMAIN_ATTR_LEVEL_SET) |
                (performance_protocol.performance_limit_notification_support[domain_id] <<
                     PERFORMANCE_DOMAIN_ATTR_LIMIT_NOTIFY) |
                (performance_protocol.performance_level_notification_support[domain_id] <<
                     PERFORMANCE_DOMAIN_ATTR_LEVEL_NOTIFY) |
                (performance_protocol.performance_domain_fast_channel_support[domain_id] <<
                     PERFORMANCE_DOMAIN_ATTR_FASTCHANNEL_SUPPORT);
        return_values[OFFSET_RET(
                struct arm_scmi_performance_domain_attributes, rate_limit)] =
                     (performance_protocol.performance_rate_limit[domain_id] <<
                      PERFORMANCE_DOMAIN_ATTR_RATE_LIMIT_LOW);
        return_values[OFFSET_RET(
                struct arm_scmi_performance_domain_attributes, sustained_perf_level)] =
                       (performance_protocol.performance_sustained_level[domain_id]);
        return_values[OFFSET_RET(
                struct arm_scmi_performance_domain_attributes,sustained_freq)] =
                       (performance_protocol.performance_sustained_freq[domain_id]);
        str = (char *)
              (&return_values[OFFSET_RET(struct arm_scmi_performance_domain_attributes, name)]);
        sprintf(str, "Domain_%d", domain_id);
        *return_values_count = 5;
        break;
    case PERF_DESC_LVL_MSG_ID:
        domain_id = parameters[OFFSET_PARAM(struct arm_scmi_performance_describe_levels,domain_id)];
        if(domain_id >= performance_protocol.num_performance_domains)
        {
            *status = SCMI_STATUS_NOT_FOUND;
            return;
        }
        if(parameters[OFFSET_PARAM(
                struct arm_scmi_performance_describe_levels,level_index)] >=
            performance_protocol.num_performance_levels[domain_id])
        {
             *status = SCMI_STATUS_INVALID_PARAMETERS;
              return;
        }
        *status = SCMI_STATUS_SUCCESS;
        return_values[OFFSET_RET(struct arm_scmi_performance_describe_levels,num_levels)] =
            performance_protocol.num_performance_levels[domain_id] << PERFORMANCE_DESC_LEVELS_NUM_LEVELS_LOW;
        *return_values_count = 1;
        for(i = 0; i < performance_protocol.num_performance_levels[domain_id]; i++)
        {
            return_values[OFFSET_RET(struct arm_scmi_performance_describe_levels,perf_levels)+i*3] =
                  performance_protocol.performance_level_value[domain_id][i];
            return_values[OFFSET_RET(struct arm_scmi_performance_describe_levels,perf_levels)+i*3+1] =
                  performance_protocol.performance_level_power_cost[domain_id][i];
            return_values[OFFSET_RET(struct arm_scmi_performance_describe_levels,perf_levels)+i*3+2] =
                  performance_protocol.performance_level_worst_latency[domain_id][i];
            *return_values_count += 3;
        }
        break;
    case PERF_LIMIT_SET_MSG_ID:
        domain_id = parameters[OFFSET_PARAM(struct arm_scmi_performance_limits_set,domain_id)];
        if(domain_id >= performance_protocol.num_performance_domains)
        {
            *status = SCMI_STATUS_NOT_FOUND;
             return;
        }
        if(performance_protocol.set_limit_capable[domain_id] == 0)
        {
            *status = SCMI_STATUS_DENIED;
             return;
        }
        if((parameters[OFFSET_PARAM(struct arm_scmi_performance_limits_set,range_max)] >
            performance_protocol.performance_level_value[domain_id]
            [performance_protocol.num_performance_levels[domain_id] - 1]) ||
            (parameters[OFFSET_PARAM(struct arm_scmi_performance_limits_set,range_min)] <
            performance_protocol.performance_level_value[domain_id][0]))
        {
            *status = SCMI_STATUS_OUT_OF_RANGE;
             return;
        }
        if((parameters[OFFSET_PARAM(struct arm_scmi_performance_limits_set,range_max)]) <
            (parameters[OFFSET_PARAM(struct arm_scmi_performance_limits_set,range_min)]))
        {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
             return;
        }
        *status = SCMI_STATUS_SUCCESS;
        perf_level_max_limit[domain_id] =
             parameters[OFFSET_PARAM(struct arm_scmi_performance_limits_set,range_max)];
        perf_level_min_limit[domain_id] =
             parameters[OFFSET_PARAM(struct arm_scmi_performance_limits_set,range_min)] ;
        break;
    case PERF_LIMIT_GET_MSG_ID:
        domain_id = parameters[OFFSET_PARAM(struct arm_scmi_performance_limits_get,domain_id)];
        if(domain_id >= performance_protocol.num_performance_domains)
        {
            *status = SCMI_STATUS_NOT_FOUND;
             return;
        }
        *status = SCMI_STATUS_SUCCESS;
        return_values[OFFSET_RET(struct arm_scmi_performance_limits_get,range_max)] =
             perf_level_max_limit[domain_id];
        return_values[OFFSET_RET(struct arm_scmi_performance_limits_get,range_min)] =
             perf_level_min_limit[domain_id];
        if( return_values[OFFSET_RET(struct arm_scmi_performance_limits_get,range_max)] == 0 &&
                return_values[OFFSET_RET(struct arm_scmi_performance_limits_get,range_min)] == 0)
        {
            return_values[OFFSET_RET(struct arm_scmi_performance_limits_get,range_max)] =
                    performance_protocol.performance_level_value[domain_id]
                    [performance_protocol.num_performance_levels[domain_id] - 1];

            return_values[OFFSET_RET(struct arm_scmi_performance_limits_get,range_min)] =
                     performance_protocol.performance_level_value[domain_id][0];
        }
        *return_values_count = 2;
        break;
    case PERF_LVL_SET_MSG_ID:
        domain_id = parameters[OFFSET_PARAM(struct arm_scmi_performance_levels_set,domain_id)];
        if (domain_id >= performance_protocol.num_performance_domains) {
            *status = SCMI_STATUS_NOT_FOUND;
             return;
        }
        if(performance_protocol.set_performance_level_capable[domain_id] == 0)
        {
            *status = SCMI_STATUS_DENIED;
             return;
        }
        if((perf_level_max_limit[domain_id] <
            parameters[OFFSET_PARAM(struct arm_scmi_performance_levels_set,performance_level)])||
            (perf_level_min_limit[domain_id] >
            parameters[OFFSET_PARAM(struct arm_scmi_performance_levels_set,performance_level)]))
        {
            *status = SCMI_STATUS_OUT_OF_RANGE;
             return;
        }
        *status = SCMI_STATUS_SUCCESS;
        perf_level_current[domain_id] =
            parameters[OFFSET_PARAM(struct arm_scmi_performance_levels_set,performance_level)];
        break;
    case PERF_LVL_GET_MSG_ID:
        domain_id = parameters[OFFSET_PARAM(struct arm_scmi_performance_levels_get,domain_id)];
        if (domain_id >= performance_protocol.num_performance_domains) {
            *status = SCMI_STATUS_NOT_FOUND;
             return;
        }
        *status = SCMI_STATUS_SUCCESS;
        if(perf_level_current[domain_id] == 0)
            return_values[OFFSET_RET(struct arm_scmi_performance_levels_get,performance_level)] =
                performance_protocol.performance_level_value[domain_id][0];
        else
            return_values[OFFSET_RET(struct arm_scmi_performance_levels_get,performance_level)] =
                    perf_level_current[domain_id];
        *return_values_count = 1;
        break;
    case PERF_NOTIFY_LIMIT_MSG_ID :
        domain_id = parameters[OFFSET_PARAM(struct arm_scmi_performance_notify_limits,domain_id)];
        if(domain_id >= performance_protocol.num_performance_domains)
        {
            *status = SCMI_STATUS_NOT_FOUND;
             return;
        }
        if(performance_protocol.performance_limit_notification_support[domain_id] == 0)
        {
            *status = SCMI_STATUS_NOT_SUPPORTED;
             return;
        }
        if((parameters[OFFSET_PARAM(struct arm_scmi_performance_notify_limits,notify_enable)]) >>
            PERFORMANCE_NOTIFY_LIMIT_RESERVED_LOW) /* Check reserved bits*/
        {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
             return;
        }
        *status = SCMI_STATUS_SUCCESS;
        break;
    case PERF_NOTIFY_LVL_MSG_ID:
        domain_id = parameters[OFFSET_PARAM(struct arm_scmi_performance_notify_level,domain_id)];
        if(domain_id >= performance_protocol.num_performance_domains)
        {
            *status = SCMI_STATUS_NOT_FOUND;
             return;
        }
        if(performance_protocol.performance_level_notification_support[domain_id] == 0)
        {
            *status = SCMI_STATUS_NOT_SUPPORTED;
             return;
        }
        if((parameters[OFFSET_PARAM(struct arm_scmi_performance_notify_level,notify_enable)]) >>
            PERFORMANCE_NOTIFY_LEVEL_RESERVED_LOW) /* Check reserved bits*/
        {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
             return;
        }
        *status = SCMI_STATUS_SUCCESS;
        break;
    case PERF_DESCRIBE_FASTCHANNEL:
        parameter_idx = OFFSET_PARAM(struct arm_scmi_performance_describe_fast_channel, domain_id);
        if(parameters[parameter_idx] > performance_protocol.num_performance_domains) {
            *status = SCMI_STATUS_NOT_FOUND;
            return;
        }
        domain_id = parameters[parameter_idx];
        parameter_idx = OFFSET_PARAM(struct arm_scmi_performance_describe_fast_channel, message_id);
        if(parameters[parameter_idx] > PERF_DESCRIBE_FASTCHANNEL) {
            *status = SCMI_STATUS_NOT_FOUND;
            return;
        }
        messageid = parameters[parameter_idx];
        if(performance_protocol.performance_domain_fast_channel_support[domain_id] == 0)
        {
            *status = SCMI_STATUS_NOT_SUPPORTED;
            return;
        }
        if(performance_protocol.performance_message_fast_channel_support[messageid] == 0)
        {
            *status = SCMI_STATUS_NOT_SUPPORTED;
            return;
        }
        if(messageid < PERF_LIMIT_SET_MSG_ID || messageid > PERF_LVL_GET_MSG_ID)
        {
            *status = SCMI_STATUS_NOT_SUPPORTED;
            return;
        }
        *status = SCMI_STATUS_SUCCESS;
        return_values[OFFSET_RET(struct arm_scmi_performance_describe_fast_channel,attributes)] =
            (((performance_protocol.door_bell_support[domain_id]
                                                     [messageid - PERF_LIMIT_SET_MSG_ID])
             << PERFORMANCE_DESCRIBE_FAST_CH_ATTR_DOORBELL_SUPPORT) +
            ((performance_protocol.door_bell_data_width[domain_id]
                                                       [messageid - PERF_LIMIT_SET_MSG_ID])
             << PERFORMANCE_DESCRIBE_FAST_CH_ATTR_DOORBELL_DATA_WIDTH_LOW));
        return_values[OFFSET_RET(struct arm_scmi_performance_describe_fast_channel,rate_limit)] =
             performance_protocol.fast_channel_rate_limit[domain_id]
                                                         [messageid - PERF_LIMIT_SET_MSG_ID];
        return_values[OFFSET_RET(
                struct arm_scmi_performance_describe_fast_channel,chan_addr_low)] =
             performance_protocol.fast_chan_addr_low[domain_id]
                                                    [messageid - PERF_LIMIT_SET_MSG_ID];
        return_values[OFFSET_RET(
                struct arm_scmi_performance_describe_fast_channel,chan_addr_high)] =
             performance_protocol.fast_chan_addr_high[domain_id]
                                                     [messageid - PERF_LIMIT_SET_MSG_ID];
        return_values[OFFSET_RET(
                struct arm_scmi_performance_describe_fast_channel,chan_size)] =
             performance_protocol.fast_chan_size[domain_id]
                                                [messageid - PERF_LIMIT_SET_MSG_ID];
        return_values[OFFSET_RET(
                struct arm_scmi_performance_describe_fast_channel,doorbell_addr_low)] =
             performance_protocol.doorbell_addr_low[domain_id]
                                                   [messageid - PERF_LIMIT_SET_MSG_ID];
        return_values[OFFSET_RET(
                struct arm_scmi_performance_describe_fast_channel,doorbell_addr_high)] =
             performance_protocol.doorbell_addr_high[domain_id]
                                                    [messageid - PERF_LIMIT_SET_MSG_ID];
        return_values[OFFSET_RET(
                struct arm_scmi_performance_describe_fast_channel,doorbell_set_mask_low)] =
             performance_protocol.doorbell_set_mask_low[domain_id]
                                                    [messageid - PERF_LIMIT_SET_MSG_ID];
        return_values[OFFSET_RET(
                struct arm_scmi_performance_describe_fast_channel,doorbell_set_mask_high)] =
             performance_protocol.doorbell_set_mask_high[domain_id]
                                                        [messageid - PERF_LIMIT_SET_MSG_ID];
        return_values[OFFSET_RET(
                struct arm_scmi_performance_describe_fast_channel,doorbell_preserve_mask_low)] =
             performance_protocol.doorbell_preserve_mask_low[domain_id]
                                                            [messageid - PERF_LIMIT_SET_MSG_ID];
        return_values[OFFSET_RET(
                struct arm_scmi_performance_describe_fast_channel,doorbell_preserve_mask_high)] =
             performance_protocol.doorbell_preserve_mask_high[domain_id]
                                                             [messageid - PERF_LIMIT_SET_MSG_ID];
        *return_values_count = 11;
        break;
    default:
        *status = SCMI_STATUS_NOT_FOUND;
        break;
    }
}
