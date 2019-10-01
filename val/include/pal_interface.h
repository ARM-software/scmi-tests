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

#ifndef __PAL_INTERFACE_H__
#define __PAL_INTERFACE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <inttypes.h>

/* set PRINT_LEVEL to one of the below values in your application entry
  to control the verbosity of the prints */
#define PAL_PRINT_ERR   1      /* Only errors, test and results description log */
#define PAL_PRINT_WARN  2      /* Only warnings & errors */
#define PAL_PRINT_TEST  3      /* Test description and result descriptions. THIS is DEFAULT */
#define PAL_PRINT_DEBUG 4      /* For Debug statements. contains register dumps etc */
#define PAL_PRINT_INFO  5      /* Print all statements. Do not use unless really needed */

#define PAL_STATUS_FAIL 0x90000000
#define PAL_STATUS_SKIP 0X10000000
#define PAL_STATUS_PASS 0x0
#define PAL_STATUS_NO_TESTS 0x1

#define SCMI_NAME_STR_SIZE 16
#define NUM_ELEMS(x) (sizeof(x) / sizeof((x)[0]))

void pal_send_message(uint32_t message_header_send, uint32_t parameter_count,
        const uint32_t *parameters, uint32_t *message_header_rcv, int32_t *status,
        uint32_t *return_values_count, uint32_t *return_values);
void pal_fill_protocols_database(void);
uint32_t pal_strcpy(uint8_t *dst_ptr, uint8_t *src_ptr);
uint32_t pal_strcmp(uint8_t *dst_ptr, uint8_t *src_ptr, uint32_t len);
void pal_print(uint32_t level, const char *string, va_list args);
void pal_memset_zero(uint32_t length, uint32_t *buffer);
void pal_initialize_system(void);
uint32_t pal_agent_get_accessible_device(uint32_t agent_id);
uint32_t pal_device_get_accessible_protocol(uint32_t device_id);
uint32_t pal_agent_get_inaccessible_device(uint32_t agent_id);
uint32_t pal_base_get_expected_protocol_version(void);
char *pal_base_get_expected_vendor_name(void);
char *pal_base_get_expected_subvendor_name(void);
uint32_t pal_base_get_expected_implementation_version(void);
uint32_t pal_base_get_expected_num_agents(void);
uint32_t pal_pwr_domain_get_expected_protocol_version(void);
uint32_t pal_pwr_domain_get_expected_num_domains(void);
uint32_t pal_get_expected_num_supported_protocols(void);
uint32_t pal_clock_get_expected_num_clocks(void);

#endif
