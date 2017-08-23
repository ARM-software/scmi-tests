/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <juno.h>

/*!
 * @brief Function implemented by user to open the log file.
 * This function is only used once in the beginning of the
 *
 * @param output pointer to the formatted output string buffer.
 *
 */
void arm_scmi_create_log()
{
      FILE* file_ptr = fopen(LOG_FILE, "w");
      fclose(file_ptr);
}

/*!
 * @brief Function implemented by user to handle
 * output from the test agent.
 * For Juno, the output is redirected to a separate file
 * that lives in the same level as the executable binary.
 *
 * @param output pointer to the formatted output string buffer.
 *
 */
void arm_scmi_log_output(const char* output)
{
    FILE* file_ptr = fopen(LOG_FILE, "a");
    fprintf(file_ptr, "%s", output);
    fclose(file_ptr);

}

