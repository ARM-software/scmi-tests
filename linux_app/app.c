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

#include "val_interface.h"

/**
  @brief This API executes all the SCMI tests
           1. Caller       -  Application layer.
           2. Prerequisite -  val_gic_create_info_table()
  @param   protocol_identifier identifier of protocol
  @return  none
**/
void suite_execute_tests(uint32_t protocol_identifier)
{
    uint32_t status =  VAL_STATUS_PASS;
    uint32_t num_fail = 0;
    uint32_t num_pass = 0;
    uint32_t num_skip = 0;
    uint32_t index = 0;
    uint32_t num_tests = 0;
    test_fn_ptr *testlist_array = NULL;

    testlist_array = val_get_testlist(protocol_identifier, &num_tests);

    if (testlist_array != NULL) {

        val_print(VAL_PRINT_ERR, "\n      ***  Starting %s tests *** \n",
                val_get_protocol_str(protocol_identifier));

        while (index < num_tests) {
            status = testlist_array[index]();

            switch (status)
            {
            case VAL_STATUS_PASS:
                num_pass++;
                break;
            case VAL_STATUS_FAIL:
                num_fail++;
                break;
            case VAL_STATUS_SKIP:
                num_skip++;
                break;
            }
            val_print(VAL_PRINT_ERR, ": Result: %s", val_get_result_string(status));
            index++;
        }
        val_print(VAL_PRINT_ERR, "\n*************Report *********************");
        val_print(VAL_PRINT_ERR, "\n\tTOTAL TESTS     : %d", num_pass + num_fail + num_skip);
        val_print(VAL_PRINT_ERR, "\n\tTOTAL PASSED    : %d", num_pass);
        val_print(VAL_PRINT_ERR, "\n\tTOTAL FAILED    : %d", num_fail);
        val_print(VAL_PRINT_ERR, "\n\tTOTAL SKIPPED   : %d", num_skip);
        val_print(VAL_PRINT_ERR, "\n******************************************", 0);
    }
}

/**
  @brief   Entry point to SCMI suite
           1. Caller       -  Platform layer.
           2. Prerequisite -  Power domain protocol info table.
  @param   none
  @return  none
**/

void suite_entry(void)
{
    uint32_t protocol_id;

    val_initialize_system();

    val_print(VAL_PRINT_ERR, "\n SCMI Compliance Suite \n ");

    for (protocol_id = PROTOCOL_BASE; protocol_id <= PROTOCOL_MAX; protocol_id++)
        suite_execute_tests(protocol_id);

    val_print(VAL_PRINT_ERR, "\n      *** SCMI tests complete. *** \n");
}
