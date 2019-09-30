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

#ifndef __VAL_INTERFACE_H__
#define __VAL_INTERFACE_H__

#include "pal_interface.h"

#define SCMI_BASE_TEST_NUM_BASE         100
#define SCMI_POWER_DOMAIN_TEST_NUM_BASE 200
#define SCMI_SYSTEM_POWER_TEST_NUM_BASE 300
#define SCMI_PERFORMANCE_TEST_NUM_BASE  400
#define SCMI_CLOCK_TEST_NUM_BASE        500
#define SCMI_SENSOR_TEST_NUM_BASE       600

/* set PRINT_LEVEL to one of the below values in your application entry
  to control the verbosity of the prints */
#define VAL_PRINT_ERR   PAL_PRINT_ERR
#define VAL_PRINT_WARN  PAL_PRINT_WARN
#define VAL_PRINT_TEST  PAL_PRINT_TEST
#define VAL_PRINT_DEBUG PAL_PRINT_DEBUG
#define VAL_PRINT_INFO  PAL_PRINT_INFO

#define VAL_STATUS_FAIL     PAL_STATUS_FAIL
#define VAL_STATUS_SKIP     PAL_STATUS_SKIP
#define VAL_STATUS_PASS     PAL_STATUS_PASS
#define VAL_STATUS_NO_TESTS PAL_STATUS_NO_TESTS

#define VAL_RESERVED_BYTE_ZERO 0x00
#define VAL_EXTRACT_BITS(data, start, end) ((data >> start) & ((1ul << (end-start+1))-1))
#define VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, ret_value_count, status) \
     {param_count = 0; rsp_msg_hdr = 0; ret_value_count = 0; status = SCMI_GENERIC_ERROR; }

typedef uint32_t (*test_fn_ptr)(void);

/* GENERIC VAL APIs */

typedef enum {
    SCMI_SUCCESS =              0,
    SCMI_NOT_SUPPORTED =        -1,
    SCMI_INVALID_PARAMETERS =   -2,
    SCMI_DENIED =               -3,
    SCMI_NOT_FOUND =            -4,
    SCMI_OUT_OF_RANGE =         -5,
    SCMI_BUSY =                 -6,
    SCMI_COMMS_ERROR =          -7,
    SCMI_GENERIC_ERROR =        -8,
    SCMI_HARDWARE_ERROR =       -9,
    SCMI_PROTOCOL_ERROR =       -10,
    SCMI_RESERVED =             -11,
    SCMI_VENDOR_SPECIFIC =      -128
} SCMI_STATUS;

typedef enum {
    COMMAND_MSG =           0,
    DELAYED_RESPONSE_MSG =  2,
    NOTIFICATION_MSG =      3
} MESSAGE_TYPE;

typedef enum {
    PROTOCOL_BASE = 0x10,
    PROTOCOL_POWER_DOMAIN,
    PROTOCOL_SYSTEM_POWER,
    PROTOCOL_PERFORMANCE,
    PROTOCOL_CLOCK,
    PROTOCOL_SENSOR,
    PROTOCOL_MAX
} PROTOCOL_IDENTIFIER;

test_fn_ptr *val_get_testlist(uint32_t protocol_identifier, uint32_t *num_tests);
uint32_t val_strcpy(uint8_t *dst_ptr, uint8_t *src_ptr);
uint32_t val_strcmp(uint8_t *dst_ptr, uint8_t *src_ptr, uint32_t len);
uint32_t val_compare_msg_hdr(uint32_t sent_msg_hdr, uint32_t rcvd_msg_hdr);
uint32_t val_protocol_version_check(uint32_t exp_version, uint32_t version);
uint32_t val_reserved_bits_check_is_zero(uint32_t reserved_bits);
void val_print(uint32_t level, const char *string, ...);
void val_memset_zero(uint32_t *buffer, uint32_t length);
uint32_t val_msg_hdr_create(uint32_t protoco_id, uint32_t msg_id, uint32_t msg_type);
char *val_get_result_string(uint32_t test_status);
char *val_get_protocol_str(uint32_t protocol_id);
char *val_get_status_string(int32_t status);
void val_send_message(uint32_t msg_hdr, uint32_t num_parameter, uint32_t *parameter_buffer,
                      uint32_t *rcvd_msg_hdr, int32_t *status, uint32_t *rcvd_buffer_size,
                      uint32_t *rcvd_buffer);
void val_initialize_system(void);
uint32_t val_test_initialize(uint32_t test_num, char *test_desc);
uint32_t val_execute_tests(uint32_t protocol_identifier);
uint32_t val_compare_status(int32_t status, int32_t expected_status);
uint32_t val_agent_get_accessible_device(uint32_t agent_id);
uint32_t val_device_get_accessible_protocol(uint32_t device_id);
uint32_t val_agent_get_inaccessible_device(uint32_t agent_id);

/* BASE VAL APIs */

typedef enum {
    BASE_PROTOCOL_VERSION,
    BASE_PROTOCOL_ATTRIBUTES,
    BASE_PROTOCOL_MESSAGE_ATTRIBUTES,
    BASE_DISCOVER_VENDOR,
    BASE_DISCOVER_SUB_VENDOR,
    BASE_DISCOVER_IMPLEMENTATION_VERSION,
    BASE_DISCOVER_LIST_PROTOCOLS,
    BASE_DISCOVER_AGENT,
    BASE_NOTIFY_ERRORS,
    BASE_SET_DEVICE_PERMISSIONS,
    BASE_SET_PROTOCOL_PERMISSIONS,
    BASE_RESET_AGENT_CONFIGURATION,
    BASE_INVALID_COMMAND
} BASE_COMMANDS;

typedef enum {
    BASE_ERROR_EVENT
} BASE_NOTIFICATIONS;

typedef enum {
    BASE_NUM_AGENTS,
    BASE_NUM_PROTOCOLS,
    BASE_TEST_AGENT_ID,
    BASE_IMPLEMENTATION_VERSION,
    BASE_VENDOR_NAME,
    BASE_SUBVENDOR_NAME,
    BASE_TEST_AGENT_NAME,
} BASE_INFO;

void val_base_save_info(uint32_t param_identifier, uint32_t param_value);
void val_base_save_name(uint32_t param_identifier, uint8_t *vendor_name);
uint32_t val_base_get_info(uint32_t param_identifier);
uint32_t val_base_get_expected_protocol_version(void);

/* POWER DOMAIN VAL APIs */

typedef enum {
    PD_PROTOCOL_VERSION,
    PD_PROTOCOL_ATTRIBUTES,
    PD_PROTOCOL_MESSAGE_ATTRIBUTES,
    POWER_DOMAIN_ATTRIBUTES,
    POWER_STATE_SET,
    POWER_STATE_GET,
    POWER_STATE_NOTIFY,
    POWER_STATE_CHANGE_REQUESTED_NOTIFY,
    POWER_PROTOCOL_INVALID_COMMAND
} POWER_DOMAIN_COMMANDS;

typedef enum {
    POWER_STATE_CHANGED,
    POWER_STATE_CHANGE_REQUESTED
} POWER_DOMAIN_NOTIFICATIONS;

typedef enum {
    NUM_POWER_DOMAIN,
    PD_STATS_ADDR_LOW,
    PD_STATS_ADDR_HIGH,
    PD_STATS_ADDR_LEN,
    PD_STATE_CHANGE_NOTI_SUPPORT,
    PD_STATE_ASYNC_SUPPORT,
    PD_STATE_SYNC_SUPPORT,
    PD_DOMAIN_NAME
} POWER_DOMAIN_INFO;

uint32_t val_power_domain_get_info(uint32_t pd_id, uint32_t param_identifier);
void val_power_domain_save_info(uint32_t param_identifier, uint32_t domain_id,
                                uint32_t param_value);
uint32_t val_power_domain_get_expected_protocol_version(void);

/* SYSTEM POWER VAL APIs */

typedef enum {
    SYSTEM_POWER_PROTOCOL_VERSION = 0x0,
    SYSTEM_POWER_PROTOCOL_ATTRIBUTES,
    SYSTEM_POWER_PROTOCOL_MESSAGE_ATTRIBUTES,
    SYSTEM_POWER_STATE_SET,
    SYSTEM_POWER_STATE_GET,
    SYSTEM_POWER_STATE_NOTIFY,
    SYSTEM_POWER_INVALID_COMMAND
} SYSTEM_POWER_COMMANDS;

typedef enum {
    SYSTEM_POWER_STATE_NOTIFIER = 0x0
} SYSTEM_POWER_NOTIFICATIONS;

typedef enum {
    SYSTEM_RESET_SUPPORT,
    SYSTEM_SUSPEND_SUPPORT
} SYSTEM_POWER_INFO;

void val_system_power_save_info(uint32_t param_identifier, uint32_t param_value);
uint32_t val_system_power_get_expected_protocol_version(void);

/* PERFORMANCE VAL APIs */

typedef enum {
    PERFORMANCE_PROTOCOL_VERSION = 0x0,
    PERFORMANCE_PROTOCOL_ATTRIBUTES,
    PERFORMANCE_PROTOCOL_MESSAGE_ATTRIBUTES,
    PERFORMANCE_DOMAIN_ATTRIBUTES,
    PERFORMANCE_DESCRIBE_LEVELS,
    PERFORMANCE_LIMITS_SET,
    PERFORMANCE_LIMITS_GET,
    PERFORMANCE_LEVEL_SET,
    PERFORMANCE_LEVEL_GET,
    PERFORMANCE_NOTIFY_LIMITS,
    PERFORMANCE_NOTIFY_LEVEL,
    PERFORMANCE_DESCRIBE_FASTCHANNEL
} PERFORMANCE_COMMANDS;

typedef enum {
    PERFORMANCE_LIMITS_CHANGED = 0x0,
    PERFORMANCE_LEVEL_CHANGED
} PERFORMANCE_NOTIFICATIONS;

typedef enum {
    NUM_PERF_DOMAINS,
    PERF_STATS_ADDR_LOW,
    PERF_STATS_ADDR_HIGH,
    PERF_STATS_ADDR_LEN
} PERFORMANCE_DOMAIN_INFO;

void val_performance_save_info(uint32_t param_identifier, uint32_t param_value);
uint32_t val_performance_get_expected_protocol_version(void);

/* CLOCK VAL APIs */

typedef enum {
    CLOCK_PROTOCOL_VERSION = 0x0,
    CLOCK_PROTOCOL_ATTRIBUTES,
    CLOCK_PROTOCOL_MESSAGE_ATTRIBUTES,
    CLOCK_ATTRIBUTES,
    CLOCK_DESCRIBE_RATES,
    CLOCK_RATE_SET,
    CLOCK_RATE_GET,
    CLOCK_CONFIG_SET,
    CLOCK_INVALID_COMMAND
} CLOCK_COMMANDS;

typedef enum {
    CLOCK_RATE_SET_COMPLETE = 0x5
} CLOCK_DELAYED_RESPONSE;

typedef enum {
    NUM_CLOCKS,
    CLOCK_MAX_PENDING_ASYNC_CMD,
    CLOCK_ATTRIBUTE,
    CLOCK_NAME
} CLOCK_INFO;

void val_clock_save_info(uint32_t param_identifier, uint32_t clock_id, uint32_t param_value);
void val_clock_save_name(uint32_t param_identifier, uint32_t clock_id, uint8_t *param_value);
uint32_t val_clock_get_info(uint32_t param_identifier, uint32_t clock_id);
uint32_t val_clock_get_expected_protocol_version(void);

/* SENSOR VAL APIs */

typedef enum {
    SENSOR_PROTOCOL_VERSION = 0x0,
    SENSOR_PROTOCOL_ATTRIBUTES,
    SENSOR_PROTOCOL_MESSAGE_ATTRIBUTES,
    SENSOR_DESCRIPTION_GET,
    SENSOR_TRIP_POINT_NOTIFY,
    SENSOR_TRIP_POINT_CONFIG,
    SENSOR_READING_GET,
    SENSOR_INVALID_COMMAND
} SENSOR_COMMANDS;

typedef enum {
    SENSOR_READING_COMPLETE = 0x6
} SENSOR_DELAYED_RESPONSE;

typedef enum {
    SENSOR_TRIP_POINT_EVENT = 0x0
} SENSOR_NOTIFICATIONS;

typedef enum {
    NUM_SENSORS,
    SENSOR_STATS_ADDR_LOW,
    SENSOR_STATS_ADDR_HIGH,
    SENSOR_STATS_ADDR_LEN
} SENSOR_INFO;

void val_sensor_save_info(uint32_t param_identifier, uint32_t param_value);
uint32_t val_sensor_get_expected_protocol_version(void);
uint32_t val_sensor_get_info(uint32_t param_identifier);

#endif
