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

#include <protocol_common.h>
#include <base_common.h>
#include <base_protocol.h>
#include <power_domain_common.h>
#include <power_domain_protocol.h>
#include <system_power_common.h>
#include <system_power_protocol.h>
#include <clock_common.h>
#include <clock_protocol.h>
#include <sensor_common.h>
#include <sensor_protocol.h>
#include <performance_common.h>
#include <performance_protocol.h>

extern struct arm_scmi_base_protocol base_protocol;
extern struct arm_scmi_power_protocol power_protocol;
extern struct arm_scmi_sensor_protocol sensor_protocol;
extern struct arm_scmi_performance_protocol performance_protocol;
extern struct arm_scmi_system_power_protocol system_power_protocol;
extern struct arm_scmi_clock_protocol clock_protocol;
extern uint32_t get_unsupported_protocol();

#define SCMI_AGENT_ID_MAX 0xFF

static char* devices[] =
{
    "DEVICE 1", "DEVICE 2"
};

static struct device_access_info agent1_device_access[] =
{
    {"DEVICE_1", true, 1},
    {"DEVICE_2", true, 3}
};

static struct device_access_info  agent2_device_access[] =
{
    {"DEVICE_2", true, 2}
};

static char *agents[] = {
    "OSPM", "PSCI"
};
/*
static struct device_protocol_access_info device2_protocol_access[] =
{
    {BASE_PROTOCOL_ID, true}
};

static struct device_protocol_access_info device1_protocol_access[] =
{
    {BASE_PROTOCOL_ID, true},
    {PWR_DOMAIN_PROTOCOL_ID, true}
};
*/

uint32_t agent_get_accessible_device(uint32_t agent_id)
{
    switch (agent_id)
    {
        case 0:
            return agent1_device_access[0].device_id;
        case 1:
            return agent2_device_access[0].device_id;
        default:
            printf("\n\tInvalid Agent id--->%-1x", agent_id);
            return SCMI_AGENT_ID_MAX;
    }
}

uint32_t agent_get_inaccessible_device(uint32_t agent_id)
{
    switch (agent_id)
    {
        case 0:
            return agent1_device_access[NUM_ELEMS(agent1_device_access)].device_id + 1;
        case 1:
            return agent2_device_access[NUM_ELEMS(agent2_device_access)].device_id + 1;
        default:
            printf("\n\tInvalid Agent id--->%-1x", agent_id);
            return SCMI_AGENT_ID_MAX;
    }
}

uint32_t device_get_accessible_protocol(uint32_t device_id)
{
    return PWR_PROTO_ID;
}

char *agent_name_get(uint32_t agent_id)
{
    if (agent_id == 0)
        return agents[0];
    if (agent_id > NUM_ELEMS(agents))
        return NULL;
    return agents[agent_id - 1];
}

/*!
 * @brief Get the bits from a range of positions within an integer of type
 * uint32_t.
 *
 *  This function is used for extracting the data bits from an upper and lower
 *  range from an integer of type uint32_t. The bits are then shifted to the
 *  right to return the normalised value.
 *
 *  @param data Integer of type uin32_t from which the bits are extracted.
 *  @param bit_upper Upper bit position.
 *  @param bit_lower Lower bit position.
 *
 *  @return The normalised unsigned value from the bits range.
 *
 */
uint32_t arm_scmi_get_norm_bits(uint32_t data, uint8_t bit_upper, uint8_t bit_lower)
{
    uint32_t mask;
    uint32_t bits;
    mask = ((1 << (bit_upper - bit_lower + 1)) - 1) << bit_lower;
    bits = (data & mask) >> bit_lower;
    return bits;
}

/**
 * @brief Interface function that sends a
 * command and receives a platform
 * message.
 *
 * This is the mocker platform implementation of the send_message interface
 * which is used by the test_agent to send commands to the platform over A2P
 * channel. In this case it fakes some dummy data and returns that data to the
 * calling test_agent to enable testing of the test harness.
 *
 **/
void mocker_send_message(uint32_t message_header_send, uint32_t parameter_count,
        const uint32_t *parameters, uint32_t *message_header_rcv, int32_t *status,
        uint32_t *return_values_count, uint32_t *return_values)
{
    char *str;
    int agent_id, device_id, protocol_id, flags, command_id;
    int message_id;
    int parameter_id;

    /*power domainprotocol*/
    int num_power_domains = 3;
    int stats_low = 0x1234, stats_high = stats_low + 0xff;
    /*system power management protocol*/
    /*Mocker platform supports warm reset and system suspend
     * 0 for not supporting
     * 1 for supporting
     * */
    int warm_reset_support = 1, system_suspend_support = 1;

    *message_header_rcv = message_header_send;
    protocol_id = arm_scmi_get_norm_bits(message_header_send,
            PROTOCOL_ID_HIGH, PROTOCOL_ID_LOW);
    message_id = arm_scmi_get_norm_bits(message_header_send,
            MESSAGE_ID_HIGH, MESSAGE_ID_LOW);

    switch (protocol_id)
    {
        case BASE_PROTO_ID:
            switch (message_id)
            {
                case BASE_PROTO_VERSION_MSG_ID:
                    *status = SCMI_STATUS_SUCCESS;
                    *return_values_count = 1;
                    return_values[OFFSET_RET(struct arm_scmi_base_protocol_version, version)] =
                            base_protocol.protocol_version;
                    break;
                case BASE_PROTO_ATTR_MSG_ID:
                    *status = SCMI_STATUS_SUCCESS;
                    *return_values_count = 1;
                    return_values[OFFSET_RET(struct arm_scmi_base_protocol_attributes, attributes)]
                        = (base_protocol.num_agents << 8) + base_protocol.num_protocols_implemented;
                    break;
                case BASE_PROTO_MSG_ATTR_MSG_ID:
                    parameter_id = OFFSET_PARAM(
                        struct arm_scmi_base_protocol_message_attributes, message_id);
                    if(parameters[parameter_id] > BASE_RESET_AGENT_CONFIGURATION_MSG_ID) {
                        *status = SCMI_STATUS_NOT_FOUND;
                    } else if (parameters[parameter_id] > BASE_DISC_AGENT_MSG_ID)
                            {
                                *status = SCMI_STATUS_NOT_FOUND;
                                switch (parameters[parameter_id])
                                {
                                case 0x8:
                                   if (base_protocol.notify_error_cmd_supported == true)
                                        *status = SCMI_STATUS_SUCCESS;
                                    break;
                                case 0x9:
                                   if (base_protocol.device_permissions_cmd_supported == true)
                                        *status = SCMI_STATUS_SUCCESS;
                                    break;
                                 case 0xA:
                                   if (base_protocol.protocol_permissions_cmd_supported == true)
                                        *status = SCMI_STATUS_SUCCESS;
                                    break;
                                 case 0xB:
                                   if (base_protocol.reset_agent_config_cmd_supported == true)
                                        *status = SCMI_STATUS_SUCCESS;
                                    break;
                                case 0x7:
                                   if (base_protocol.discover_agent_cmd_supported == true)
                                        *status = SCMI_STATUS_SUCCESS;
                                    break;
                                default:
                                        *status = SCMI_STATUS_SUCCESS;

                                }
                            }
                            else {
                                *status = SCMI_STATUS_SUCCESS;
                                *return_values_count = 1;
                                return_values[OFFSET_RET(struct arm_scmi_base_protocol_message_attributes,
                                attributes)] = 0x0;
                            }
                    break;
                case BASE_DISC_VENDOR_MSG_ID:
                    *status = SCMI_STATUS_SUCCESS;
                    sprintf((char *)return_values, "%s", base_protocol.vendor_identifier);
                    *return_values_count = strlen((const char*)return_values);
                    break;
                case BASE_DISC_SUBVENDOR_MSG_ID:
                    if(base_protocol.subvendor_cmd_supported == false)
                    {
                        *status = SCMI_STATUS_NOT_FOUND;
                        break;
                    }
                    *status = SCMI_STATUS_SUCCESS;
                    sprintf((char *)return_values, "%s", base_protocol.subvendor_identifier);
                    *return_values_count = strlen((const char*)return_values);
                    break;
                case BASE_DISC_IMPLEMENTATION_VERSION_MSG_ID:
                    *status = SCMI_STATUS_SUCCESS;
                    *return_values_count = 1;
                    return_values[OFFSET_RET(struct arm_scmi_base_discover_implementation_version,
                        implementation_version)] = base_protocol.implementation_version;
                    break;
                case BASE_DISC_PROTO_LIST_MSG_ID:
                    parameter_id = OFFSET_PARAM(
                        struct arm_scmi_base_discover_list_protocols, skip);
                    if (parameters[parameter_id] > base_protocol.num_protocols_implemented) {
                        *status = SCMI_STATUS_INVALID_PARAMETERS;
                    } else {
                        *status = SCMI_STATUS_SUCCESS;
                        *return_values_count = 2 + (base_protocol.num_protocols_implemented -
                                               parameters[parameter_id]);
                        return_values[0] = base_protocol.num_protocols_implemented -
                                           parameters[parameter_id];
                        return_values[1] = 0x11;
                    }
                    break;
                case BASE_DISC_AGENT_MSG_ID:
                    if(base_protocol.discover_agent_cmd_supported == false)
                    {
                        *status = SCMI_STATUS_NOT_FOUND;
                        break;
                    }
                    parameter_id = OFFSET_PARAM(
                        struct arm_scmi_base_discover_agent, agent_id);
                    if ((parameters[parameter_id] != 0xFFFFFFFF) &&
                         (parameters[parameter_id] > base_protocol.num_agents))
                    {
                        *status = SCMI_STATUS_NOT_FOUND;
                    }
                    else
                    {
                        *status = SCMI_STATUS_SUCCESS;
                        if (parameters[parameter_id] == 0xFFFFFFFF)
                        {
                            return_values[0] = 0;
                            sprintf((char *)&return_values[1], "%s", agent_name_get(0));
                        }
                        else
                        {
                            return_values[0] = parameters[parameter_id];
                            sprintf((char *)&return_values[1], "%s",
                                (parameters[parameter_id] == 0 ?
                                "platform_mock" : agent_name_get(parameters[parameter_id])));
                        }
                        *return_values_count = strlen((const char*)return_values) + 1;
                    }
                    break;
                case BASE_NOTIFY_ERRORS_MSG_ID:
                    if (base_protocol.notify_error_cmd_supported == false)
                    {
                        *status = SCMI_STATUS_NOT_FOUND;
                        break;
                    }
                    parameter_id = OFFSET_PARAM(
                        struct arm_scmi_base_notify_error, notify_enable);
                    if (parameters[parameter_id] > 0x1) {
                        *status = SCMI_STATUS_INVALID_PARAMETERS;
                    }
                    else
                    {
                        base_protocol.notify_error_cmd_supported = (bool)parameters[parameter_id];
                        *status = SCMI_STATUS_SUCCESS;
                    }
                    break;
                case BASE_SET_DEVICE_PERMISSIONS_MSG_ID:
                    if (base_protocol.device_permissions_cmd_supported == false)
                    {
                        *status = SCMI_STATUS_NOT_FOUND;
                        break;
                    }
                    agent_id = OFFSET_PARAM(struct arm_scmi_base_set_device_permissions,
                                agent_id);
                    device_id = OFFSET_PARAM(struct arm_scmi_base_set_device_permissions,
                                device_id);
                    flags = OFFSET_PARAM(struct arm_scmi_base_set_device_permissions, flags);
                    if (parameters[flags] > 0x1)
                        *status = SCMI_STATUS_INVALID_PARAMETERS;
                    else if ((parameters[agent_id] > base_protocol.num_agents) || (parameters[device_id]
                         > NUM_ELEMS(devices)))
                            *status = SCMI_STATUS_NOT_FOUND;
                    switch (agent_id)
                    {
                        case 1:
                            if (parameters[device_id] > NUM_ELEMS(agent1_device_access))
                                *status = SCMI_STATUS_DENIED;
                            agent1_device_access[parameters[device_id]].device_access =
                                parameters[flags];
                                *status = SCMI_STATUS_SUCCESS;
                            break;
                        case 2:
                            if (parameters[device_id] > NUM_ELEMS(agent2_device_access))
                                *status = SCMI_STATUS_DENIED;
                            agent2_device_access[parameters[device_id]].device_access =
                                parameters[flags];
                                *status = SCMI_STATUS_SUCCESS;
                            break;
                    }
                    break;
                case BASE_SET_PROTOCOL_PERMISSIONS_MSG_ID:
                    if (base_protocol.protocol_permissions_cmd_supported == false)
                    {
                        *status = SCMI_STATUS_NOT_FOUND;
                        break;
                    }
                    *status = SCMI_STATUS_SUCCESS;
                    agent_id = OFFSET_PARAM(struct arm_scmi_base_set_protocol_permissions,
                                agent_id);
                    device_id = OFFSET_PARAM(struct arm_scmi_base_set_protocol_permissions,
                                device_id);
                    flags = OFFSET_PARAM(struct arm_scmi_base_set_protocol_permissions, flags);
                    command_id = OFFSET_PARAM(struct arm_scmi_base_set_protocol_permissions,
                                command_id);
                    if (parameters[command_id] == BASE_PROTO_ID)
                        *status = SCMI_STATUS_NOT_FOUND;
                    else if ((parameters[agent_id] > base_protocol.num_agents) || (parameters[device_id]
                         > NUM_ELEMS(devices)) || (parameters[command_id]
                         > get_unsupported_protocol()))
                            *status = SCMI_STATUS_NOT_FOUND;
                    else if (parameters[flags] > 0x1)
                            *status = SCMI_STATUS_INVALID_PARAMETERS;
                    /*
                    switch (agent_id)
                    {
                        case 1:
                            if (command_id > agent1_protocol_access[NUM_ELEMS
                                (agent1_protocol_access)].protocol_id)
                                    *status = SCMI_STATUS_NOT_SUPPORTED;
                            break;
                        case 2:
                            if (command_id > agent2_protocol_access[NUM_ELEMS
                                (agent2_protocol_access)].protocol_id)
                                    *status = SCMI_STATUS_NOT_SUPPORTED;
                            break;
                    }
                    */
                    break;
                case BASE_RESET_AGENT_CONFIGURATION_MSG_ID:
                    if (base_protocol.reset_agent_config_cmd_supported == false)
                    {
                        *status = SCMI_STATUS_NOT_FOUND;
                        break;
                    }
                    *status = SCMI_STATUS_SUCCESS;
                    agent_id = OFFSET_PARAM(struct arm_scmi_base_reset_agent_configuration,
                                agent_id);
                    flags = OFFSET_PARAM(struct arm_scmi_base_reset_agent_configuration, flags);
                    if (parameters[agent_id] > base_protocol.num_agents)
                            *status = SCMI_STATUS_NOT_FOUND;
                    else if (parameters[flags] > 0x1)
                            *status = SCMI_STATUS_INVALID_PARAMETERS;
                    break;
                default:
                    *status = SCMI_STATUS_NOT_FOUND;
            }
            break;
            case PWR_PROTO_ID:
                switch (message_id)
                {
                    case PWR_PROTO_VER_MSG_ID:
                        *status = SCMI_STATUS_SUCCESS;
                        *return_values_count = 1;
                        return_values[0] = power_protocol.protocol_version;
                        break;
                    case PWR_PROTO_ATTR_MSG_ID:
                        *status = SCMI_STATUS_SUCCESS;
                        *return_values_count = 1;
                        return_values[OFFSET_RET(
                            struct arm_scmi_power_protocol_attributes,
                            attributes)] =
                                (num_power_domains <<
                                        POWER_PROT_ATTR_NUMBER_DOMAINS_LOW);
                        return_values[OFFSET_RET(
                            struct arm_scmi_power_protocol_attributes,
                            statistics_address_low)] = stats_low;
                        return_values[OFFSET_RET(
                            struct arm_scmi_power_protocol_attributes,
                            statistics_address_high)] = stats_high;
                        return_values[OFFSET_RET(
                            struct arm_scmi_power_protocol_attributes,
                            statistics_len)] = stats_high - stats_low;
                        break;
                    case PWR_PROTO_MSG_ATTR_MSG_ID:
                        parameter_id = OFFSET_PARAM(
                        struct arm_scmi_power_protocol_message_attributes, message_id);
                        if(parameters[parameter_id] > PWR_STATE_CHANGE_REQUESTED_NOTIFY_MSG_ID) {
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
                        return_values[OFFSET_RET(
                                struct arm_scmi_power_domain_attributes,
                                attributes)] =
                                (1 << POWER_DOMAIN_ATTR_SYNC_LOW) +
                                (1 << POWER_DOMAIN_ATTR_ASYNC_LOW) +
                                (1 << POWER_DOMAIN_ATTR_STATE_NOTIFICATIONS_LOW);
                        str = (char *)
                        (&return_values[OFFSET_RET(
                                struct arm_scmi_power_domain_attributes, name)]);
                        sprintf(str, "Domain_%d", parameters[
                        OFFSET_PARAM(struct arm_scmi_power_domain_attributes,
                                domain_id)]);
                        break;
                    case PWR_STATE_SET_MSG_ID:
                        if ((parameters[OFFSET_PARAM(struct arm_scmi_power_state_set,
                                power_state)] & (1 < 31)) != 0) {
                            *status = SCMI_STATUS_INVALID_PARAMETERS;
                            break;
                        }
                        if (parameters[OFFSET_PARAM(struct arm_scmi_power_state_set,
                                flags)] >= 1) {
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
            break;
            case SYSTEM_POWER_PROTO_ID:
                switch (message_id)
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
                        if (parameters[OFFSET_PARAM(
                                struct arm_scmi_system_power_protocol_message_attributes,
                                message_id)] > SYSTEM_POWER_STATE_NOTIFY_MSG_ID) {
                              *status = SCMI_STATUS_NOT_FOUND;
                                return;
                        }
                        if (parameters[OFFSET_PARAM(
                                struct arm_scmi_system_power_protocol_message_attributes,
                                message_id)] != SYSTEM_POWER_STATE_SET_MSG_ID) {
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
/*
                    case SYSTEM_POWER_STATE_SET_MSG_ID:
                        if ((parameters[OFFSET_PARAM(
                                struct arm_scmi_system_power_state_set,
                                    system_state)] >= 0x5) &&
                            (parameters[OFFSET_PARAM(
                                struct arm_scmi_system_power_state_set,
                                    system_state)] <= 0x7fffffff)) {
                            *status = SCMI_STATUS_INVALID_PARAMETERS;
                        }
                        break;
*/
                    case SYSTEM_POWER_STATE_GET_MSG_ID:
                        *status = SCMI_STATUS_SUCCESS;
                        *return_values_count = 1;
                        return_values[0] = 0x3;
                        break;
                    default:
                        *status = SCMI_STATUS_NOT_FOUND;
                }
            break;
            case PERF_MGMT_PROTO_ID:
                switch (message_id)
                {
                    case PERF_MGMT_PROTO_VER_MSG_ID:
                        *status = SCMI_STATUS_SUCCESS;
                        *return_values_count = 1;
                        return_values[0] = performance_protocol.protocol_version;
                        break;
                    case PERF_MGMT_PROTO_ATTRIB_MSG_ID:
                        *status = SCMI_STATUS_SUCCESS;
                        *return_values_count = 1;
                        return_values[OFFSET_RET(
                            struct arm_scmi_performance_protocol_attributes,
                            attributes)] =
                                (1 << PERFORMANCE_PROT_POWER_VALUES_LOW) +
                                (performance_protocol.num_performance_domains <<
                                    PERFORMANCE_PROT_ATTR_NUMBER_DOMAINS_LOW);
                        return_values[OFFSET_RET(
                            struct arm_scmi_performance_protocol_attributes,
                            statistics_address_low)] = performance_protocol.statistics_address_low;
                        return_values[OFFSET_RET(
                            struct arm_scmi_performance_protocol_attributes,
                            statistics_address_high)] = performance_protocol.statistics_address_high;
                        return_values[OFFSET_RET(
                            struct arm_scmi_performance_protocol_attributes,
                            statistics_len)] = performance_protocol.statistics_address_high -
                                               performance_protocol.statistics_address_low;
                        break;
                    case PERF_MGMT_PROTO_MSG_ATTRIB_MSG_ID:
                        *status = SCMI_STATUS_SUCCESS;
                        *return_values_count = 1;
                        return_values[OFFSET_RET(
                            struct arm_scmi_performance_protocol_message_attributes,
                            attributes)] = 0x0;
                        break;
                    case PERF_DOMAIN_ATTRIB_MSG_ID:
                        *status = (parameters[OFFSET_PARAM(
                                struct arm_scmi_performance_domain_attributes,
                                domain_id)] >= performance_protocol.num_performance_domains ?
                                SCMI_STATUS_NOT_FOUND: SCMI_STATUS_SUCCESS);
                        return_values[OFFSET_RET(
                                struct arm_scmi_performance_domain_attributes,
                                attributes)] =
                                (1 << PERFORMANCE_DOMAIN_ATTR_LIMIT_SET) |
                                (1 << PERFORMANCE_DOMAIN_ATTR_LEVEL_SET);
                        return_values[OFFSET_RET(
                                struct arm_scmi_performance_domain_attributes,
                                rate_limit)] = 0;
                        return_values[OFFSET_RET(
                                struct arm_scmi_performance_domain_attributes,
                                sustained_perf_level)] = (parameters[0] + 1) * 11;
                        return_values[OFFSET_RET(
                                struct arm_scmi_performance_domain_attributes,
                                sustained_freq)] = (parameters[0] + 1) * 10;
                        str = (char *)
                        (&return_values[OFFSET_RET(
                            struct arm_scmi_performance_domain_attributes, name)]);
                        sprintf(str, "Domain_%d", parameters[
                        OFFSET_PARAM(struct arm_scmi_performance_domain_attributes,
                            domain_id)]);
                        break;
                    default:
                        *status = SCMI_STATUS_NOT_FOUND;
                }
            break;
                case CLK_PROTO_ID:
                    switch (message_id)
                    {
                        case CLK_PROTO_VER_MSG_ID:
                            *status = SCMI_STATUS_SUCCESS;
                            *return_values_count = 1;
                            return_values[0] = clock_protocol.protocol_version;
                            break;
                        case CLK_PROTO_ATTRIB_MSG_ID:
                            *status = SCMI_STATUS_SUCCESS;
                            *return_values_count = 1;
                            return_values[OFFSET_RET(
                                struct arm_scmi_clock_protocol_attributes,
                                attributes)] =
                                    (clock_protocol.protocol_version << CLK_NUMBER_CLOCKS_LOW);
                            break;
                        case CLK_PROTO_MSG_ATTRIB_MSG_ID:
                            *status = SCMI_STATUS_SUCCESS;
                            *return_values_count = 1;
                            return_values[OFFSET_RET(
                                struct arm_scmi_clock_protocol_message_attributes,
                                attributes)] = 0x0;
                            break;
/*
                        case CLK_ATTRIB_MSG_ID:
                            if (parameters[OFFSET_PARAM(struct arm_scmi_clock_attributes,
                                    clock_id)] >= NUMBER_CLOCKS) {
                                *status = SCMI_STATUS_NOT_FOUND;
                                break;
                            }
                            *status = SCMI_STATUS_SUCCESS;
                            str = (char *)
                            (&return_values[OFFSET_RET(
                                struct arm_scmi_clock_attributes,
                                clock_name)]);
                            sprintf(str, "CLOCK_%d", parameters[0]);
                            break;
                        case CLK_DESC_RATE_MSG_ID:
                            if (parameters[OFFSET_PARAM(struct arm_scmi_clock_describe_rates,
                                    clock_id)] >= NUMBER_CLOCKS) {
                                *status = SCMI_STATUS_NOT_FOUND;
                                break;
                            }
                            *status = SCMI_STATUS_SUCCESS;
                            j = 100;k=0;
                            return_values[OFFSET_RET(
                            struct arm_scmi_clock_describe_rates,
                            num_rates_flags)] = (clock_rates[parameters[0]] <<
                                    CLK_DESCRATE_NUM_RATES_RETURNED_LOW);
                            for (i = 0; i < clock_rates[parameters[0]]; ++i) {
                                return_values[OFFSET_RET(
                                struct arm_scmi_clock_describe_rates,
                                rates) + 2 * i] = j * (i + 1);
                                return_values[OFFSET_RET(
                                struct arm_scmi_clock_describe_rates,
                                rates) + 2 * i + 1] = j * (i + 1) + 50;
                            }
                            break;
                        case CLK_RATE_SET_MSG_ID:
                            if (parameters[OFFSET_PARAM(
                                struct arm_scmi_clock_rate_set, clock_id)] >=
                                    NUMBER_CLOCKS) {
                                *status = SCMI_STATUS_NOT_FOUND;
                                break;
                            }
                            *status = SCMI_STATUS_SUCCESS;
                            break;
                        case CLK_RATE_GET_MSG_ID:
                            if (parameters[OFFSET_PARAM(
                                struct arm_scmi_clock_rate_get, clock_id)] >=
                                    NUMBER_CLOCKS) {
                                *status = SCMI_STATUS_NOT_FOUND;
                                break;
                            }
                            *status = SCMI_STATUS_SUCCESS;
                            return_values[OFFSET_RET(
                                struct arm_scmi_clock_rate_get,
                                rate)] = 100;
                            return_values[OFFSET_RET(
                                struct arm_scmi_clock_rate_get,
                                rate) + 1] = 150;
                            break;
                        case CLK_CONFIG_SET_MSG_ID:
                            if (parameters[OFFSET_PARAM(
                                struct arm_scmi_clock_config_set, clock_id)] >=
                                    NUMBER_CLOCKS) {
                                *status = SCMI_STATUS_NOT_FOUND;
                                break;
                            }
                            *status = SCMI_STATUS_SUCCESS;
                            break;
*/
                        default:
                            *status = SCMI_STATUS_NOT_FOUND;
                    }
                break;
            case SNSR_PROTO_ID:
                switch(message_id)
                {
                    case SNSR_PROTO_VER_MSG_ID:
                        *status = SCMI_STATUS_SUCCESS;
                        *return_values_count = 1;
                        return_values[0] = sensor_protocol.protocol_version;
                        break;
                    case SNSR_PROTO_ATTRIB_MSG_ID:
                        *status = SCMI_STATUS_SUCCESS;
                        *return_values_count = 4;
                        return_values[OFFSET_RET(
                        struct arm_scmi_sensor_protocol_attributes,
                        attributes)] =
                            (sensor_protocol.number_sensors << SNR_NUMBER_SENSORS_LOW) +
                            (sensor_protocol.max_num_pending_async_cmds_supported <<
                            SNR_MAX_NUM_PENDING_ASYNC_CMDS_SPRT_LOW) +
                            (RESERVED << SNR_RESERVED_LOW);
                        return_values[OFFSET_RET(
                            struct arm_scmi_sensor_protocol_attributes,
                            sensor_reg_address_low)] = sensor_protocol.sensor_reg_address_low;
                        return_values[OFFSET_RET(
                            struct arm_scmi_sensor_protocol_attributes,
                            sensor_reg_address_high)] = sensor_protocol.sensor_reg_address_high;
                        return_values[OFFSET_RET(
                            struct arm_scmi_sensor_protocol_attributes,
                            sensor_reg_len)] = sensor_protocol.sensor_reg_length;
                        break;
                    case SNSR_PROTO_MSG_ATTRIB_MSG_ID:
                        *status = SCMI_STATUS_SUCCESS;
                        *return_values_count = 1;
                        return_values[OFFSET_RET(
                            struct arm_scmi_sensor_protocol_message_attributes,
                            attributes)] = 0x0;
                        break;
                   default:
                        *status = SCMI_STATUS_NOT_FOUND;
                }
            break;
       default:
            printf("\nProtocol: %d\n", message_id);
            assert(!"\nUnknown protocol id\n");
            break;
    }
}
