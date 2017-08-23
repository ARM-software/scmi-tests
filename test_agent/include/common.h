/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#ifndef COMMON_H_
#define COMMON_H_

#include <arm_scmi_constants.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

#define NUM_ELEMS(x)  (sizeof(x) / sizeof((x)[0]))
#define COUNT_TEST_CASES(TEST_CASES) ((sizeof(TEST_CASES) / \
	sizeof(arm_scmi_test_case_t)) - 1) /* -1 for END_OF_TEST_LIST */
/*
 * These macros are used to compute the offset values of return and parameters
 * fields.
 */
#define OFFSET_BYTES_RET(st, elem) \
	((offsetof(st, returns.elem) - offsetof(st, returns)))
#define OFFSET_RET(st, elem) \
	(OFFSET_BYTES_RET(st, elem)/sizeof(uint32_t))
/*
 * Compute the offset of a parameter within a structure.
 */
#define OFFSET_PARAM(st, elem) \
(((offsetof(st, parameters.elem) - offsetof(st, parameters)))/sizeof(uint32_t))
/*
 * Compute the altered offset of an invalid parameter within a test specification
 * structure.
 */
#define OFFSET_INVALID_PARAM(st, elem) (OFFSET_PARAM(st, elem) + 1)
/*
 * Compute the offset of an invalid parameter from an altered parameter
 * offset.
 */
#define OFFSET_FROM_INVALID(invalid_offset) (invalid_offset - 1)
/*
 * Retrieve an integer return value. The value is retrieved from the message
 * payload stored in "received". The offset in the payload array is computed
 * from the offset of the "elem" field in the C structure "st" mapping the
 * layout of the returned values.
 */
#define RETURN_VAL(received, st, elem) (received.payload[OFFSET_RET(st, elem)])
/*
 * Retrieve an integer parameter value. The value is retrieved from the array of
 * parameters "parameters". The offset in the array is computed from the offset
 * of the "elem" field in the C structure "st" mapping the layout of the
 * parameters.
 */
#define PARAM_VAL(parameters, st, elem) (parameters[OFFSET_PARAM(st, elem)])
/*
 * Retrieve a pointer to a string returned value. The pointer to the string is
 * computed from the offset of the "elem" field in the C structure "st" mapping
 * the layout of the returned values.
 */
#define RETURN_STR(received, st, elem) \
	((char *)(&received.payload[OFFSET_RET(st, elem)]))
#define FIELD_SIZE_BYTES(type, member) sizeof(((type *)0)->member)
#define T_EOT_T {.type = T_EOT}
#define CHECK 1
#define PRINT 0
/*
 * We chose deliberately a reserved value for this 'special' flag which
 * is a 'hint' for the test engine that if this value is given in status field,
 * then don't check for any specific return values. For example, if the test
 * case was checking if an optional command is implemented or not; the result
 * could be either SUCCESS or NOT_FOUND depending on the platform's
 * implementation status. So seeing this in the expected value tell's the
 * executor *not* to compare against any status value just run the command and
 * report the outputs as reported by the platform.
 */
enum arm_scmi_error_codes {
	SCMI_STATUS_SUCCESS = 0,
	SCMI_STATUS_NOT_SUPPORTED = -1,
	SCMI_STATUS_INVALID_PARAMETERS = -2,
	SCMI_STATUS_DENIED = -3,
	SCMI_STATUS_NOT_FOUND = -4,
	SCMI_STATUS_OUT_OF_RANGE = -5,
	SCMI_STATUS_BUSY = -6,
	SCMI_STATUS_COMMS_ERROR = -7,
	SCMI_STATUS_GENERIC_ERROR = -8,
	SCMI_STATUS_HARDWARE_ERROR = -9,
	SCMI_STATUS_PROTOCOL_ERROR = -10,
	SCMI_STATUS_NOT_SPECIFIED = -100
};

/* Create a 1-bits mask from msb position to lsb positions in an uint32_t */
#define BIT_MASK(msb, lsb) ~(~0 << (msb - lsb + 1)) << lsb
#define RESERVED 0x0
#define TOKEN 0x0
#define NO_ERROR 0
#define EXIT_ON_ERROR 1
#define LEFT_SHIFT(dec) (1 << dec)

enum message_types {
	COMMAND=0,
	DELAY_RESP=2,
	NOTIFICATIONS=3
};
/* Test status values */
enum test_status {
	UNDEFINED = 0,
	PASSED,
	FAILED,
	SKIP,
	INFO,
	ERROR,
	CONFORMANT,
	NON_CONFORMANT,
	UNTESTED
};

/* Precondition actions */
enum precondition_actions {
	PRECOND_NONE,
	PRECOND_DEFINE,
	PRECOND_USE
};

/*
 * Structure used to handle data sent/received from the platform.
 */
struct arm_scmi_message {
	uint8_t reserved;				/* Field bits 31:28 */
	uint16_t token;					/* Field bits 27:18 */
	uint8_t protocol_id;			/* Field bits 17:10 */
	uint8_t message_type;			/* Field bits 09:08 */
	uint8_t message_id;				/* Field bits 07:00 */
	int32_t status;					/* Field bits 63:32 */
	size_t payload_size;			/* Size in bytes not including status */
	uint32_t *payload;				/* Field bits ..:64 */
	enum bits_header {
		RESERVED_LOW=28,
		RESERVED_HIGH=31,
		TOKEN_LOW=18,
		TOKEN_HIGH=27,
		PROTOCOL_ID_LOW=10,
		PROTOCOL_ID_HIGH=17,
		MESSAGE_TYPE_LOW=8,
		MESSAGE_TYPE_HIGH=9,
		MESSAGE_ID_LOW=0,
		MESSAGE_ID_HIGH=7
	} bits_header;
};

/*
 * Typedefs for hook functions
 */
typedef struct arm_scmi_test_case arm_scmi_test_case_t; /* Forward declarations */
struct arm_scmi_protocol_execution_context;
typedef void (*get_return_values_func_t)(arm_scmi_test_case_t *);
typedef void (*post_proccesing_func_t)(
		struct arm_scmi_protocol_execution_context *,
		arm_scmi_test_case_t *,
		struct arm_scmi_message *);
typedef void (*custom_tester_func_t)(
		struct arm_scmi_protocol_execution_context *,
		arm_scmi_test_case_t *);

/* Return value types */
typedef enum {
	/* T_EOT must be the first then if not expected returns
	 * are defined, T_EOT it is used by default.
	 */
	/* End of table marker to avoid the burden of defining the size. */
    T_EOT = 0,
    T_UINT32_HEX, /* Expected a uint32_t in hex format */
	T_UINT32_DEC, /* Expected a uint32_t in decimal format */
    T_UINT16,
    T_UINT8,
    T_BIT_FIELD,
    T_STRING
} return_value_type_t;

typedef struct return_value_def_t {
    char *name;
    unsigned int offset;
    size_t len;
    /* Not used for non-bit field type */
    uint8_t msb;
    uint8_t lsb;
} return_value_def_t;

/* Holds the value of a returned value */
typedef union {
    uint32_t integer;
    char *string;
} return_value_t;

/* Return value description, i.e. definition and value */
typedef struct {
    /* Flags: bit 0:
     * 1, Value to be checked
     * 0, Value to be printed only
     */
    unsigned int flags;
    return_value_type_t type;
    /* Pointer to be able to use the same description several times. */
    return_value_def_t *def;
    return_value_t value;
} return_value_desc_t;

/*
 * Definition of a test suite; essentially an array of test cases.
 */
typedef struct arm_scmi_test_suite {
	arm_scmi_test_case_t *test_case_table;
	const char *name;
	bool skip; /* Indicates if the test suite must be skipped (true)*/
} arm_scmi_test_suite_t;

/*
 *
 * Declaration of structure to hold a test case
 *
 */
struct arm_scmi_test_case {
	/* This provides a string ID in the format
	 * [protocol]-{COMMAND}-<description>-<testcasecount>
	*/
	const char* testcase_id;
	/* Protocol and message id for the test case command */
	uint32_t protocol_id;
	uint32_t message_id;
	/*
	 * This is a 'hint' flag. A TRUE value for this means that the parameters
	 * for this test case need to be determined from the discovery phase;
	 * otherwise the parameters are defined statically.
	 */
	bool parameter_discovery_driven;
	/*
	 * This is a 'hint' flag. A TRUE value for this means that the specified
	 * command needs to be sent as an Async command during testing if the Async
	 * command flag is supported.
	 */
	bool send_cmd_as_async;
	/*
	 * This is a 'hint' timeout counter. A non-zero value for this specify how
	 * much timeout needs to be applied for a delayed response if the command is
	 * issued as Async.
	 */
	uint32_t timeout_in_sec;
	/*
	 * This is a 'hint' flag to tell which input parameter needs to have the
	 * invalid argument in a negative test case; otherwise is ignored. A special
	 * macro must be used to set this value.
	 */
	uint32_t invalid_parameter;

	/* Parameters array */
	uint32_t parameter_count;
	uint32_t *parameters;

	/* Expected status */
	int32_t status;

	/* Expected returned data structure */
	return_value_desc_t *expected_returns;

	/* Field for precondition action carried by current test case */
	uint8_t precondition_indicator;
	/*
	 * Hooks called by the test execution engine
	 */
	/*
	 * Hook to be called just before sending a command and test the received
	 * data and generally used to fill the expected return table for the command
	 * and possibly set other hooks.
	 */
	get_return_values_func_t get_return_values_func;
	/*
	 * Hook to be called after filling the expected data but before sending
	 * the command hence it is used to override generic sending & testing.
	 */
	custom_tester_func_t custom_tester_func;
	/* Hook to be called just after testing the command and it is used to do a
	 * post-processing related to test results and the received data.
	 */
	post_proccesing_func_t post_proccesing_func;
};

#endif /* COMMON_H_ */
