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

#ifndef __PROTOCOL_COMMON_H__
#define __PROTOCOL_COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>
#include <base_protocol.h>
#include <assert.h>

#define SCMI_NAME_STR_SIZE 16
#define NUM_ELEMS(x) (sizeof(x) / sizeof((x)[0]))

enum BITS_HEADER {
    RESERVED_LOW =      28,
    RESERVED_HIGH =     31,
    TOKEN_LOW =         18,
    TOKEN_HIGH =        27,
    PROTOCOL_ID_LOW =   10,
    PROTOCOL_ID_HIGH =  17,
    MESSAGE_TYPE_LOW =  8,
    MESSAGE_TYPE_HIGH = 9,
    MESSAGE_ID_LOW =    0,
    MESSAGE_ID_HIGH =   7
};

/*
 * These macros are used to compute the offset values of return and parameters
 * fields.
*/
#define OFFSET_BYTES_RET(st, elem) \
    ((offsetof(st, returns.elem) - offsetof(st, returns)))
#define OFFSET_RET(st, elem) \
    (OFFSET_BYTES_RET(st, elem)/sizeof(uint32_t))
#define OFFSET_PARAM(st, elem) \
(((offsetof(st, parameters.elem) - offsetof(st, parameters)))/sizeof(uint32_t))

enum SCMI_STATUS_CODES {
    SCMI_STATUS_SUCCESS =                0,
    SCMI_STATUS_NOT_SUPPORTED =         -1,
    SCMI_STATUS_INVALID_PARAMETERS =    -2,
    SCMI_STATUS_DENIED =                -3,
    SCMI_STATUS_NOT_FOUND =             -4,
    SCMI_STATUS_OUT_OF_RANGE =          -5,
    SCMI_STATUS_BUSY =                  -6,
    SCMI_STATUS_COMMS_ERROR =           -7,
    SCMI_STATUS_GENERIC_ERROR =         -8,
    SCMI_STATUS_HARDWARE_ERROR =        -9,
    SCMI_STATUS_PROTOCOL_ERROR =        -10,
    SCMI_RESERVED =                     -11,
    SCMI_STATUS_NOT_SPECIFIED =         -100
};

#endif
