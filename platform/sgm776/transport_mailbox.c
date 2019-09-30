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

#include <poll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pal_platform.h>

/* mailbox transport macros and libraries */
#define MAX_MEMORY_LENGTH 128
#define REV(x) ((x << 24) | ((x & 0xff00) << 8) | ((x >> 8) & 0xff00) | (x >> 24))
#define MB_SIGNAL_FILE "/sys/kernel/debug/mailbox/signal"
#define MB_MESSAGE_FILE "/sys/kernel/debug/mailbox/message"

#define NO_ERROR 0
#define ERROR 5

/* mailbox memory byte offset mapping */
enum {
    MB_RESERVED =                        0x0,
    MB_CHANNEL_STATUS =                  0x4,
    MB_RESERVED_IMPLEMENTATION_DEFINED = 0x8,
    MB_FLAGS =                           0x10,
    MB_HEADER_PAYLOAD_LENGTH =           0x14,
    MB_MESSAGE_HEADER =                  0x18,
    MB_MESSAGE_PAYLOAD =                 0x1c
} mailbox_byte_offset;
/* mailbox memory byte length mapping */
enum {
    MB_LENGTH_RESERVED =                        0x4,
    MB_LENGTH_CHANNEL_STATUS =                  0x4,
    MB_LENGTH_RESERVED_IMPLEMENTATION_DEFINED = 0x8,
    MB_LENGTH_FLAGS =                           0x4,
    MB_LENGTH_HEADER_PAYLOAD_LENGTH =           0x4,
    MB_LENGTH_MESSAGE_HEADER =                  0x4
} mailbox_byte_length;

/*!
 * @brief Interface function that sends a
 * command to mailbox driver interfaces and receives a platform
 * message.
 *
 * This function builds the mailbox message using the data passed in. The
 * format is based on the table of mailbox memory layout described in Section 5.1.2
 * Mailbox Memory in the SCMI specification.
 *
 */
int32_t sgm_send_message(uint32_t message_header_send, uint32_t parameter_count,
        const uint32_t *parameters, uint32_t *message_header_rcv, int32_t *status,
        uint32_t *return_values_count, uint32_t *return_values)
{
    /* variables to be used in this function */
    uint32_t fd_signal, fd_message; // file descriptors for each mailbox interface
    uint32_t ret = 0; // return polling signal
    uint8_t *buffer; // pointer to access mailbox memory
    uint32_t counter;
    uint32_t *header_payload_length; // pointer to read payload length
    struct pollfd pfd;

    /* initialise buffer */
    buffer = (uint8_t *)malloc(MAX_MEMORY_LENGTH);
    if (buffer == NULL)
        return ERROR;
    memset(buffer, 0x0, MAX_MEMORY_LENGTH);

    /* prepare the message to be sent to mailbox driver interfaces in the following order:
     *      - RESERVED 1 (skipped as it has already been defaulted to zero)
     *      - CHANNEL STATUS (skipped as it has already been defaulted to zero)
     *      - RESERVED 2 (skipped as it has already been defaulted to zero)
     *      - MAILBOX FLAGS (always set to 1 to indicate completion via an interrupt)
     *      - LENGTH (this will be particularly useful to dertermine the returned payload length)
     *      - MESSAGE HEADER (as the data passed in)
     *      - MESSAGE PAYLOAD (used to hold parameter when sending OR return values upon receipt)
     * */

    buffer[MB_FLAGS] = 0x1; // populating MAILBOX_FLAGS

    buffer[MB_HEADER_PAYLOAD_LENGTH] = 0x04 + (parameter_count * 4); // populating LENGTH (4+N)

    for (counter = 0; counter < 4; ++counter) // populating MESSAGE HEADER (4 bytes)
    {
        buffer[MB_MESSAGE_HEADER+counter] = (message_header_send >> (counter * 8)) & 0xFF;
    }

    if (parameter_count != 0) { // populating MESSAGE PAYLOAD
        for (counter = 0; counter < (parameter_count * 4); counter += 4) {
            uint32_t byte_shift;

            for (byte_shift = 0; byte_shift < 4; ++byte_shift) {
                buffer[MB_MESSAGE_PAYLOAD+counter+byte_shift] =
                        (parameters[counter/4] >> (byte_shift * 8)) & 0xFF;
            }
        }
    }

    /* Send message to the mailbox driver interface
     * defined by the MB_MESSAGE_FILE macro
     * */
    fd_message = open(MB_MESSAGE_FILE, 0x0002);
    if (fd_message < 0) {
        return ERROR;
    }
    write(fd_message, buffer, MAX_MEMORY_LENGTH);

    /* Send Signal to the mailbox driver interface
     * defined by the MB_SIGNAL_FILE macro
     * */
    fd_signal = open(MB_SIGNAL_FILE, 0x0001);
    if (fd_signal < 0) {
        return ERROR;
    }
    memset(buffer, 0x0, MAX_MEMORY_LENGTH);
    buffer[0] = 0x01;
    write(fd_signal, buffer, 1);

    /* polling */
    pfd.fd = fd_message;
    pfd.revents = POLLIN;
    ret = poll(&pfd, 1, 1000);

    // non-negative indicates successful completion
    if (ret < 0) {
        return ERROR;
    }

    /* read returned message into the buffer */
    read(fd_message, buffer, MAX_MEMORY_LENGTH);

    /* Extract bytes using an uint32_t pointer. */
    header_payload_length = (uint32_t *) &buffer[MB_HEADER_PAYLOAD_LENGTH];

    /* read the payload part */
    for (counter = 0; counter < (*(header_payload_length) / 4); ++counter) {
        if (counter == 0) {
            // extract header
            // plus 1 to skip the Length
            *message_header_rcv = *(header_payload_length + 1 + counter);
        }
        else if (counter == 1) {
            // extract status
            // plus 1 to skip the Length
            *status = *(header_payload_length + 1 + counter);
        }
        else {
            // extract payload
            // plus 1 to skip the Length
            // return_values starts from 0, hence subtract 2 here
            return_values[(counter - 2)] =  *(header_payload_length + 1 + counter);
        }
    }

    return NO_ERROR;
}

/*!
 * @brief Interface function that waits for
 * delayed response.
 *
 * This is the mocker platform implementation of the receive_platform_response
 * interface which will be called by the test_agent when it is expecting to
 * receive either a delayed_response or notification from the platform over the
 * P2A channel.
 *
 */
int sgm_wait_for_response(uint32_t *message_header_rcv,
        int32_t *status, size_t *return_values_count, uint32_t *return_values,
        bool *message_ready, uint32_t timeout)
{
    return NO_ERROR;
}

uint32_t sgm_agent_get_accessible_device(uint32_t agent_id)
{
   return NO_ERROR;
}

uint32_t sgm_agent_get_inaccessible_device(uint32_t agent_id)
{
    return NO_ERROR;
}

uint32_t sgm_device_get_accessible_protocol(uint32_t device_id)
{
    return NO_ERROR;
}


