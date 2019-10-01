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
#include <base_protocol.h>
#include <power_domain_protocol.h>
#include <sensor_protocol.h>
#include <performance_protocol.h>
#include <system_power_protocol.h>
#include <clock_protocol.h>
#include <pal_interface.h>
#include <pal_expected.h>

struct arm_scmi_base_protocol base_protocol;
struct arm_scmi_power_protocol power_protocol;
struct arm_scmi_sensor_protocol sensor_protocol;
struct arm_scmi_performance_protocol performance_protocol;
struct arm_scmi_system_power_protocol system_power_protocol;
struct arm_scmi_clock_protocol clock_protocol;

static uint32_t statistics_address_low = 0x1234;
static uint32_t statistics_address_len = 0xFF;

uint32_t get_unsupported_protocol()
{
    return supported_protocols[NUM_ELEMS(supported_protocols) - 1] + 1;
}

void fill_base_protocol()
{
    base_protocol.protocol_version = BASE_VERSION;
    base_protocol.num_protocols_implemented = NUM_ELEMS(supported_protocols);
    sprintf(base_protocol.vendor_identifier, "%s", vendor_name);
    sprintf(base_protocol.subvendor_identifier, "%s", subvendor_name);
    base_protocol.implementation_version = implementation_version;
    base_protocol.supported_protocols = supported_protocols;
    /* Setting of base protocol optional cmd support */
    base_protocol.notify_error_cmd_supported = true;
    base_protocol.subvendor_cmd_supported = true;
    base_protocol.discover_agent_cmd_supported = true;
    base_protocol.device_permissions_cmd_supported = true;
    base_protocol.protocol_permissions_cmd_supported = true;
    base_protocol.reset_agent_config_cmd_supported = true;
    base_protocol.num_agents = NUM_ELEMS(agents);
}

void fill_power_protocol()
{
    power_protocol.protocol_version = POWER_DOMAIN_VERSION;
    power_protocol.num_power_domains = num_power_domains;
    power_protocol.statistics_address_low = statistics_address_low;
    power_protocol.statistics_length = statistics_address_len;
    power_protocol.statistics_address_high =
        power_protocol.statistics_address_low + power_protocol.statistics_length;
    power_protocol.power_state_notify_cmd_supported = true;
    power_protocol.power_state_change_requested_notify_cmd_supported = true;
}

void fill_sensor_protocol()
{
    sensor_protocol.protocol_version = SENSOR_VERSION;
    sensor_protocol.number_sensors = num_sensors;
    sensor_protocol.sensor_trip_point_notify_supported = true;
    sensor_protocol.sensor_trip_point_config_supported = true;
}

void fill_performance_protocol()
{
    performance_protocol.protocol_version = PERFORMANCE_VERSION;
    performance_protocol.num_performance_domains = num_performance_domains;
    performance_protocol.performance_limit_notification_support = true;
    performance_protocol.performance_level_notification_support = true;
}

void fill_clock_protocol()
{
    clock_protocol.protocol_version = CLOCK_VERSION;
    clock_protocol.number_clocks = num_clocks;
}

void mocker_fill_protocols_database()
{
    fill_base_protocol();
    fill_power_protocol();
    fill_performance_protocol();
    fill_sensor_protocol();
    fill_clock_protocol();
}
