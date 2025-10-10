/***********************************************************************************************//**
 * \file xensiv_pas_gas_a2l.h
 *
 * Description: This file contains the functions for interacting with the
 *              XENSIV™ PAS A2L gas sensor.
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

#ifndef XENSIV_PAS_GAS_A2L_H_
#define XENSIV_PAS_GAS_A2L_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "xensiv_pas_gas.h"

/************************************** Macros *******************************************/
#define XENSIV_PAS_GAS_A2L_MEAS_RATE_MIN            (3U)
#define XENSIV_PAS_GAS_A2L_FCS_MEAS_RATE_S          (3U)


/********************************* Type definitions **************************************/

/** Enum defining the different device commands */
typedef enum
{
    XENSIV_PAS_GAS_A2L_CMD_SOFT_RESET = 0xA3U,             /**< Soft reset the sensor */
    XENSIV_PAS_GAS_A2L_CMD_RESET_ABOC = 0xBCU,             /**< Resets the ABOC context */
    XENSIV_PAS_GAS_A2L_CMD_SAVE_FCS_CALIB_OFFSET = 0xCFU,  /**< Saves the force calibration offset into the non volatile memory */
    XENSIV_PAS_GAS_A2L_CMD_RESET_FCS = 0xFCU,              /**< Resets the forced calibration correction factor */
} xensiv_pas_gas_a2l_cmd_t;

/** Structure of the A2L sensor's status register (SENS_STS) */
typedef union
{
    xensiv_pas_gas_status_t base;                       /*<! Base status structure */
    struct
    {
        uint32_t : 3;
        uint32_t iccerr : 1;                            /*!< Communication error notification bit (A2L). */
        uint32_t orvs : 1;                              /*!< Out-of-range VDD5V error bit (A2L). */
        uint32_t ortmp : 1;                             /*!< Out-of-range temperature error bit (A2L). */
        uint32_t sen_rdy : 1;                           /*!< Sensor ready bit (A2L). */
        uint32_t : 1;
    } b;                                                /*!< Structure used for bit  access */
    uint8_t u;                                          /*!< Type used for byte access */
} xensiv_pas_gas_a2l_status_t;

/** Structure of the A2L sensor's gas configuration register */
typedef union {
    struct {
        uint8_t gas_select : 2;   /*!< Gas type selection (bits 0-1) */
        uint8_t : 2;               /*!< Reserved bits (bits 2-3, always 0) */
        uint8_t gas_avail : 4;     /*!< Gas availability (bits 4-7) */
    } b;
    uint8_t u;                       /*!< Type used for byte access */
} xensiv_pas_gas_a2l_gas_config_t;

/** Structure of the A2L sensor's alarm configuration (ALARM_CFG) */
typedef union
{
    struct
    {
        uint32_t alarm_poll : 2;   /*!< Pin ALARM electrical configuration (bits 0-1) */
        uint32_t : 6;    /*!< Reserved bits (bits 2-7, always 0) */
    } b;
    uint8_t u;                    /*!< Type used for byte access */
} xensiv_pas_gas_a2l_alarm_config_t;

/** Structure of the A2L sensor's aboc cycle configuration (ABOC_CYCLE) */
typedef union {
    struct
    {
        uint32_t aboc_cycle : 7; /*!< ABOC cycle in days (bits 0 - 6) */
        uint32_t : 1;           /*!< Reserved bit (bit 7, always 0) */
    } b;
    uint8_t u;                    /*!< Type used for byte access */
} xensiv_pas_gas_a2l_aboc_cycle_config_t;

/** Structure of the A2L sensor's denoise configuration (DENOISE_CFG) */
typedef union
{
    struct
    {
        uint32_t smoothing_factor : 7;  /*!< Smoothing factor (bits 0 - 6) */
        uint32_t : 1;                   /*!< Reserved bit (bit 7, always 0) */
    } b;
    uint8_t u; /*!< Type used for byte access */
} xensiv_pas_gas_a2l_denoise_config_t;

/** Structure of the A2L sensor's self test register (SELF_TEST) */
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
} xensiv_pas_gas_a2l_self_test_t;

/** Structure of the A2L sensor's self test clear register (SELF_TEST_CLR) */
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
} xensiv_pas_gas_a2l_self_test_clr_t;


/** Structure of the A2L sensor's humidity compensation control register */
typedef union {
    struct {
        uint8_t hc_enable : 1;    /*!< Humidity Compensation software enable bit (bit 0) */
        uint8_t hc_err_clr : 1;   /*!< Out of range humidity error clear bit (bit 1) */
        uint8_t hum_err : 1;      /*!< Out of range humidity error bit (bit 2) */
        uint8_t hum_stale : 1;    /*!< Stale Humidity bit (bit 3) */
        uint8_t hum_mis_abs : 1; /*!< Missing Absolute Humidity bit (bit 4) */
        uint8_t : 4;              /*!< Reserved bits (bits 5-7, always 0) */
    } b;
    uint8_t u;                       /*!< Type used for byte access */
} xensiv_pas_gas_a2l_humidity_control_t;


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the XENSIV™ PAS GAS A2L device.
 * It initializes the dev structure, verifies the integrity of the communication layer of the serial communication interface, and checks whether the sensor is ready
 *
 * @param[in out] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure allocated by the user,
 * but the init function will initialize its contents
 * @param[in] itf Communication interface (I2C/UART)
 * @param[in] ctx Pointer to the platform-specific specific protocol communication handler
 * @return XENSIV_PAS_GAS_OK if the initialization was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_init(xensiv_pas_gas_t *dev, xensiv_pas_gas_interface_t itf, void *ctx);

/**
 * @brief Gets the device index.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[out] dev_id_index Pointer to a variable to store the device index
 * @return XENSIV_PAS_GAS_OK if the read was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_get_dev_idx(const xensiv_pas_gas_t *dev, uint8_t *dev_id_index);

/**
 * @brief Sets the device index.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[in] dev_id_index New device index to set
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_set_dev_idx(const xensiv_pas_gas_t *dev, uint8_t dev_id_index);

/**
 * @brief Reads the device ID of the XENSIV™ PAS GAS A2L sensor.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[out] dev_id Pointer to a buffer to store the device ID
 * @return XENSIV_PAS_GAS_OK if the read was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_get_device_id(const xensiv_pas_gas_t *dev, void *dev_id);

/**
 * @brief Configures the ABOC prefill value.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[in] prefill The number of hours to prefill the flash table from 0 to the maximum hours value. The maximum hours value is the ABOC cycle minus one hour.
 * For example, if the ABOC cycle is set to 10 hours, the maximum prefill value is 9 hours.
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_aboc_prefill(const xensiv_pas_gas_t *dev, uint8_t prefill);

/**
 * @brief Configures the alarm configuration.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[in] alarm_config New alarm configuration to apply
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_set_alarm_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_alarm_config_t alarm_config);

/**
 * @brief Reads the alarm configuration.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[out] alarm_config Pointer to a structure to store the alarm configuration
 * @return XENSIV_PAS_GAS_OK if the read was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_get_alarm_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_alarm_config_t *alarm_config);

/**
 * @brief Configures the ABOC cycle.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[in] aboc_cycle New ABOC cycle configuration
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_set_aboc_cycle(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_aboc_cycle_config_t aboc_cycle);

/**
 * @brief Reads the ABOC cycle configuration.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[out] aboc_cycle Pointer to a structure to store the ABOC cycle configuration
 * @return XENSIV_PAS_GAS_OK if the read was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_get_aboc_cycle(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_aboc_cycle_config_t *aboc_cycle);

/**
 * @brief Configures the denoising of the XENSIV™ PAS GAS A2L sensor.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[in] denoise_config New denoising configuration
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_set_denoise_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_denoise_config_t denoise_config);

/**
 * @brief Gets the denoising smoothing factor of XENSIV™ PAS GAS A2L sensor.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[in] denoise_config Pointer to a structure containing the denoising configuration
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_get_denoise_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_denoise_config_t *denoise_config);

/**
 * @brief Configures the self test
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[out] self_test Pointer to a structure to store the self test results
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_get_self_test(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_self_test_t *self_test);

/**
 * @brief Clears the self test results.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[in] self_test_clr Pointer to a structure containing the self test clear configuration
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_clr_self_test(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_self_test_clr_t self_test_clr);

/**
 * @brief Configures the gas type.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[in] gas_config New gas configuration to apply
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_set_gas_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_gas_config_t gas_config);

/**
 * @brief Reads the gas type configuration.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[out] gas_config Pointer to a structure to store the gas configuration
 * @return XENSIV_PAS_GAS_OK if the read was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_get_gas_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_gas_config_t *gas_config);

/**
 * @brief Sets the alarm hysteresis.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[in] alarm_hys New alarm hysteresis value
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_set_alarm_hysteresis(const xensiv_pas_gas_t *dev, uint16_t alarm_hys);

/**
 * @brief Gets the alarm hysteresis.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[out] alarm_hys Pointer to store the alarm hysteresis value
 * @return XENSIV_PAS_GAS_OK if the read was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_get_alarm_hysteresis(const xensiv_pas_gas_t *dev, uint16_t *alarm_hys);

/**
 * @brief Sets the absolute humidity reference value for humidity compensation.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[in] abs_hum_ref New absolute humidity reference value to apply [0-500 g/m³]
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_set_absolute_humidity_ref(const xensiv_pas_gas_t *dev, uint16_t abs_hum_ref);

/**
 * @brief Gets the absolute humidity reference value for humidity compensation.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[out] abs_hum_ref Pointer to a variable to store the absolute humidity reference value [0-500 g/m³]
 * @return XENSIV_PAS_GAS_OK if the read was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_get_absolute_humidity_ref(const xensiv_pas_gas_t *dev, uint16_t *abs_hum_ref);

/**
 * @brief Configures the humidity compensation control.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[in] hc_control New humidity compensation control configuration to apply
 * @return XENSIV_PAS_GAS_OK if the configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_set_humidity_control(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_humidity_control_t hc_control);

/**
 * @brief Reads the humidity compensation control configuration.
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS A2L sensor device structure
 * @param[out] hc_control Pointer to a structure to store the humidity compensation control configuration
 * @return XENSIV_PAS_GAS_OK if the read was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_a2l_get_humidity_control(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_humidity_control_t *hc_control);

#ifdef __cplusplus
}
#endif

#endif /* XENSIV_PAS_GAS_A2L_H_ */
