/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

/*!
 * @file
 *
 * @brief This file declares the functions that the platform must implement and
 * export in order for the test library to communicate with the SCMI and logging
 * logic of the platform.
 */

#ifndef PLATFORM_EXPORTS_H_
#define PLATFORM_EXPORTS_H_

#include <inttypes.h>
#include <stdbool.h>

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
 * @brief Wait for and return an SCMI delayed response.
 *
 * @param message_header_rcv Storage for the SCMI message header returned.
 * @param status Storage for the SCMI status returned.
 * @param return_values_count Storage for the number of parameters returned.
 * @param return_values Storage for the SCMI parameters returned.
 * @param timeout Number of seconds to wait on a response before aborting.
 *
 * @return NO_ERROR if no error occurred, or any other value otherwise.
 */
int arm_scmi_wait_for_response(uint32_t *message_header_rcv,
	int32_t *status, size_t *return_values_count, uint32_t *return_values,
	bool *message_ready, uint32_t timeout);

/*!
 * @brief Interface function used to handle output from the test agent.
 *
 * @param output Pointer to the formatted output string buffer.
 *
 */
void arm_scmi_log_output(const char *output);

#endif /* PLATFORM_EXPORTS_H_ */
