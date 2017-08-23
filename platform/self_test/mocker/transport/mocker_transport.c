/*
* Copyright (c) 2017, ARM Limited and Contributors. All rights reserved.
*
* SPDX-License-Identifier: Apache-2.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <test_helper.h>
#include <platform_exports.h>
#include <test_execute.h>
#include <base_common.h>
#ifdef POWER_PROTOCOL
#include <power_common.h>
#endif
#ifdef SYSTEM_POWER_PROTOCOL
#include <system_power_common.h>
#endif
#ifdef PERFORMANCE_PROTOCOL
#include <performance_common.h>
#endif
#ifdef CLOCK_PROTOCOL
#include <clock_common.h>
#endif
#ifdef SENSOR_PROTOCOL
#include <sensor_common.h>
#endif
#define NUMBER_CLOCKS 4
#define NUMBER_SENSORS 4
#define MAX_NUM_ASYNC_RATES_CHGS_SPRT 8

/*!
 * @brief Interface function that sends a
 * command and receives a platform
 * message.
 *
 * This is the mocker platform implementation of the send_message interface
 * which is used by the test_agent to send commands to the platform over A2P
 * channel. In this case it fakes some dummy data and returns that data to the
 * calling test_agent to enable testing of the test harness.
 *
 */
int arm_scmi_send_message(uint32_t message_header_send, size_t parameter_count,
		const uint32_t *parameters, uint32_t *message_header_rcv, int32_t *status,
		size_t *return_values_count, uint32_t *return_values)
{
	uint32_t offset;
	int i,j,k;
	char *str; // for re-casting the uint8_t to char
	/*base protocol*/
	int num_agents = 1;
	int num_protocols = 5;
	int protocol_id;
	int message_id;
	int parameter_id;

#ifdef POWER_PROTOCOL
	/*power protocol*/
	int num_power_domains = 3;
	int stats_low = 0x1234, stats_high = stats_low + 0xff;
#endif

#ifdef SYSTEM_POWER_PROTOCOL
	/*system power management protocol*/
	/*Mocker platform supports warm reset and system suspend
	 * 0 for not supporting
	 * 1 for supporting
	 * */
	int warm_reset_support = 1, system_suspend_support = 1;
#endif

#ifdef PERFORMANCE_PROTOCOL
	/*perf protocol*/
	int num_perf_domains = 2, perf_stats_high = 0, perf_stats_low = 0;
	int perf_levels = 2;
#endif

#ifdef CLOCK_PROTOCOL
	/*clock protocol*/
	int clock_rates[NUMBER_CLOCKS] = {3, 2, 5, 4};
#endif

#ifdef SENSOR_PROTOCOL
	int sensor_idx;
	/*sensor protocol*/
	/*Number of sensor descriptors platform is able to return for each query*/
	int number_sensor_descriptors = NUMBER_SENSORS;
	int number_trip_points[NUMBER_SENSORS] = {3, 2, 1, 4};
	/*Here prepare some mocker sensor descriptors*/
	struct arm_scmi_sensor_descriptor mocker_sensor_desc[NUMBER_SENSORS] = {
			{
				.sensor_id = 0,
				/*here we have async sensor read support disabled
				 * and 2 supported trip points*/
				.sensor_attributes_low = (0 << SNR_DESC_ATTRLOW_RESV_LOW) +
										 (0 << SNR_DESC_ATTRLOW_RESV_LOW) + 2,
				/*here we have 0 update interval, 0 multiplier and sensor type 2*/
				.sensor_attributes_high = (0 << SNR_DESC_ATTRHIGH_UPDATE_INTERVAL_SEC_LOW) +
										  (0 << SNR_DESC_ATTRHIGH_UPDATE_INTERVAL_MULT_LOW) +
										  (0 << SNR_DESC_ATTRHIGH_RESV_UPPER_LOW) +
										  (0 << SNR_DESC_ATTRHIGH_MULTIPLIER_LOW) +
										  (0 << SNR_DESC_ATTRHIGH_RESV_LOWER_LOW)+
										  (2 << SNR_DESC_ATTRHIGH_SNR_TYPE_LOW),
				/*here we have an array of decimal ASCII representation of the sensor name "SNSR-0"
				 * Letter "S": 83,
				 * Letter "N": 78,
				 * Letter "S": 83,
				 * Letter "R": 82,
				 * Symbol "-": 45
				 * Number "0": 48*/
				/*Need to cast to uint32_t as of the type of our *received pointer*/
				//.sensor_name = {83, 78, 83, 82, 45, 48}
			  .sensor_name = {}
			},
			{
				.sensor_id = 1,
				/*here we have async sensor read support disabled
				 * and 4 supported trip points*/
				.sensor_attributes_low = (0 << SNR_DESC_ATTRLOW_RESV_LOW) +
										 (0 << SNR_DESC_ATTRLOW_RESV_LOW) + 4,
				/*here we have an update interval of 5x10^1 seconds and sensor type 3*/
				.sensor_attributes_high = (5 << SNR_DESC_ATTRHIGH_UPDATE_INTERVAL_SEC_LOW) +
										  (1 << SNR_DESC_ATTRHIGH_UPDATE_INTERVAL_MULT_LOW) +
										  (0 << SNR_DESC_ATTRHIGH_RESV_UPPER_LOW) +
										  (0 << SNR_DESC_ATTRHIGH_MULTIPLIER_LOW) +
										  (0 << SNR_DESC_ATTRHIGH_RESV_LOWER_LOW)+
										  (3 << SNR_DESC_ATTRHIGH_SNR_TYPE_LOW),
				/*here we have an array of decimal ASCII representation of the sensor name "SNSR-1"*/
				//.sensor_name = {83, 78, 83, 82, 45, 49}
				.sensor_name = {}

			},
			{
				.sensor_id = 2,
				/*here we have async sensor read support disabled
				 * and 5 supported trip points*/
				.sensor_attributes_low = (0 << SNR_DESC_ATTRLOW_RESV_LOW) +
										 (0 << SNR_DESC_ATTRLOW_RESV_LOW) + 3,
				/*here we have an update interval of 2x10^3 seconds and sensor type 4*/
				.sensor_attributes_high = (2 << SNR_DESC_ATTRHIGH_UPDATE_INTERVAL_SEC_LOW) +
										  (3 << SNR_DESC_ATTRHIGH_UPDATE_INTERVAL_MULT_LOW) +
										  (0 << SNR_DESC_ATTRHIGH_RESV_UPPER_LOW) +
										  (0 << SNR_DESC_ATTRHIGH_MULTIPLIER_LOW) +
										  (0 << SNR_DESC_ATTRHIGH_RESV_LOWER_LOW)+
										  (4 << SNR_DESC_ATTRHIGH_SNR_TYPE_LOW),
				/*here we have an array of decimal ASCII representation of the sensor name "SNSR-2"*/
				//.sensor_name = {83, 78, 83, 82, 45, 50}
				.sensor_name = {}
			},
			{
				.sensor_id = 3,
				/*here we have async sensor read support disabled
				 * and 7 supported trip points*/
				.sensor_attributes_low = (0 << SNR_DESC_ATTRLOW_RESV_LOW) +
										 (0 << SNR_DESC_ATTRLOW_RESV_LOW) + 1,
				/*here we have an update interval of 9x10^3 seconds and sensor type 4*/
				.sensor_attributes_high = (9 << SNR_DESC_ATTRHIGH_UPDATE_INTERVAL_SEC_LOW) +
										  (3 << SNR_DESC_ATTRHIGH_UPDATE_INTERVAL_MULT_LOW) +
										  (0 << SNR_DESC_ATTRHIGH_RESV_UPPER_LOW) +
										  (0 << SNR_DESC_ATTRHIGH_MULTIPLIER_LOW) +
										  (0 << SNR_DESC_ATTRHIGH_RESV_LOWER_LOW)+
										  (4 << SNR_DESC_ATTRHIGH_SNR_TYPE_LOW),
				/*here we have an array of decimal ASCII representation of the sensor name "SNSR-2"*/
				//.sensor_name = {83, 78, 83, 82, 45, 51}
				.sensor_name = {}
			}
	};
#endif

	*message_header_rcv = message_header_send;
	protocol_id = arm_scmi_get_norm_bits(message_header_send,
			PROTOCOL_ID_HIGH, PROTOCOL_ID_LOW);
	message_id = arm_scmi_get_norm_bits(message_header_send,
			MESSAGE_ID_HIGH, MESSAGE_ID_LOW);
	switch (protocol_id)
	{
		case BASE_PROTO_ID:
			switch (message_id)
			{
				case BASE_PROTO_VERSION_MSG_ID:
					*status = SCMI_STATUS_SUCCESS;
					*return_values_count = 1;
					return_values[0] = 0x00010000; //PROTOCOL_VERSION;
					break;
				case BASE_PROTO_ATTR_MSG_ID:
					*status = SCMI_STATUS_SUCCESS;
					*return_values_count = 1;
					return_values[0] = (num_agents << 8) + num_protocols;
					break;
				case BASE_PROTO_MSG_ATTR_MSG_ID:
					*status = SCMI_STATUS_SUCCESS;
					*return_values_count = 1;
					return_values[0] = 0x0;
					break;
				case BASE_DISC_VENDOR_MSG_ID:
					*status = SCMI_STATUS_SUCCESS;
					sprintf((char *)return_values, "VENDOR 1");
					*return_values_count = strlen((const char*)return_values);
					break;
				case BASE_DISC_SUBVENDOR_MSG_ID:
					*status = SCMI_STATUS_SUCCESS;
					sprintf((char *)return_values, "SUBVENDOR 1");
					*return_values_count = strlen((const char*)return_values);
					break;
				case BASE_DISC_IMPLEMENTATION_VERSION_MSG_ID:
					*status = SCMI_STATUS_SUCCESS;
					*return_values_count = 1;
					return_values[0] = 0x1;
					break;
				case BASE_DISC_PROTO_LIST_MSG_ID:
					*status = (parameters[0] > num_protocols ?
						SCMI_STATUS_INVALID_PARAMETERS: SCMI_STATUS_SUCCESS);
					*return_values_count = num_protocols + 1;
					return_values[0] = num_protocols;
					return_values[1] = 0x11 | 0x13 << 8 | 0x14 << 16 | 0x15 << 24;
					return_values[2] = 0x12;
					break;
				case BASE_DISC_AGENT_MSG_ID:
					parameter_id = OFFSET_PARAM(
						struct arm_scmi_base_discover_agent, agent_id);
					if (parameters[parameter_id] > num_agents) {
						*status = SCMI_STATUS_NOT_FOUND;
					} else {
						*status = parameters[parameter_id];
						sprintf((char *)return_values,
				(parameters[parameter_id] == 0 ? "platform_mock" : "agent"));
					*return_values_count = strlen((const char*)return_values);
					}
					break;
				case BASE_NOTIFY_ERRORS_MSG_ID:
					*status = SCMI_STATUS_SUCCESS;
					break;
				default:
					assert(!"\nUnknown message id");
			}
			break;
#ifdef POWER_PROTOCOL
			case PWR_PROTO_ID:
				switch (message_id)
				{
					case PWR_PROTO_VER_MSG_ID:
						*status = SCMI_STATUS_SUCCESS;
						*return_values_count = 1;
						return_values[0] = 0x00010000; //PROTOCOL_VERSION;
						break;
					case PWR_PROTO_ATTR_MSG_ID:
						*status = SCMI_STATUS_SUCCESS;
						*return_values_count = 1;
						return_values[OFFSET_RET(
							struct arm_scmi_power_protocol_attributes,
							attributes)] =
								(num_power_domains <<
										POWER_PROT_ATTR_NUMBER_DOMAINS_LOW);
						return_values[OFFSET_RET(
							struct arm_scmi_power_protocol_attributes,
							statistics_address_low)] = stats_low;
						return_values[OFFSET_RET(
							struct arm_scmi_power_protocol_attributes,
							statistics_address_high)] = stats_high;
						return_values[OFFSET_RET(
							struct arm_scmi_power_protocol_attributes,
							statistics_len)] = stats_high - stats_low;
						break;
					case PWR_PROTO_MSG_ATTR_MSG_ID:
						*status = SCMI_STATUS_SUCCESS;
						*return_values_count = 1;
						return_values[OFFSET_RET(
							struct arm_scmi_power_protocol_message_attributes,
							attributes)] = 0x0;
						break;
					case PWR_DOMAIN_ATTRIB_MSG_ID:
						*status = (parameters[OFFSET_PARAM(
								struct arm_scmi_power_domain_attributes,
								domain_id)] > num_power_domains ?
								SCMI_STATUS_NOT_FOUND: SCMI_STATUS_SUCCESS);
						*return_values_count = 3;
						return_values[OFFSET_RET(
								struct arm_scmi_power_domain_attributes,
								attributes)] =
								(1 << POWER_DOMAIN_ATTR_SYNC_LOW) +
								(1 << POWER_DOMAIN_ATTR_ASYNC_LOW) +
								(1 << POWER_DOMAIN_ATTR_STATE_NOTIFICATIONS_LOW);
						str = (char *)
						(&return_values[OFFSET_RET(
								struct arm_scmi_power_domain_attributes, name)]);
						sprintf(str, "Domain_%d", parameters[
						OFFSET_PARAM(struct arm_scmi_power_domain_attributes,
								domain_id)]);
						break;
					case PWR_STATE_SET_MSG_ID:
						if (parameters[OFFSET_PARAM(struct arm_scmi_power_state_set,
								domain_id)] >= num_power_domains) {
							*status = SCMI_STATUS_NOT_FOUND;
							break;
						}
						*status = SCMI_STATUS_SUCCESS;
						break;
					case PWR_STATE_GET_MSG_ID:
						if (parameters[OFFSET_PARAM(
								struct arm_scmi_power_state_get, domain_id)] >=
								num_power_domains) {
							*status = SCMI_STATUS_NOT_FOUND;
							break;
						}
						*status = SCMI_STATUS_SUCCESS;
						*return_values_count = 1;
						return_values[OFFSET_RET(struct arm_scmi_power_state_get,
								power_state)] = 0xf;
						break;
					case PWR_STATE_NOTIFY_MSG_ID:
						if (parameters[OFFSET_PARAM(
								struct arm_scmi_power_state_notify,
								domain_id)] >= num_power_domains) {
							*status = SCMI_STATUS_NOT_FOUND;
							break;
						}
						*status = SCMI_STATUS_SUCCESS;
						break;
					default:
						printf("\nMessage id--->%0x", message_id);
						assert(!"\nUnknown message id");
				}
			break;
#endif
#ifdef SYSTEM_POWER_PROTOCOL
			case SYSTEM_POWER_PROTO_ID:
				switch (message_id)
				{
					case SYSTEM_POWER_PROTO_VER_MSG_ID:
						*status = SCMI_STATUS_SUCCESS;
						*return_values_count = 1;
						return_values[0] = 0x00010000; //PROTOCOL_VERSION;
						break;
					case SYSTEM_POWER_PROTO_ATTRIB_MSG_ID:
						*status = SCMI_STATUS_SUCCESS;
						*return_values_count = 1;
						return_values[0] = 0x0;
						break;
					case SYSTEM_POWER_PROTO_MSG_ATTRIB_MSG_ID:
						if (parameters[OFFSET_PARAM(
								struct arm_scmi_system_power_protocol_message_attributes,
								message_id)] != SYSTEM_POWER_STATE_SET_MSG_ID) {
							/*except for SYSPWR_PROTO_MSG_ATTRIB_MSG_ID, return 0*/
							*status = SCMI_STATUS_SUCCESS;
							*return_values_count = 1;
							return_values[0] = 0x0;
						}
						else {
							*status = SCMI_STATUS_SUCCESS;
							*return_values_count = 1;
							/*System warm reset and suspend are supported*/
							return_values[0] = (warm_reset_support << 31) +
									(system_suspend_support << 30) + 0;
						}
						break;
					case SYSTEM_POWER_STATE_SET_MSG_ID:
						if ((parameters[OFFSET_PARAM(
								struct arm_scmi_system_power_state_set,
									system_state)] >= 0x5) &&
							(parameters[OFFSET_PARAM(
								struct arm_scmi_system_power_state_set,
									system_state)] <= 0x7fffffff)) {
							*status = SCMI_STATUS_INVALID_PARAMETERS;
						}
						else {
							/* For future extensions of testing valid power state */
						}
						break;
					case SYSTEM_POWER_STATE_GET_MSG_ID:
						*status = SCMI_STATUS_SUCCESS;
						break;
					default:
						printf("\nMessage id--->%0x", message_id);
						assert(!"\nUnknown message id");
				}
			break;
#endif
#ifdef PERFORMANCE_PROTOCOL
			case PERF_MGMT_PROTO_ID:
				switch (message_id)
				{
					case PERF_MGMT_PROTO_VER_MSG_ID:
						*status = SCMI_STATUS_SUCCESS;
						*return_values_count = 1;
						return_values[0] = 0x00010000; //PROTOCOL_VERSION;
						break;
					case PERF_MGMT_PROTO_ATTRIB_MSG_ID:
						*status = SCMI_STATUS_SUCCESS;
						*return_values_count = 1;
						return_values[OFFSET_RET(
							struct arm_scmi_performance_protocol_attributes,
							attributes)] =
								(1 << PERFORMANCE_PROT_POWER_VALUES_LOW) +
								(num_perf_domains <<
									PERFORMANCE_PROT_ATTR_NUMBER_DOMAINS_LOW);
						return_values[OFFSET_RET(
							struct arm_scmi_performance_protocol_attributes,
							statistics_address_low)] = perf_stats_low;
						return_values[OFFSET_RET(
							struct arm_scmi_performance_protocol_attributes,
							statistics_address_high)] = perf_stats_high;
						return_values[OFFSET_RET(
							struct arm_scmi_performance_protocol_attributes,
							statistics_len)] = perf_stats_high - perf_stats_low;
						break;
					case PERF_MGMT_PROTO_MSG_ATTRIB_MSG_ID:
						*status = SCMI_STATUS_SUCCESS;
						*return_values_count = 1;
						return_values[OFFSET_RET(
							struct arm_scmi_performance_protocol_message_attributes,
							attributes)] = 0x0;
						break;
					case PERF_DOMAIN_ATTRIB_MSG_ID:
						*status = (parameters[OFFSET_PARAM(
								struct arm_scmi_performance_domain_attributes,
								domain_id)] >= num_perf_domains ?
								SCMI_STATUS_NOT_FOUND: SCMI_STATUS_SUCCESS);
						return_values[OFFSET_RET(
								struct arm_scmi_performance_domain_attributes,
								attributes)] =
								(1 << PERFORMANCE_DOMAIN_ATTR_LIMIT_SET) |
								(1 << PERFORMANCE_DOMAIN_ATTR_LEVEL_SET);
						return_values[OFFSET_RET(
								struct arm_scmi_performance_domain_attributes,
								rate_limit)] = 0;
						return_values[OFFSET_RET(
								struct arm_scmi_performance_domain_attributes,
								sustained_perf_level)] = (parameters[0] + 1) * 11;
						return_values[OFFSET_RET(
								struct arm_scmi_performance_domain_attributes,
								sustained_freq)] = (parameters[0] + 1) * 10;
						str = (char *)
						(&return_values[OFFSET_RET(
							struct arm_scmi_performance_domain_attributes, name)]);
						sprintf(str, "Domain_%d", parameters[
						OFFSET_PARAM(struct arm_scmi_performance_domain_attributes,
							domain_id)]);
						break;
					case PERF_DESC_LVL_MSG_ID:
						if (parameters[0] >= num_perf_domains) {
							*status = SCMI_STATUS_NOT_FOUND;
							break;
						} else if (parameters[1] >= perf_levels) {
							*status = SCMI_STATUS_INVALID_PARAMETERS;
							break;
						}
						*status = SCMI_STATUS_SUCCESS;
						offset = OFFSET_RET(
						struct arm_scmi_performance_describe_levels, num_levels);
						return_values[offset] =
						(0 << PERFORMANCE_DESC_LEVELS_NUM_LEVELS_REMAINING_LOW) |
						(perf_levels << PERFORMANCE_DESC_LEVELS_NUM_LEVELS_LOW);
						offset = OFFSET_RET(
						struct arm_scmi_performance_describe_levels, perf_levels);
						k = 0;
						for (i = 0; i < num_perf_domains; ++i)
							for (j = 0; j < perf_levels; ++j) {
								return_values[offset + k] =
										(parameters[0] + 1) * 10  + (j + 1);
								return_values[offset + k  + 1] =
										(parameters[0] + 1) * 100 + j;
								return_values[offset + k + 2] =
										(parameters[0] + 5) * 20 + j;
								k += 3;
							}
						break;
					case PERF_LIMIT_SET_MSG_ID:
						if (parameters[0] >= num_perf_domains) {
							*status = SCMI_STATUS_NOT_FOUND;
							break;
						} else if ((parameters[2] > parameters[1]) ||
							(parameters[1] > ((parameters[0] + 1) * 10 +
							perf_levels)) || (parameters[2] <
									((parameters[0] + 1) * 10) + 1)) {
							*status = SCMI_STATUS_OUT_OF_RANGE;
							break;
						}
						*status = SCMI_STATUS_SUCCESS;
						break;
					case PERF_LIMIT_GET_MSG_ID:
						if (parameters[0] >= num_perf_domains) {
							*status = SCMI_STATUS_NOT_FOUND;
							break;
						}
						*status = SCMI_STATUS_SUCCESS;
						return_values[OFFSET_RET(
							struct arm_scmi_performance_limits_get,range_max)] =
							(parameters[0] + 1) * 10 + perf_levels;
						return_values[OFFSET_RET(
							struct arm_scmi_performance_limits_get,range_min)] =
							(parameters[0] + 1) * 10 + 1;
						break;
					case PERF_LVL_SET_MSG_ID:
						if (parameters[0] >= num_perf_domains) {
							*status = SCMI_STATUS_NOT_FOUND;
							break;
						} else if ((parameters[1] >
							((parameters[0] + 1) * 10 + perf_levels)) ||
							(parameters[1] < ((parameters[0] + 1) * 10) + 1)) {
							*status = SCMI_STATUS_OUT_OF_RANGE;
							break;
						}
						*status = SCMI_STATUS_SUCCESS;
						break;
					case PERF_LVL_GET_MSG_ID:
						if (parameters[0] >= num_perf_domains) {
							*status = SCMI_STATUS_NOT_FOUND;
							break;
						}
						*status = SCMI_STATUS_SUCCESS;
						return_values[OFFSET_RET(
							struct arm_scmi_performance_levels_get,
							performance_level)] = (parameters[0] + 1) * 10 +
								perf_levels;
						break;
				}
			break;
#endif
#ifdef CLOCK_PROTOCOL
				case CLK_PROTO_ID:
					switch (message_id)
					{
						case CLK_PROTO_VER_MSG_ID:
							*status = SCMI_STATUS_SUCCESS;
							*return_values_count = 1;
							return_values[0] = 0x00010000;
							break;
						case CLK_PROTO_ATTRIB_MSG_ID:
							*status = SCMI_STATUS_SUCCESS;
							*return_values_count = 1;
							return_values[OFFSET_RET(
								struct arm_scmi_clock_protocol_attributes,
								attributes)] =
									(NUMBER_CLOCKS << CLK_NUMBER_CLOCKS_LOW);
							break;
						case CLK_PROTO_MSG_ATTRIB_MSG_ID:
							*status = SCMI_STATUS_SUCCESS;
							*return_values_count = 1;
							return_values[OFFSET_RET(
								struct arm_scmi_clock_protocol_message_attributes,
								attributes)] = 0x0;
							break;
						case CLK_ATTRIB_MSG_ID:
							if (parameters[OFFSET_PARAM(struct arm_scmi_clock_attributes,
									clock_id)] >= NUMBER_CLOCKS) {
								*status = SCMI_STATUS_NOT_FOUND;
								break;
							}
							*status = SCMI_STATUS_SUCCESS;
							str = (char *)
							(&return_values[OFFSET_RET(
								struct arm_scmi_clock_attributes,
								clock_name)]);
							sprintf(str, "CLOCK_%d", parameters[0]);
							break;
						case CLK_DESC_RATE_MSG_ID:
							if (parameters[OFFSET_PARAM(struct arm_scmi_clock_describe_rates,
									clock_id)] >= NUMBER_CLOCKS) {
								*status = SCMI_STATUS_NOT_FOUND;
								break;
							}
							*status = SCMI_STATUS_SUCCESS;
							j = 100;k=0;
							return_values[OFFSET_RET(
							struct arm_scmi_clock_describe_rates,
							num_rates_flags)] = (clock_rates[parameters[0]] <<
									CLK_DESCRATE_NUM_RATES_RETURNED_LOW);
							for (i = 0; i < clock_rates[parameters[0]]; ++i) {
								return_values[OFFSET_RET(
								struct arm_scmi_clock_describe_rates,
								rates) + 2 * i] = j * (i + 1);
								return_values[OFFSET_RET(
								struct arm_scmi_clock_describe_rates,
								rates) + 2 * i + 1] = j * (i + 1) + 50;
							}
							break;
						case CLK_RATE_SET_MSG_ID:
							if (parameters[OFFSET_PARAM(
								struct arm_scmi_clock_rate_set, clock_id)] >=
									NUMBER_CLOCKS) {
								*status = SCMI_STATUS_NOT_FOUND;
								break;
							}
							*status = SCMI_STATUS_SUCCESS;
							break;
						case CLK_RATE_GET_MSG_ID:
							if (parameters[OFFSET_PARAM(
								struct arm_scmi_clock_rate_get, clock_id)] >=
									NUMBER_CLOCKS) {
								*status = SCMI_STATUS_NOT_FOUND;
								break;
							}
							*status = SCMI_STATUS_SUCCESS;
							return_values[OFFSET_RET(
								struct arm_scmi_clock_rate_get,
								rate)] = 100;
							return_values[OFFSET_RET(
								struct arm_scmi_clock_rate_get,
								rate) + 1] = 150;
							break;
						case CLK_CONFIG_SET_MSG_ID:
							if (parameters[OFFSET_PARAM(
								struct arm_scmi_clock_config_set, clock_id)] >=
									NUMBER_CLOCKS) {
								*status = SCMI_STATUS_NOT_FOUND;
								break;
							}
							*status = SCMI_STATUS_SUCCESS;
							break;
					}
				break;
#endif
#ifdef SENSOR_PROTOCOL
			case SNSR_PROTO_ID:
				switch(message_id)
				{
					case SNSR_PROTO_VER_MSG_ID:
						*status = SCMI_STATUS_SUCCESS;
						*return_values_count = 1;
						return_values[0] = 0x00010000;
						break;
					case SNSR_PROTO_ATTRIB_MSG_ID:
						*status = SCMI_STATUS_SUCCESS;
						*return_values_count = 4;
						return_values[OFFSET_RET(
						struct arm_scmi_sensor_protocol_attributes,
						attributes)] =
							(NUMBER_SENSORS << SNR_NUMBER_SENSORS_LOW) +
							(MAX_NUM_ASYNC_RATES_CHGS_SPRT <<
							SNR_MAX_NUM_PENDING_ASYNC_CMDS_SPRT_LOW) +
							(RESERVED << SNR_RESERVED_LOW);
						return_values[OFFSET_RET(
							struct arm_scmi_sensor_protocol_attributes,
							sensor_reg_address_low)] = 0;
						return_values[OFFSET_RET(
							struct arm_scmi_sensor_protocol_attributes,
							sensor_reg_address_high)] = 0;
						return_values[OFFSET_RET(
							struct arm_scmi_sensor_protocol_attributes,
							sensor_reg_len)] = 0;
						break;
					case SNSR_PROTO_MSG_ATTRIB_MSG_ID:
						*status = SCMI_STATUS_SUCCESS;
						*return_values_count = 1;
						return_values[OFFSET_RET(
							struct arm_scmi_sensor_protocol_message_attributes,
							attributes)] = 0x0;
						break;
					case SNSR_DESC_GET_MSG_ID:
						if (parameters[OFFSET_PARAM(
							struct arm_scmi_sensor_description_get, desc_index)]
							>= NUMBER_SENSORS) {
							/*invalid sensor descriptor id*/
							*status = SCMI_STATUS_NOT_FOUND;
							break;
						} else {
							/*valid sensor descriptor id*/
							return_values[OFFSET_RET(
								struct arm_scmi_sensor_description_get,
								num_sensor_flags)] =
									(0 << SNR_NUMBER_REMAIN_SNR_DESC_LOW) |
									(RESERVED << SNR_NUM_SNR_FLAG_RESV_LOW) |
									(number_sensor_descriptors <<
									SNR_NUMBER_SNR_DESC_LOW);
							offset = OFFSET_BYTES_RET(
								struct arm_scmi_sensor_description_get,
								sensor_descriptors);
							struct arm_scmi_sensor_descriptor *ptr =
								(struct arm_scmi_sensor_descriptor *)
								((uint8_t *)return_values + offset);
							for (sensor_idx = 0;
								sensor_idx < number_sensor_descriptors;
								++sensor_idx) {
								ptr->sensor_id = sensor_idx;
								ptr->sensor_attributes_low = (0 <<
									SNR_DESC_ATTRLOW_ASYNC_READ_SUPPORT) |
									(number_trip_points[sensor_idx] <<
									SNR_DESC_ATTRLOW_NUM_TRIPPOINT_SPRT_LOW);
								ptr->sensor_attributes_high =
									((sensor_idx * sensor_idx) <<
									SNR_DESC_ATTRHIGH_SNR_TYPE_LOW);
								sprintf((char *)(ptr->sensor_name), "SENSOR_%d",
									sensor_idx);
								ptr = (struct arm_scmi_sensor_descriptor *)
									((uint8_t *)ptr->sensor_name +
									sizeof(ptr->sensor_name));
							}
						}
						break;
					case SNSR_TRIP_POINT_NOTIFY_ID:
						if (parameters[OFFSET_PARAM(
							struct arm_scmi_sensor_trip_point_notify,
								sensor_id)] >= NUMBER_SENSORS) {
							/*invalid sensor id*/
							*status = SCMI_STATUS_NOT_FOUND;
							break;
						} else {
							/*valid sensor id*/
							*status = SCMI_STATUS_SUCCESS;
						}
						break;
					case SNSR_TRIP_POINT_CONFIG_ID:
						if (parameters[OFFSET_PARAM(
							struct arm_scmi_sensor_trip_point_config, sensor_id)
							] >= NUMBER_SENSORS) {
							*status = SCMI_STATUS_NOT_FOUND;
							break;
						}
						else if ((arm_scmi_get_norm_bits(parameters[OFFSET_PARAM(
								struct arm_scmi_sensor_trip_point_config,
								trip_point_ev_ctrl)],
								SNR_TRIP_POINT_ID_HIGH, SNR_TRIP_POINT_ID_LOW)) >=
								arm_scmi_get_norm_bits(mocker_sensor_desc[
								parameters[OFFSET_PARAM(
								struct arm_scmi_sensor_trip_point_config,
								sensor_id)]].sensor_attributes_low,
								SNR_DESC_ATTRLOW_NUM_TRIPPOINT_SPRT_HIGH,
								SNR_DESC_ATTRLOW_NUM_TRIPPOINT_SPRT_LOW)) {

							*status = SCMI_STATUS_NOT_FOUND;
							break;

						}
						else {
							*status = SCMI_STATUS_SUCCESS;
						}
						break;
					case SNSR_READING_GET_MSG_ID:
						if (parameters[OFFSET_PARAM(
							struct arm_scmi_sensor_trip_point_config, sensor_id)
							] >= NUMBER_SENSORS) {
							*status = SCMI_STATUS_NOT_FOUND;
							break;
						}
						else {
							*status = SCMI_STATUS_SUCCESS;
							/*Here the mocker returns a sample value calculated
							 * using its sensor id*/
							*return_values_count = 2;
							return_values[OFFSET_RET(
							struct arm_scmi_sensor_reading_get,
							sensor_value_low)] = parameters[OFFSET_PARAM(
								struct arm_scmi_sensor_reading_get, sensor_id)] * 10;
							return_values[OFFSET_RET(struct arm_scmi_sensor_reading_get,
								sensor_value_high)] = parameters[OFFSET_PARAM(
								struct arm_scmi_sensor_reading_get, sensor_id)] * 100;
						}
						break;
				}
			break;

#endif
		default:
			printf("\nProtocol: %d\n", message_id);
			assert(!"\nUnknown protocol id\n");
			break;
	}
	return NO_ERROR;
}

/*!
 * @brief Interface function that waits for
 * delayed response.
 *
 * This is the mocker platform implementation of the receive_platform_response
 * interface which will be called by the test_agent when it is expecting to
 * receive either a delayed_response or notification from the platform over the
 * P2A channel.
 *
 */
int arm_scmi_wait_for_response(uint32_t *message_header_rcv,
		int32_t *status, size_t *return_values_count, uint32_t *return_values,
		bool *message_ready, uint32_t timeout)
{
	return NO_ERROR;
}
