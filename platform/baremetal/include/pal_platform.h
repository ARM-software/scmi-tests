/** @file
 * Copyright (c) 2017-2020, Arm Limited or its affiliates. All rights reserved.
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

#ifndef _PAL_PLATFORM_H_
#define _PAL_PLATFORM_H_

#include <inttypes.h>
#include <stdbool.h>

/* Frequently used constants */
#define NO_ERROR 0
#define MAX_RETURNS_SIZE 32
#define LOG_STR_SIZE 96
/*
 * Structure to pass information from the platform to arm_scmi_agent_execute
 */
struct arm_scmi_platform_protocol_info {
    unsigned int protocol_id;
    const void *expected_return_values;
};

struct arm_scmi_platform_info {
    size_t platform_protocol_info_count;
    struct arm_scmi_platform_protocol_info *platform_protocol_info_table;
};

/*!
 * @brief Send an SCMI command and return the reply.
 *
 * @param message_header_send Outgoing SCMI message header.
 * @param parameter_count Number of outgoing SCMI parameters.
 * @param parameters Outgoing SCMI parameters.
 * @param message_header_rcv Storage for the SCMI message header returned.
 * @param status Storage for the SCMI status returned.
 * @param return_values_count Storage for the number of SCMI parameters
 *                            returned.
 * @param return_values Storage for the SCMI parameters returned.
 *
 * @return NO_ERROR if no error occurred, or any other value otherwise.
 */
int arm_scmi_send_message(uint32_t message_header_send,
    size_t parameter_count, const uint32_t *parameters,
    uint32_t *message_header_rcv, int32_t *status,
    size_t *return_values_count, uint32_t *return_values);

/*!
 * @brief Return an SCMI delayed response.
 *
 * @param message_header_rcv Storage for the SCMI message header returned.
 * @param status Storage for the SCMI status returned.
 * @param return_values_count Storage for the number of parameters returned.
 * @param return_values Storage for the SCMI parameters returned.
 *
 * @return NO_ERROR if no error occurred, or any other value otherwise.
 */
int arm_scmi_wait_for_response(uint32_t *message_header_rcv,
    int32_t *status, size_t *return_values_count, uint32_t *return_values);

/*!
 * @brief Return an SCMI notification.
 *
 * @param message_header_rcv Storage for the SCMI message header returned.
 * @param status Storage for the SCMI status returned.
 * @param return_values_count Storage for the number of parameters returned.
 * @param return_values Storage for the SCMI parameters returned.
 *
 * @return NO_ERROR if no error occurred, or any other value otherwise.
 */
int arm_scmi_wait_for_notification(uint32_t *message_header_rcv,
    size_t *return_values_count, uint32_t *return_values);

/*!
 * @brief Interface function used to handle output from the test agent.
 *
 * @param output Pointer to the formatted output string buffer.
 *
 */
void arm_scmi_log_output(const char *output);

int arm_scmi_agent_execute(void *agent_info);

#endif /* _PAL_PLATFORM_H_ */
