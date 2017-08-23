/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <common.h>
#include <juno.h>
#include <test_agent_exports.h>
#include <base_expected.h>
#ifdef POWER_PROTOCOL
#include <power_expected.h>
#endif
#ifdef SYSTEM_POWER_PROTOCOL
#include <system_power_expected.h>
#endif
#ifdef PERFORMANCE_PROTOCOL
#include <performance_expected.h>
#endif
#ifdef CLOCK_PROTOCOL
#include <clock_expected.h>
#endif
#ifdef SENSOR_PROTOCOL
#include <sensor_expected.h>
#endif

#define SCMI_BASE_PROTOCOL_ID 0x10
enum scmi_protocol_ids {
    SCMI_POWER_PROTOCOL_ID=0x11,
    SCMI_SYSTEM_POWER_PROTOCOL_ID=0x12,
    SCMI_PERF_PROTOCOL_ID=0x13,
    SCMI_CLOCK_PROTOCOL_ID=0x14,
    SCMI_SENSOR_PROTOCOL_ID=0x15
};

static struct arm_scmi_base_expected base_expected;
#ifdef POWER_PROTOCOL
static struct arm_scmi_power_expected power_expected;
#endif
#ifdef SYSTEM_POWER_PROTOCOL
static struct arm_scmi_system_power_expected system_power_expected;
#endif
#ifdef CLOCK_PROTOCOL
static struct arm_scmi_clock_expected clock_expected;
#endif
#ifdef SENSOR_PROTOCOL
static struct arm_scmi_sensor_expected sensor_expected;
#endif

char board_variant[8] = {};
char buffer_test_preamble[256] = {};

static void fill_base_expected()
{
    snprintf(buffer_test_preamble, NUM_ELEMS(buffer_test_preamble), "\n Populating base_expected values using board variant %s...\n", board_variant);
    arm_scmi_log_output(buffer_test_preamble);
    base_expected = ospm_base_expectations;
}

#ifdef POWER_PROTOCOL
static void fill_power_expected()
{
    // TO-DO: complete this function for expected values of power protocol
}
#endif

#ifdef SYSTEM_POWER_PROTOCOL
static void fill_system_power_expected()
{
    // TO-DO: complete this function for expected values of system power protocol
}
#endif

#ifdef PERFORMANCE_PROTOCOL
static void fill_performance_power_expected()
{
    // TO-DO: complete this function for expected values of performance protocol
}
#endif

#ifdef CLOCK_PROTOCOL
static void fill_clock_expected()
{
    // TO-DO: complete this function for expected values of clock protocol
}
#endif

#ifdef SENSOR_PROTOCOL
static void fill_sensor_expected()
{
    // TO-DO: complete this function for expected values of sensor protocol
}
#endif

/*!
 * @brief Function used to print usage message.
 *
 *  The function is called when arm_scmi_verify_arg returns error.
 *
 *  @param None.
 *
 */
static void arm_scmi_usage()
{
    snprintf(buffer_test_preamble, NUM_ELEMS(buffer_test_preamble), "Usage: \n");
    arm_scmi_log_output(buffer_test_preamble);
    snprintf(buffer_test_preamble, NUM_ELEMS(buffer_test_preamble), "\t -board <r0|r1|r2> \n");
    arm_scmi_log_output(buffer_test_preamble);
}

/*!
 * @brief Function used to verify the board variant.
 *
 *  The function is called by the juno main function to verify the board
 *  variant. NULL or unsupported board version will be reported as EXIT_ON_ERROR,
 *  and help information will be printed to inform the test of supported
 *  values.
 *
 *  @param None.
 *
 */
static int arm_scmi_verify_arg()
{
    if(strcmp(board_variant, "") == 0) {
        snprintf(buffer_test_preamble, NUM_ELEMS(buffer_test_preamble), "Please specify board variant.\n");
        arm_scmi_log_output(buffer_test_preamble);
        arm_scmi_usage();
        return EXIT_ON_ERROR;
    }
    else{
        char variant = board_variant[0];
        char variant_number = board_variant[1];
        switch (variant)
        {
            case 'r':
                switch (variant_number)
                {
                    case '0':
                    case '1':
                    case '2':
                        snprintf(buffer_test_preamble, NUM_ELEMS(buffer_test_preamble), "Testing on Juno-r%c\n", variant_number);
                        arm_scmi_log_output(buffer_test_preamble);
                        break;
                    default:
                        snprintf(buffer_test_preamble, NUM_ELEMS(buffer_test_preamble), "Unknown Juno variant_number %c \n", variant_number);
                        arm_scmi_log_output(buffer_test_preamble);
                        arm_scmi_usage();
                        return EXIT_ON_ERROR;
                }
                break;
            default:
                snprintf(buffer_test_preamble, NUM_ELEMS(buffer_test_preamble), "Unknown Juno variant %s.\n", board_variant);
                arm_scmi_log_output(buffer_test_preamble);
                arm_scmi_usage();
                return EXIT_ON_ERROR;
        }
    }

    return NO_ERROR;
}

int main(int argc, char *argv[])
{
    struct arm_scmi_platform_info platform_details;
    struct arm_scmi_test_report test_report;

    arm_scmi_create_log();

    /* extract and verify the board variant */
    size_t counter;
    for(counter=1; counter<argc;) {
        if(strcmp(argv[counter], "-board") == 0 &&
                argc > 2) {
            strcpy(board_variant, argv[counter+1]);
        }
        counter = counter + 2;
    }
    int return_value = arm_scmi_verify_arg();
    if(return_value == EXIT_ON_ERROR) {
        return EXIT_ON_ERROR;
    }
    else {
        snprintf(buffer_test_preamble, NUM_ELEMS(buffer_test_preamble), "Running test ... \n");
        arm_scmi_log_output(buffer_test_preamble);
    }

    fill_base_expected();

#ifdef POWER_PROTOCOL
    fill_power_expected();
#endif
#ifdef SYSTEM_POWER_PROTOCOL
    fill_system_power_expected();
#endif
#ifdef PERFORMANCE_PROTOCOL
    fill_performance_power_expected();
#endif
#ifdef CLOCK_PROTOCOL
    fill_clock_expected();
#endif
#ifdef SENSOR_PROTOCOL
    fill_sensor_expected();
#endif
    struct arm_scmi_platform_protocol_info protocols[] =
        {
            {
                .protocol_id = SCMI_BASE_PROTOCOL_ID,
                .expected_return_values = (void *)&base_expected
            },
#ifdef POWER_PROTOCOL
            {
                .protocol_id = SCMI_POWER_PROTOCOL_ID,
                .expected_return_values = (void *)&power_expected
            },
#endif
#ifdef SYSTEM_POWER_PROTOCOL
            {
                .protocol_id = SCMI_SYSTEM_POWER_PROTOCOL_ID,
                .expected_return_values = (void *)&system_power_expected
            },
#endif
#ifdef PERFORMANCE_PROTOCOL
            {
                .protocol_id = SCMI_PERF_PROTOCOL_ID,
                .expected_return_values = NULL
            },
#endif
#ifdef CLOCK_PROTOCOL
            {
                .protocol_id = SCMI_CLOCK_PROTOCOL_ID,
                .expected_return_values = NULL
            },
#endif
#ifdef SENSOR_PROTOCOL
            {
                .protocol_id = SCMI_SENSOR_PROTOCOL_ID,
                .expected_return_values = NULL
            },
#endif
        };

    platform_details.platform_protocol_info_table = protocols;
    platform_details.platform_protocol_info_count =
        sizeof(protocols) / sizeof(protocols[0]);
    arm_scmi_agent_execute(&platform_details, NULL, &test_report);
    return NO_ERROR;
}

