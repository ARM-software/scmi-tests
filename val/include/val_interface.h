/** @file
 * Copyright (c) 2019-2020, Arm Limited or its affiliates. All rights reserved.
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

#define RUN_TEST(x) val_report_status(x)

#define PROTOCOL_VERSION_1 0x00010000
#define PROTOCOL_VERSION_2 0x00020000

#define SCMI_BASE_TEST_NUM_BASE         100
#define SCMI_POWER_DOMAIN_TEST_NUM_BASE 200
#define SCMI_SYSTEM_POWER_TEST_NUM_BASE 300
#define SCMI_PERFORMANCE_TEST_NUM_BASE  400
#define SCMI_CLOCK_TEST_NUM_BASE        500
#define SCMI_SENSOR_TEST_NUM_BASE       600
#define SCMI_RESET_TEST_NUM_BASE        700

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
#define VAL_EXTRACT_BITS(data, start, end) ((data >> start) & ((1ul << (end - start + 1)) - 1))
#define VAL_GET_64BIT_DATA(upper, lower) ((((uint64_t)upper) << 32) | lower)
#define VAL_INIT_TEST_PARAM(param_count, rsp_msg_hdr, ret_value_count, status) \
     {param_count = 0; rsp_msg_hdr = 0; ret_value_count = 0; status = SCMI_GENERIC_ERROR; }

#define VAL_ERR_PRINT_ID_ENABLED  1
#define VAL_ERR_PRINT_ID_DISABLED 0

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
    PROTOCOL_RESET,
    PROTOCOL_MAX
} PROTOCOL_IDENTIFIER;

uint32_t val_strcpy(uint8_t *dst_ptr, uint8_t *src_ptr);
uint32_t val_strcmp(uint8_t *dst_ptr, uint8_t *src_ptr, uint32_t len);
uint32_t val_compare_msg_hdr(uint32_t sent_msg_hdr, uint32_t rsp_msg_hdr);
uint32_t val_protocol_version_check(uint32_t exp_version, uint32_t version);
uint32_t val_reserved_bits_check_is_zero(uint32_t reserved_bits);
void val_print(uint32_t level, const char *string, ...);
void val_memset(void *ptr, int value, size_t length);
uint32_t val_msg_hdr_create(uint32_t protoco_id, uint32_t msg_id, uint32_t msg_type);
char *val_get_result_string(uint32_t test_status);
char *val_get_protocol_str(uint32_t protocol_id);
char *val_get_status_string(int32_t status);
uint32_t val_compare_return_count(uint32_t received, uint32_t expected);
void val_send_message(uint32_t msg_hdr, size_t num_parameter, uint32_t *parameter_buffer,
         uint32_t *rcvd_msg_hdr, int32_t *status, size_t *rcvd_buffer_size, uint32_t *rcvd_buffer);
void val_receive_delayed_response(uint32_t *message_header_rcv, int32_t *status,
                              size_t *return_values_count, uint32_t *return_values);
void val_receive_notification(uint32_t *message_header_rcv, size_t *return_values_count,
                              uint32_t *return_values);
uint32_t val_initialize_system(void *info);
uint32_t val_test_initialize(uint32_t test_num, char *test_desc);
void val_print_return_values(uint32_t count, uint32_t *values);
uint32_t val_compare_status(int32_t status, int32_t expected_status);
uint32_t val_agent_get_accessible_device(uint32_t agent_id);
uint32_t val_device_get_accessible_protocol(uint32_t device_id);
uint32_t val_agent_get_inaccessible_device(uint32_t agent_id);
void val_agent_set_supported_protocol_list(uint32_t protocol_list);
uint32_t val_agent_check_protocol_support(uint32_t protocol);
uint32_t val_check_trusted_agent(uint32_t agent_id);
uint32_t val_report_status(uint32_t status);
uint32_t val_get_test_passed(void);
uint32_t val_get_test_failed(void);
uint32_t val_get_test_skipped(void);
uint32_t val_compare(char *parameter, uint32_t rcvd_val, uint32_t exp_val);
uint32_t val_compare_str(char *parameter, char *rcvd_val, char *exp_val, uint32_t len);

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
uint32_t val_base_execute_tests(void);

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
uint32_t val_power_domain_execute_tests(void);

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

typedef enum {
    SYSTEM_POWER_SHUTDOWN,
    SYSTEM_POWER_COLD_RESET,
    SYSTEM_POWER_WARM_RESET,
    SYSTEM_POWER_POWER_UP,
    SYSTEM_POWER_SUSPEND,
    SYSTEM_POWER_RESERVED,
} SYSTEM_POWER_STATES;

void val_system_power_save_info(uint32_t param_identifier, uint32_t param_value);
uint32_t val_system_power_execute_tests(void);

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
    PERFORMANCE_DESCRIBE_FASTCHANNEL,
    PERFORMANCE_INVALID_COMMAND
} PERFORMANCE_COMMANDS;

typedef enum {
    PERFORMANCE_LIMITS_CHANGED = 0x0,
    PERFORMANCE_LEVEL_CHANGED
} PERFORMANCE_NOTIFICATIONS;

typedef enum {
    NUM_PERF_DOMAINS,
    PERF_STATS_ADDR_LOW,
    PERF_STATS_ADDR_HIGH,
    PERF_STATS_ADDR_LEN,
    PERF_DOMAIN_NAME,
    PERF_MESSAGE_FAST_CH_SUPPORT,
    PERF_DOMAIN_FAST_CH_SUPPORT,
    PERF_DOMAIN_LVL_CHANGE_NTFY_SUPPORT,
    PERF_DOMAIN_LIMIT_CHANGE_NTFY_SUPPORT,
    PERF_DOMAIN_SET_PERFORMANCE_LEVEL_SUPPORT,
    PERF_DOMAIN_SET_PERFORMANCE_LIMIT_SUPPORT,
    PERF_DOMAIN_MAX_LEVEL,
    PERF_DOMAIN_MIN_LEVEL,
    PERF_DOMAIN_INTERMEDIATE_LEVEL,
} PERFORMANCE_DOMAIN_INFO;

void val_performance_save_info(uint32_t param_identifier, uint32_t perf_id, uint32_t param_value);
uint32_t val_performance_get_info(uint32_t param_identifier, uint32_t perf_id);
uint32_t val_performance_execute_tests(void);

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
    CLOCK_NAME,
    CLOCK_LOWEST_RATE,
    CLOCK_HIGHEST_RATE
} CLOCK_INFO;

void val_clock_save_info(uint32_t param_identifier, uint32_t clock_id, uint32_t param_value);
void val_clock_save_rate(uint32_t param_identifier, uint32_t clock_id, uint64_t param_value);
void val_clock_save_name(uint32_t param_identifier, uint32_t clock_id, uint8_t *param_value);
uint32_t val_clock_get_info(uint32_t param_identifier, uint32_t clock_id);
uint64_t val_clock_get_rate(uint32_t param_identifier, uint32_t clock_id);
uint32_t val_clock_execute_tests(void);

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
    SENSOR_STATS_ADDR_LEN,
    SENSOR_NUM_OF_TRIP_POINTS,
    SENSOR_ASYNC_READ_SUPPORT
} SENSOR_INFO;

void val_sensor_save_info(uint32_t param_identifier, uint32_t param_value);
uint32_t val_sensor_get_info(uint32_t param_identifier);
void val_sensor_save_desc_info(uint32_t param_identifier, uint32_t sensor_id,
        uint32_t param_value);
uint32_t val_sensor_get_desc_info(uint32_t param_identifier, uint32_t sensor_id);
uint32_t val_sensor_execute_tests(void);

/* Reset protocol defines & APIs*/

typedef enum {
    RESET_PROTOCOL_VERSION = 0x0,
    RESET_PROTOCOL_ATTRIBUTES,
    RESET_PROTOCOL_MESSAGE_ATTRIBUTES,
    RESET_DOMAIN_ATTRIBUTES,
    RESET_PROTOCOL_RESET,
    RESET_PROTOCOL_NOTIFY,
    RESET_INVALID_COMMAND
} RESET_COMMANDS;

typedef enum {
    RESET_COMPLETE = 0x04
} RESET_DELAYED_RESPONSE;

typedef enum {
    RESET_ISSUED = 0x0
} RESET_NOTIFICATIONS;

typedef enum {
    NUM_RESET_DOMAINS,
    RESET_ASYNC_SUPPORT,
    RESET_NOTIFY_SUPPORT
} RESET_INFO;

void val_reset_save_info(uint32_t param_identifier, uint32_t domain_id, uint32_t param_value);
uint32_t val_reset_get_info(uint32_t param_identifier, uint32_t domain_id);
uint32_t val_reset_execute_tests(void);

#endif
