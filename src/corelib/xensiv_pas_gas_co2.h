/***********************************************************************************************//**
 * \file xensiv_pas_gas_co2.h
 *
 * Description: This file contains the functions for interacting with the
 *              XENSIV™ PAS CO2 sensor.
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

#ifndef XENSIV_PAS_GAS_CO2_H_
#define XENSIV_PAS_GAS_CO2_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "xensiv_pas_gas.h"
#include "xensiv_pas_gas_co2_regs.h"

/**
 * \addtogroup group_board_libs XENSIV™ PAS GAS CO2 sensor
 * \{
 */

/************************************** Macros *******************************************/

/** Minimum allowed measurement rate */
#define XENSIV_PAS_GAS_CO2_MEAS_RATE_MIN            (5U)

/********************************* Type definitions **************************************/

/** Enum defining the different device commands */
typedef enum
{
    XENSIV_PAS_GAS_CO2_CMD_SOFT_RESET = 0xA3U,             /**< Soft reset the sensor */
    XENSIV_PAS_GAS_CO2_CMD_RESET_ABOC = 0xBCU,             /**< Resets the ABOC context */
    XENSIV_PAS_GAS_CO2_CMD_SAVE_FCS_CALIB_OFFSET = 0xCFU,  /**< Saves the force calibration offset into the non volatile memory */
    XENSIV_PAS_GAS_CO2_CMD_RESET_FCS = 0xFCU,              /**< Resets the forced calibration correction factor */
} xensiv_pas_gas_co2_cmd_t;

/** Enum defining the PWM mode configuration */
typedef enum
{
    XENSIV_PAS_GAS_CO2_PWM_MODE_SINGLE_PULSE = 0U,         /**< PWM single-pulse */
    XENSIV_PAS_GAS_CO2_PWM_MODE_TRAIN_PULSE = 1U           /**< PWM pulse-train mode */
} xensiv_pas_gas_co2_pwm_mode_t;

/** Structure of the sensor's status register (SENS_STS) */
typedef union
{
    xensiv_pas_gas_status_t base;                       /*<! Base status structure */
    struct
    {
        uint32_t : 3;
        uint32_t iccerr : 1;                            /*!< Communication error notification bit.
                                                             Indicates whether an invalid command has been received by the serial communication interface*/
        uint32_t orvs : 1;                              /*!< Out-of-range VDD5V error bit */
        uint32_t ortmp : 1;                             /*!< Out-of-range temperature error bit */
        uint32_t pwm_dis_st : 1;                        /*!< PWM_DIS pin status */
        uint32_t sen_rdy : 1;                           /*!< Sensor ready bit */
    } b;                                                /*!< Structure used for bit access */
    uint8_t u;                                          /*!< Type used for byte access */
} xensiv_pas_gas_co2_status_t;

/** Structure of the sensor's measurement configuration register (MEAS_CFG) */
typedef union
{
    xensiv_pas_gas_measurement_config_t base;           /*<! Base measurement configuration structure */
    struct
    {
        uint32_t op_mode : 2;                           /*!< @ref xensiv_pas_gas_op_mode_t */
        uint32_t boc_cfg : 2;                           /*!< @ref xensiv_pas_gas_boc_cfg_t */
        uint32_t pwm_mode : 1;                          /*!< @ref xensiv_pas_gas_pwm_mode_t */
        uint32_t pwm_outen : 1;                         /*!< PWM output software enable bit */
        uint32_t : 2;
    } b;                                                /*!< Structure used for bit access */
    uint8_t u;                                          /*!< Type used for byte access */
} xensiv_pas_gas_co2_measurement_config_t;

/******************************* Function prototypes *************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the XENSIV™ PAS GAS CO2 device.
 * It initializes the dev structure, verifies the integrity of the communication layer of the serial communication interface, and checks whether the sensor is ready
 *
 * @param[in out] dev Pointer to a XENSIV™ PAS GAS CO2 sensor device structure allocated by the user,
 * but the init function will initialize its contents
 * @param[in] itf Communication interface (I2C/UART)
 * @param[in] ctx Pointer to the platform-specific specific protocol communication handler
 * @return XENSIV_PAS_GAS_OK if the initialization was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_co2_init(xensiv_pas_gas_t *dev, xensiv_pas_gas_interface_t itf, void *ctx);

#ifdef __cplusplus
}
#endif

/** \} group_board_libs */

#endif /* XENSIV_PAS_GAS_CO2_H_ */
