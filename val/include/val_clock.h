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

#ifndef __VAL_CLOCK_H__
#define __VAL_CLOCK_H__

#define VERSION_OFFSET                     0
#define ATTRIBUTE_OFFSET                   0
#define NAME_OFFSET                        1
#define RATE_FLAG_OFFSET                   0
#define RATES_OFFSET                       1
#define RATE_LOWER_WORD_OFFSET             0
#define RATE_UPPER_WORD_OFFSET             1

#define DELAYED_RESP_CLOCK_ID_OFFSET       0
#define DELAYED_RESP_RATE_LOWER_OFFSET     1
#define DELAYED_RESP_RATE_UPPER_OFFSET     2


#define INVALID_ATTRIBUTE                  0xF
#define CONFIG_SET_ENABLE                  0x1
#define CONFIG_SET_DISABLE                 0x0
#define CLOCK_SET_ASYNC_MODE               0x1
#define CLOCK_SET_SYNC_MODE                0x0
#define CLOCK_SET_INVALID_FLAG             0xFF
#define MASK_FOR_LOWER_WORD                0xFFFFFFFFLL
#define MASK_FOR_UPPER_WORD                0xFFFFFFFF00000000LL
#define CLOCK_RATE_FMT_ARRAY               0x0
#define CLOCK_RATE_FMT_STEP_SIZE           0x1


#define MAX_NUM_OF_CLOCKS 10

typedef struct {
    uint64_t lowest_rate;
    uint64_t highest_rate;
} CLOCK_RATE_INFO_s;

typedef struct {
    uint32_t num_clocks;
    uint32_t max_async_cmd;
    uint32_t clock_attribute[MAX_NUM_OF_CLOCKS];
    uint8_t  clock_name[MAX_NUM_OF_CLOCKS][SCMI_NAME_STR_SIZE];
    CLOCK_RATE_INFO_s clock_rate[MAX_NUM_OF_CLOCKS];
} CLOCK_INFO_s;

/* Common Tests */
uint32_t clock_query_protocol_version(uint32_t *version);
uint32_t clock_query_protocol_attributes(void);
uint32_t clock_query_mandatory_command_support(void);
uint32_t clock_invalid_messageid_call(void);
uint32_t clock_attributes_check(void);
uint32_t clock_attributes_check_invalid_clock_id(void);
uint32_t clock_query_describe_rates(void);
uint32_t clock_query_describe_rates_invalid_clock_id(void);
uint32_t clock_rate_set_sync_check(void);
uint32_t clock_rate_set_async_check(void);
uint32_t clock_rate_set_invalid_param_check(void);
uint32_t clock_rate_set_invalid_clock_id_check(void);
uint32_t clock_rate_get_check(void);
uint32_t clock_rate_get_invalid_clock_id_check(void);
uint32_t clock_config_set_invalid_attr_check(void);
uint32_t clock_config_set_invalid_clock_id_check(void);
uint32_t clock_config_set_check(void);

/* expected values */
uint32_t val_clock_get_expected_num_clocks(void);
uint32_t val_clock_get_expected_max_async_cmd(void);
uint32_t val_clock_get_expected_number_of_rates(uint32_t clock_id);
uint32_t val_check_clock_config_change_support(uint32_t clock_id);

#endif
