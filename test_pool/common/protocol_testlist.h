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



test_fn_ptr *val_base_get_testlist(uint32_t *num_tests);
#ifdef POWER_DOMAIN_PROTOCOL
test_fn_ptr *val_power_domain_get_testlist(uint32_t *num_tests);
#endif
#ifdef SYSTEM_POWER_PROTOCOL
test_fn_ptr *val_system_power_get_testlist(uint32_t *num_tests);
#endif
#ifdef CLOCK_PROTOCOL
test_fn_ptr *val_clock_get_testlist(uint32_t *num_tests);
#endif
#ifdef SENSOR_PROTOCOL
test_fn_ptr *val_sensor_get_testlist(uint32_t *num_tests);
#endif
#ifdef PERFORMANCE_PROTOCOL
test_fn_ptr *val_performance_get_testlist(uint32_t *num_tests);
#endif
