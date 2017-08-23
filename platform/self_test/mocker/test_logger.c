/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

/*!
 * @brief Function implemented by user to handle
 * output from the test agent.
 *
 * @param output pointer to the formatted output string buffer.
 *
 */
void arm_scmi_log_output(const char* output)
{
	printf("%s", output);
}

