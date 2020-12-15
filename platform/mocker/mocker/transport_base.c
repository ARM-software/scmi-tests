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

#include <base_protocol.h>
#include <base_common.h>
#include <pal_base_expected.h>

struct arm_scmi_base_protocol base_protocol;
#define SCMI_AGENT_ID_MAX 0xFF
#define TRUSTED_AGENT     1

void fill_base_protocol()
{
    base_protocol.protocol_version = BASE_VERSION;
    base_protocol.num_protocols_implemented = NUM_ELEMS(supported_protocols);
    sprintf(base_protocol.vendor_identifier, "%s", vendor_name);
    sprintf(base_protocol.subvendor_identifier, "%s", subvendor_name);
    base_protocol.implementation_version = implementation_version;
    base_protocol.supported_protocols = (uint32_t *)supported_protocols;
    /* Setting of base protocol optional cmd support */
    base_protocol.notify_error_cmd_supported = true;
    base_protocol.subvendor_cmd_supported = true;
    base_protocol.discover_agent_cmd_supported = true;
    base_protocol.device_permissions_cmd_supported = true;
    base_protocol.protocol_permissions_cmd_supported = true;
    base_protocol.reset_agent_config_cmd_supported = true;
    base_protocol.num_agents = NUM_ELEMS(agents);
}

uint32_t get_unsupported_protocol()
{
    return supported_protocols[NUM_ELEMS(supported_protocols) - 1] + 1;
}

static char* devices[] =
{
    "DEVICE 1", "DEVICE 2"
};

static struct device_access_info agent1_device_access[] =
{
    {"DEVICE_0", true, 0},
    {"DEVICE_1", true, 1}
};

static struct device_protocol_access_info agent_protocol_access[] =
{
    {POWER_DOMAIN_PROTOCOL_ID, true},
};

char *agent_name_get(uint32_t agent_id)
{
    if (agent_id == 0)
        return agents[0];
    if (agent_id > NUM_ELEMS(agents))
        return NULL;
    return agents[agent_id - 1];
}

uint32_t agent_get_accessible_device(uint32_t agent_id)
{
    return agent1_device_access[0].device_id;
}

bool device_access_permitted (void)
{
    return agent1_device_access[0].device_access;
}

bool protocol_access_permitted (uint32_t protocol_id)
{
    int32_t i;

    for(i = 0; i < NUM_ELEMS(agent_protocol_access); i++) {
        if(agent_protocol_access[i].protocol_id == protocol_id)
            return agent_protocol_access[i].protocol_access;
    }
    return false;
}

uint32_t agent_get_inaccessible_device(uint32_t agent_id)
{
    return agent1_device_access[NUM_ELEMS(agent1_device_access)].device_id + 1;

}

uint32_t device_get_accessible_protocol(uint32_t device_id)
{
    return POWER_DOMAIN_PROTOCOL_ID;
}

uint32_t check_trusted_agent(uint32_t agent_id)
{
    return TRUSTED_AGENT;
}

void base_send_message(uint32_t message_id, uint32_t parameter_count,
                   const uint32_t *parameters, int32_t *status,
                   size_t *return_values_count, uint32_t *return_values)
{
    uint32_t parameter_idx, return_idx, i;
    uint32_t agent_id, device_id, flags, command_id;

    switch (message_id)
    {
    case BASE_PROTO_VERSION_MSG_ID:
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 1;
        return_idx = OFFSET_RET(struct arm_scmi_base_protocol_version, version);
        return_values[return_idx] =
                base_protocol.protocol_version;
        break;
    case BASE_PROTO_ATTR_MSG_ID:
        *status = SCMI_STATUS_SUCCESS;
        *return_values_count = 1;
        return_idx = OFFSET_RET(struct arm_scmi_base_protocol_attributes, attributes);
        return_values[return_idx] = (base_protocol.num_agents << 8) +
                base_protocol.num_protocols_implemented;
        break;
    case BASE_PROTO_MSG_ATTR_MSG_ID:
        parameter_idx = OFFSET_PARAM(
                struct arm_scmi_base_protocol_message_attributes, message_id);
        if(parameters[parameter_idx] > BASE_RESET_AGENT_CONFIGURATION_MSG_ID) {
            *status = SCMI_STATUS_NOT_FOUND;
        }
        else if (parameters[parameter_idx] > BASE_DISC_AGENT_MSG_ID)
        {
            *status = SCMI_STATUS_NOT_FOUND;
            switch (parameters[parameter_idx])
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
            return_idx = OFFSET_RET(struct arm_scmi_base_protocol_message_attributes,
                    attributes);
            return_values[return_idx] = 0x0;
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
        return_idx = OFFSET_RET(struct arm_scmi_base_discover_implementation_version,
                implementation_version);
        return_values[return_idx] = base_protocol.implementation_version;
        break;
    case BASE_DISC_PROTO_LIST_MSG_ID:
        parameter_idx = OFFSET_PARAM(
                struct arm_scmi_base_discover_list_protocols, skip);
        if (parameters[parameter_idx] > base_protocol.num_protocols_implemented) {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
        } else {
            *status = SCMI_STATUS_SUCCESS;
            *return_values_count = 2 + ((base_protocol.num_protocols_implemented -
                    parameters[parameter_idx]) + 4)/4;
            return_values[0] = base_protocol.num_protocols_implemented -
                    parameters[parameter_idx];
            memcpy(&return_values[1], &base_protocol.supported_protocols
                    [parameters[parameter_idx]],
                    (base_protocol.num_protocols_implemented -
                            parameters[parameter_idx]));
        }
        break;
    case BASE_DISC_AGENT_MSG_ID:
        if(base_protocol.discover_agent_cmd_supported == false)
        {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        parameter_idx = OFFSET_PARAM(
                struct arm_scmi_base_discover_agent, agent_id);
        if ((parameters[parameter_idx] != 0xFFFFFFFF) &&
                (parameters[parameter_idx] > base_protocol.num_agents))
        {
            *status = SCMI_STATUS_NOT_FOUND;
        }
        else
        {
            *status = SCMI_STATUS_SUCCESS;
            if (parameters[parameter_idx] == 0xFFFFFFFF)
            {
                return_values[0] = 0;
                sprintf((char *)&return_values[1], "%s", agent_name_get(0));
            }
            else
            {
                return_values[0] = parameters[parameter_idx];
                sprintf((char *)&return_values[1], "%s",
                        (parameters[parameter_idx] == 0 ?
                                "platform_mock" : agent_name_get(parameters[parameter_idx])));
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
        parameter_idx = OFFSET_PARAM(
                struct arm_scmi_base_notify_error, notify_enable);
        if (parameters[parameter_idx] > 0x1) {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
        }
        else
        {
            base_protocol.notify_error_cmd_supported = (bool)parameters[parameter_idx];
            *status = SCMI_STATUS_SUCCESS;
        }
        break;
    case BASE_SET_DEVICE_PERMISSIONS_MSG_ID:
        if (base_protocol.device_permissions_cmd_supported == false) {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        parameter_idx = OFFSET_PARAM(struct arm_scmi_base_set_device_permissions,
                        agent_id);
        agent_id = parameters[parameter_idx];
        parameter_idx = OFFSET_PARAM(struct arm_scmi_base_set_device_permissions,
                        device_id);
        device_id = parameters[parameter_idx];
        parameter_idx = OFFSET_PARAM(struct arm_scmi_base_set_device_permissions,
                        flags);
        flags = parameters[parameter_idx];
        if (flags > 0x1) {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
            break;
        }
        if ((agent_id > base_protocol.num_agents) || (device_id > NUM_ELEMS(devices))) {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }

        if (device_id > NUM_ELEMS(agent1_device_access)) {
            *status = SCMI_STATUS_DENIED;
            break;
        }
        agent1_device_access[device_id].device_access = flags;
        *status = SCMI_STATUS_SUCCESS;
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
        if (parameters[command_id] == BASE_PROTO_ID) {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        if ((parameters[agent_id] > base_protocol.num_agents) ||
                (parameters[device_id] > NUM_ELEMS(devices)) ||
                (parameters[command_id] >= get_unsupported_protocol())) {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        if (parameters[flags] > 0x1) {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
            break;
        }
        for(i = 0; i < NUM_ELEMS(agent_protocol_access); i++) {
            if(agent_protocol_access[i].protocol_id == parameters[command_id])
                agent_protocol_access[i].protocol_access = parameters[flags];
        }
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
        if (parameters[agent_id] > base_protocol.num_agents) {
            *status = SCMI_STATUS_NOT_FOUND;
            break;
        }
        if (parameters[flags] > 0x1) {
            *status = SCMI_STATUS_INVALID_PARAMETERS;
            break;
        }
        if(parameters[flags] == 1) {
            agent1_device_access[0].device_access = true;
            for(i = 0; i < NUM_ELEMS(agent_protocol_access); i++) {
                agent_protocol_access[i].protocol_access = true;
            }
        }
        break;
    default:
        *status = SCMI_STATUS_NOT_FOUND;
    }
}
