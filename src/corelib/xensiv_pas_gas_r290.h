/***********************************************************************************************//**
 * \file xensiv_pas_gas_r290.h
 *
 * Description: This file contains the functions for interacting with the
 *              XENSIV™ PAS R290 sensor.
 *
 ***************************************************************************************************
 * \copyright
 * Copyright 2025-2026 Infineon Technologies AG
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **************************************************************************************************/

#ifndef XENSIV_PAS_GAS_R290_H_
#define XENSIV_PAS_GAS_R290_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "xensiv_pas_gas.h"

/************************************** Macros *******************************************/
#define XENSIV_PAS_GAS_R290_MEAS_RATE_MIN            (3U)

/********************************* Type definitions **************************************/

/** Enum defining the different device commands */
typedef enum
{
    XENSIV_PAS_GAS_R290_CMD_SOFT_RESET = 0xA3U,         /**< Soft reset the sensor */
} xensiv_pas_gas_r290_cmd_t;

/** Structure of the R290 sensor's status register (SENS_STS) */
typedef union
{
    xensiv_pas_gas_status_t base;                       /*<! Base status structure */
    struct
    {
        uint32_t : 3;
        uint32_t iccerr : 1;                            /*!< Communication error notification bit (R290). */
        uint32_t orvs : 1;                              /*!< Out-of-range VDD12V error bit (R290). */
        uint32_t ortmp : 1;                             /*!< Out-of-range temperature error bit (R290). */
        uint32_t sen_rdy : 1;                           /*!< Sensor ready bit (R290). */
        uint32_t : 1;
    } b;                                                /*!< Structure used for bit  access */
    uint8_t u;                                          /*!< Type used for byte access */
} xensiv_pas_gas_r290_status_t;

/** Structure of the R290 sensor's alarm configuration (ALARM_CFG) */
typedef union
{
    struct
    {
        uint32_t alarm_poll : 2;   /*!< Pin ALARM electrical configuration (bits 0-1) */
        uint32_t : 6;    /*!< Reserved bits (bits 2-7, always 0) */
    } b;
    uint8_t u;                    /*!< Type used for byte access */
} xensiv_pas_gas_r290_alarm_config_t;

/** Structure of the R290 sensor's aboc cycle configuration ( ABOC_CYCLE) */
typedef union {
    struct
    {
        uint32_t aboc_cycle : 7; /*!< ABOC cycle in days (bits 0 - 6) */
        uint32_t : 1;           /*!< Reserved bit (bit 7, always 0) */
    } b;
    uint8_t u;                    /*!< Type used for byte access */
} xensiv_pas_gas_r290_aboc_cycle_config_t;

/** structure of the R290 sensor's denoise configuration (DENOISE_CFG) */
typedef union
{
    struct
    {
        uint32_t smoothing_factor : 7;  /*!< Smoothing factor (bits 0 - 6) */
        uint32_t : 1;                   /*!< Reserved bit (bit 7, always 0) */
    } b;
    uint8_t u; /*!< Type used for byte access */
} xensiv_pas_gas_r290_denoise_config_t;

/** Structure of the R290 sensor's self test register (SELF_TEST) */
typedef union
{
    struct
    {
        uint32_t voltage_err       : 1;  /*!< VOLTAGE_ERR: Voltage error */
        uint32_t temp_err          : 1;  /*!< TEMP_ERR: Temperature error */
        uint32_t simic_err         : 1;  /*!< SIMIC_ERR: Simic error */
        uint32_t emitter_err       : 1;  /*!< EMITTER_ERR: Emitter error */
        uint32_t aboc_drift_err    : 1;  /*!< ABOC_DRIFT_ERR: ABOC drift error */
        uint32_t lifetime_err      : 1;  /*!< LIFETIME_ERR: Lifetime error */
        uint32_t : 1;
        uint32_t replace_s_en      : 1;  /*!< REPLACE_S_EN: Replace sensor enable */
    } b;                                /*!< Structure used for bit access */
    uint8_t u;                        /*!< Type used for byte access */
} xensiv_pas_gas_r290_self_test_t;

/** Structure of the R290 sensor's self test clear register (SELF_TEST_CLR) */
typedef union {
    struct {
        uint8_t voltage_err_clr : 1;    /*!< Bit 0: Out of range VDD5V error clear */
        uint8_t temp_err_clr    : 1;    /*!< Bit 1: Out of range temperature error clear */
        uint8_t simic_err_clr  : 1;     /*!< Bit 2: Simic error clear */
        uint8_t emitter_err_clr : 1;    /*!< Bit 3: Emitter error clear */
        uint8_t aboc_drift_err_clr : 1; /*!< Bit 4: ABOC drift error clear */
        uint8_t : 3;                    /*!< Bits 5-7: Reserved, must be written as 0 */
    } b;
    uint8_t u;                       /*!< Type used for byte access */
} xensiv_pas_gas_r290_self_test_clr_t;


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the XENSIV™ PAS GAS R290 device.
 * It initializes the dev structure, verifies the integrity of the communication layer of the serial communication interface, and checks whether the sensor is ready
 *
 * @param[in out] dev Pointer to a XENSIV™ PAS GAS R290 sensor device structure allocated by the user,
 * but the init function will initialize its contents
 * @param[in] itf Communication interface (I2C/UART)
 * @param[in] ctx Pointer to the platform-specific specific protocol communication handler
 * @return XENSIV_PAS_GAS_OK if the initialization was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_r290_init(xensiv_pas_gas_t *dev, xensiv_pas_gas_interface_t itf, void *ctx);

/**
 * @brief Reads the device ID of the XENSIV™ PAS GAS R290 sensor.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS R290 sensor device structure
 * @param[out] dev_id Pointer to a buffer to store the device ID
 * @return XENSIV_PAS_GAS_OK if the read was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_r290_get_device_id(const xensiv_pas_gas_t *dev, void *dev_id);

/**
 * @brief Configures the ABOC prefill value.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS R290 sensor device structure
 * @param[in] prefill The number of hours to prefill the flash table from 0 to the maximum hours value.The maximum hours value is the ABOC cycle minus one hour.
  * For example, if the ABOC cycle is set to 10 hours, the maximum prefill value is 9 hours.
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_r290_aboc_prefill(const xensiv_pas_gas_t *dev, uint8_t prefill);

/**
 * @brief Configures the alarm configuration.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS R290 sensor device structure
 * @param[in] alarm_config New alarm configuration to apply
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_r290_set_alarm_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_r290_alarm_config_t alarm_config);

/**
 * @brief Reads the alarm configuration.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS R290 sensor device structure
 * @param[out] alarm_config Pointer to a structure to store the alarm configuration
 * @return XENSIV_PAS_GAS_OK if the read was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_r290_get_alarm_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_r290_alarm_config_t *alarm_config);

/**
 * @brief Configures the ABOC cycle.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS R290 sensor device structure
 * @param[in] aboc_cycle New ABOC cycle configuration
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_r290_set_aboc_cycle(const xensiv_pas_gas_t *dev, xensiv_pas_gas_r290_aboc_cycle_config_t aboc_cycle);

/**
 * @brief Reads the ABOC cycle configuration.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS R290 sensor device structure
 * @param[out] aboc_cycle Pointer to a structure to store the ABOC cycle configuration
 * @return XENSIV_PAS_GAS_OK if the read was successful; an error indicating what went wrong otherwise
 */

int32_t xensiv_pas_gas_r290_get_aboc_cycle(const xensiv_pas_gas_t *dev, xensiv_pas_gas_r290_aboc_cycle_config_t *aboc_cycle);

/**
 * @brief Configures the denoising of the XENSIV™ PAS GAS R290 sensor.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS R290 sensor device structure
 * @param[in] denoise_config New denoising configuration
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_r290_set_denoise_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_r290_denoise_config_t denoise_config);

/**
 * @brief Gets the denoising smoothing factor of XENSIV™ PAS GAS R290 sensor.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS R290 sensor device structure
 * @param[in] denoise_config Pointer to a structure containing the denoising configuration
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_r290_get_denoise_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_r290_denoise_config_t *denoise_config);

/**
 * @brief Configures the self test
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS R290 sensor device structure
 * @param[out] self_test Pointer to a structure to store the self test results
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_r290_get_self_test(const xensiv_pas_gas_t *dev, xensiv_pas_gas_r290_self_test_t *self_test);

/**
 * @brief Clears the self test results.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS R290 sensor device structure
 * @param[in] self_test_clr Pointer to a structure containing the self test clear configuration
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_r290_clr_self_test(const xensiv_pas_gas_t *dev, xensiv_pas_gas_r290_self_test_clr_t self_test_clr);


#ifdef __cplusplus
}
#endif

#endif /* XENSIV_PAS_GAS_R290_H_ */
