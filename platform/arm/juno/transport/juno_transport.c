/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <juno.h>
#include <test_helper.h>
#include <platform_exports.h>
#include <test_execute.h>
#include <base_common.h>
#ifdef POWER_PROTOCOL
#include <power_common.h>
#endif
#ifdef SYSTEM_POWER_PROTOCOL
#include <system_power_common.h>
#endif
#ifdef PERFORMANCE_PROTOCOL
#include <performance_common.h>
#endif
#ifdef CLOCK_PROTOCOL
#include <clock_common.h>
#endif
#ifdef SENSOR_PROTOCOL
#include <sensor_common.h>
#endif
#define NUMBER_CLOCKS 4
#define NUMBER_SENSORS 4
#define MAX_NUM_ASYNC_RATES_CHGS_SPRT 8

/* mailbox transport macros and libraries */
#define MAX_MEMORY_LENGTH 128
#define REV(x) ((x << 24) | ((x & 0xff00) << 8) | ((x >> 8) & 0xff00) | (x >> 24))
#define MB_SIGNAL_FILE "/sys/kernel/debug/mailbox/signal"
#define MB_MESSAGE_FILE "/sys/kernel/debug/mailbox/message"
#include <poll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>

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
int arm_scmi_send_message(uint32_t message_header_send, size_t parameter_count,
        const uint32_t *parameters, uint32_t *message_header_rcv, int32_t *status,
        size_t *return_values_count, uint32_t *return_values)
{
    /* variables to be used in this function */
    uint32_t fd_signal, fd_message; // file descriptors for each mailbox interface
    uint32_t ret = 0; // return polling signal
    uint8_t *buffer; // pointer to access mailbox memory
    uint32_t counter;
    uint32_t rw_size;
    uint32_t *header_payload_length; // pointer to read payload length
    struct pollfd pfd;

    /* initialise buffer */
    buffer = (uint8_t *)malloc(MAX_MEMORY_LENGTH);
    if (buffer == NULL) {
        return ERROR;
    }
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

    for (counter=0; counter < 4; ++counter) // populating MESSAGE HEADER (4 bytes)
    {
        buffer[MB_MESSAGE_HEADER+counter] = (message_header_send >> (counter * 8)) & 0xFF;
    }

    if (parameter_count != 0) { // populating MESSAGE PAYLOAD
        for (counter=0; counter < (parameter_count * 4); counter+=4) {
            uint32_t byte_shift;

            for (byte_shift=0; byte_shift < 4; ++byte_shift) {
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
    rw_size = write(fd_message, buffer, MAX_MEMORY_LENGTH);

    /* Send Signal to the mailbox driver interface
     * defined by the MB_SIGNAL_FILE macro
     * */
    fd_signal = open(MB_SIGNAL_FILE, 0x0001);
    if (fd_signal < 0) {
        return ERROR;
    }
    memset(buffer, 0x0, MAX_MEMORY_LENGTH);
    buffer[0] = 0x01;
    rw_size = write(fd_signal, buffer, 1);

    /* polling */
    pfd.fd = fd_message;
    pfd.revents = POLLIN;
    ret = poll(&pfd, 1, 1000);

    // non-negative indicates successful completion
    if (ret < 0) {
        return ERROR;
    }

    /* read returned message into the buffer */
    rw_size = read(fd_message, buffer, MAX_MEMORY_LENGTH);

    /* Extract bytes using an uint32_t pointer. */
    header_payload_length = (uint32_t *)&buffer[MB_HEADER_PAYLOAD_LENGTH];

    /* read the payload part */
    for ( counter=0; counter < (*(header_payload_length) / 4); ++counter) {
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
int arm_scmi_wait_for_response(uint32_t *message_header_rcv,
        int32_t *status, size_t *return_values_count, uint32_t *return_values,
        bool *message_ready, uint32_t timeout)
{
    return NO_ERROR;
}
