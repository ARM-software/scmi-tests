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

#include "val_interface.h"

/**
  @brief   Entry point to SCMI suite
           1. Caller       -  Platform layer.
           2. Prerequisite -  Power domain protocol info table.
  @param   none
  @return  none
**/

int main(int argc, char *argv[])
{
    uint32_t num_pass;
    uint32_t num_fail;
    uint32_t num_skip;

    val_print(VAL_PRINT_ERR, "\n\n        **** SCMI Compliance Suite **** ");

    if (val_initialize_system((void *) NULL)) {
        val_print(VAL_PRINT_ERR, "\n **** INIT FAILED ***");
        return 0;
    }

    val_print(VAL_PRINT_ERR, "\n\n          *** Starting BASE tests ***");
    val_base_execute_tests();

#ifdef POWER_DOMAIN_PROTOCOL
    val_print(VAL_PRINT_ERR, "\n\n          *** Starting POWER tests *** ");
    val_power_domain_execute_tests();
#endif

#ifdef SYSTEM_POWER_PROTOCOL
    val_print(VAL_PRINT_ERR, "\n\n          *** Starting SYSTEM POWER tests ***  ");
    val_system_power_execute_tests();
#endif

#ifdef PERFORMANCE_PROTOCOL
    val_print(VAL_PRINT_ERR, "\n\n          *** Starting PERFORMANCE tests ***  ");
    val_performance_execute_tests();
#endif

 #ifdef CLOCK_PROTOCOL
    val_print(VAL_PRINT_ERR, "\n\n          *** Starting CLOCK tests ***");
    val_clock_execute_tests();
#endif

#ifdef SENSOR_PROTOCOL
    val_print(VAL_PRINT_ERR, "\n\n          *** Starting SENSOR tests ***  ");
    val_sensor_execute_tests();
#endif

#ifdef RESET_PROTOCOL
    val_print(VAL_PRINT_ERR, "\n\n          *** Starting RESET tests ***  ");
    val_reset_execute_tests();
#endif

    num_pass = val_get_test_passed();
    num_fail = val_get_test_failed();
    num_skip = val_get_test_skipped();

    val_print(VAL_PRINT_ERR, "\n****************************************************", 0);
    val_print(VAL_PRINT_ERR, "\n  TOTAL TESTS: %d", num_pass + num_fail + num_skip);
    val_print(VAL_PRINT_ERR, "    PASSED: %d", num_pass);
    val_print(VAL_PRINT_ERR, "    FAILED: %d", num_fail);
    val_print(VAL_PRINT_ERR, "    SKIPPED: %d", num_skip);
    val_print(VAL_PRINT_ERR, "\n****************************************************", 0);

    val_print(VAL_PRINT_ERR, "\n\n        **** SCMI tests complete **** \n ");

    return 1;
}
