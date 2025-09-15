/***********************************************************************************************//**
 * \file xensiv_pas_gas.h
 *
 * Description: This file contains the functions for interacting with the
 *              XENSIV™ PAS GAS sensor.
 *
 ***************************************************************************************************
 * \copyright
 * Copyright 2021-2022 Infineon Technologies AG
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

#ifndef XENSIV_PAS_GAS_H_
#define XENSIV_PAS_GAS_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "xensiv_pas_gas_regs.h"
#include "xensiv_pas_gas_platform.h"

/**
 * \addtogroup group_board_libs XENSIV™ PAS GAS sensor
 * \{
 * Library which provides a platform-independent driver for the XENSIV™ PAS GAS sensor.
 * It provides full access to all features of the sensor.
 *
 * The library depends on the target platform-specific implementation of the following functions:
 * - \ref xensiv_pas_gas_plat_i2c_transfer implementation must be provided when using the I2C interface.
 * - \ref xensiv_pas_gas_plat_uart_read, \ref xensiv_pas_gas_plat_uart_write implementation must be provided when using the UART interface.
 * - \ref xensiv_pas_gas_plat_delay implementation must be provided that delays the processing for a certain number of milliseconds.
 * - \ref xensiv_pas_gas_plat_htons implementation must be provided for byte reversing.
 * - \ref xensiv_pas_gas_plat_assert implementation must be provided for runtime assertion.
 *
 */

/************************************** Macros *******************************************/

/** Result code indicating a successful operation */
#define XENSIV_PAS_GAS_OK                        (0)
/** Result code indicating a communication error */
#define XENSIV_PAS_GAS_ERR_COMM                  (1)
/** Result code indicating that an unexpectedly large I2C write was requested which is not supported */
#define XENSIV_PAS_GAS_ERR_WRITE_TOO_LARGE       (2)
/** Result code indicating that the sensor is not yet ready after reset */
#define XENSIV_PAS_GAS_ERR_NOT_READY             (3)
/** Result code indicating whether a non-valid command has been received by the serial communication interface */
#define XENSIV_PAS_GAS_ICCERR                    (4)
/** Result code indicating whether a condition where VDD12V has been outside the specified valid range has been detected */
#define XENSIV_PAS_GAS_ORVS                      (5)
/** Result code indicating whether a condition where the temperature has been outside the specified valid range has been detected */
#define XENSIV_PAS_GAS_ORTMP                     (6)
/** Result code indicating that a new GAS value is not yet ready */
#define XENSIV_PAS_GAS_READ_NRDY                 (7)
/** Result code indicating that an invalid sensor interface was selected*/
#define XENSIV_PAS_GAS_INVALID_SENSOR_INTERFACE  (8)

/** Minimum allowed measurement rate */
#define XENSIV_PAS_GAS_MEAS_RATE_MIN             (5U)

/** Maximum allowed measurement rate */
#define XENSIV_PAS_GAS_MEAS_RATE_MAX             (4095U)

/** I2C address of the XENSIV™ PASGAS sensor */
#define XENSIV_PAS_GAS_I2C_ADDR                  (0x28U)

/********************************* Type definitions **************************************/

/** Enum defining the different comm. interfaces */
typedef enum
{
    XENSIV_PAS_GAS_INTERFACE_I2C = 0U,                  /**< I2C interface */
    XENSIV_PAS_GAS_INTERFACE_UART = 1U                  /**< UART interface */
} xensiv_pas_gas_interface_t;

/** Enum defining the different device commands */
typedef enum
{
    XENSIV_PAS_GAS_CMD_SOFT_RESET = 0xA3U,             /**< Soft reset the sensor */
} xensiv_pas_gas_cmd_t;

/** Enum defining the different device operating modes */
typedef enum
{
    XENSIV_PAS_GAS_OP_MODE_IDLE = 0U,                    /**< The device does not perform any GAS concentration measurement */
    XENSIV_PAS_GAS_OP_MODE_SINGLE = 1U,                  /**< The device triggers a single measurement sequence. At the end of the measurement sequence, the device automatically goes back to idle mode. */
    XENSIV_PAS_GAS_OP_MODE_CONTINUOUS = 2U               /**< The device periodically triggers a GAS concentration measurement sequence.
                                                             Once a measurement sequence is completed, the device goes back to an inactive state and wakes
                                                             up automatically for the next measurement sequence. The measurement period can be programmed from 5 seconds to 4095 seconds. */
} xensiv_pas_gas_op_mode_t;

/** Enum defining the different device baseline offset compensation (BOC) modes */
typedef enum
{
    XENSIV_PAS_GAS_BOC_CFG_DISABLE = 0U,                 /**< No offset compensation occurs */
    XENSIV_PAS_GAS_BOC_CFG_AUTOMATIC = 1U,               /**< The offset is periodically updated at each BOC computation */
    XENSIV_PAS_GAS_BOC_CFG_FORCED = 2U                   /**< Forced compensation */
} xensiv_pas_gas_boc_cfg_t;

/** Enum defining different interrupt active levels */
typedef enum
{
    XENSIV_PAS_GAS_INTERRUPT_TYPE_LOW_ACTIVE = 0U,       /**< Pin INT is configured as push-pull and is active LOW */
    XENSIV_PAS_GAS_INTERRUPT_TYPE_HIGH_ACTIVE = 1U       /**< Pin INT is configured as push-pull and is active HIGH */
} xensiv_pas_gas_interrupt_type_t;

/** Enum defining different pin interrupt functions */
typedef enum
{
    XENSIV_PAS_GAS_INTERRUPT_FUNCTION_NONE = 0U,         /**< Pin INT is inactive */
    XENSIV_PAS_GAS_INTERRUPT_FUNCTION_ALARM = 1U,        /**< Pin INT is configured as the alarm threshold violation notification pin */
    XENSIV_PAS_GAS_INTERRUPT_FUNCTION_DRDY = 2U,         /**< Pin INT is configured as the data ready notification pin */
    XENSIV_PAS_GAS_INTERRUPT_FUNCTION_BUSY = 3U,         /**< Pin INT is configured as the sensor busy notification pin */
    XENSIV_PAS_GAS_INTERRUPT_FUNCTION_EARLY = 4U         /**< Pin INT is configured as the early measurement start notification pin
                                                             @note This function is available only in continuous mode */
} xensiv_pas_gas_interrupt_function_t;

/** Enum defining whether an alarm is issued in the case of a lower or higher threshold violation */
typedef enum
{
    XENSIV_PAS_GAS_ALARM_TYPE_HIGH_TO_LOW = 0U,          /**< GAS ppm value falling below the alarm threshold */
    XENSIV_PAS_GAS_ALARM_TYPE_LOW_TO_HIGH = 1U           /**< GAS ppm value rising above the alarm threshold */
} xensiv_pas_gas_alarm_type_t;

/** Structure of the sensor's product and revision ID register (PROD_ID) */
typedef union
{
    struct
    {
        uint32_t rev : 5;                               /*!< Product and firmware revision */
        uint32_t prod : 3;                              /*!< Product type */
    } b;                                                /*!< Structure used for bit  access */
    uint8_t u;                                          /*!< Type used for byte access */
} xensiv_pas_gas_id_t;

/** Structure of the sensor's status register (SENS_STS) */
typedef union
{
    struct
    {
        uint32_t : 3;
        uint32_t iccerr : 1;                            /*!< Communication error notification bit.
                                                             Indicates whether an invalid command has been received by the serial communication interface*/
        uint32_t orvs : 1;                              /*!< Out-of-range VDD12V error bit */
        uint32_t ortmp : 1;                             /*!< Out-of-range temperature error bit */
        uint32_t : 2;                                   /*!< Reserved (usage depends on variant) */
    } b;                                                /*!< Structure used for bit  access */
    uint8_t u;                                          /*!< Type used for byte access */
} xensiv_pas_gas_status_t;

/** Structure of the sensor's measurement configuration register (MEAS_CFG) */
typedef union
{
    struct
    {
        uint32_t op_mode : 2;                           /*!< @ref xensiv_pas_gas_op_mode_t */
        uint32_t boc_cfg : 2;                           /*!< @ref xensiv_pas_gas_boc_cfg_t */
        uint32_t : 4;
    } b;                                                /*!< Structure used for bit  access */
    uint8_t u;                                          /*!< Type used for byte access */
} xensiv_pas_gas_measurement_config_t;

/** Structure of the sensor's interrupt configuration register (INT_CFG) */
typedef union
{
    struct
    {
        uint32_t alarm_typ : 1;                         /*!< @ref xensiv_pas_gas_alarm_type_t */
        uint32_t int_func : 3;                          /*!< @ref xensiv_pas_gas_interrupt_function_t */
        uint32_t int_typ : 1;                           /*!< @ref xensiv_pas_gas_interrupt_type_t */
        uint32_t : 3;
    } b;                                                /*!< Structure used for bit access */
    uint8_t u;                                          /*!< Type used for byte access */
} xensiv_pas_gas_interrupt_config_t;

/** Structure of the sensor's measurement status register (MEAS_STS) */
typedef union
{
    struct
    {
        uint32_t : 2;
        uint32_t alarm : 1;                             /*!< Set at the end of every measurement sequence if a threshold violation occurs */
        uint32_t int_sts : 1;                           /*!< Indicates whether the INT pin has been latched to active state (if alarm or data is ready) */
        uint32_t drdy : 1;                              /*!< Indicates whether new data is available */
        uint32_t : 3;
    } b;                                                /*!< Structure used for bit  access */
    uint8_t u;                                          /*!< Type used for byte access */
} xensiv_pas_gas_meas_status_t;

struct xensiv_pas_gas_s;                                   /* Forward declaration */

/* Function pointer to the platform-specific forced compensation function */
typedef int32_t (*xensiv_pas_gas_fcs_fptr_t)(const struct xensiv_pas_gas_s *dev, uint16_t gas_ref);

/* Function pointer to the platform-specific function for reading the sensor registers via I2C/UART */
typedef int32_t (*xensiv_pas_gas_read_fptr_t)(const struct xensiv_pas_gas_s *dev, uint8_t reg_addr, uint8_t *data, uint8_t len);

/* Function pointer to the platform-specific function for writing  the sensor registers via I2C/UART */
typedef int32_t (*xensiv_pas_gas_write_fptr_t)(const struct xensiv_pas_gas_s *dev, uint8_t reg_addr, const uint8_t *data, uint8_t len);

/** Structure of the XENSIV™ PAS GAS sensor device. Initialized using \ref xensiv_pas_gas_init_i2c or \ref xensiv_pas_gas_init_uart */
typedef struct xensiv_pas_gas_s
{
    uint8_t meas_rate_min;                  /*!< Minimum measurement rate in seconds */
    uint8_t fcs_meas_rate_s;                /*!< Measurement rate in seconds required for forced calibration */
    xensiv_pas_gas_fcs_fptr_t force_comp;   /*!< Pointer to the perform forced compensation function */

    void *ctx;                           /*!< Context for I2C/UART platform-specific read and write functions */
    xensiv_pas_gas_read_fptr_t read;     /*!< Pointer to the register read function which depends on the communication interface used */
    xensiv_pas_gas_write_fptr_t write;   /*!< Pointer to the register write function which depends on the communication interface used */

} xensiv_pas_gas_t;

/******************************* Function prototypes *************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the XENSIV™ PAS GAS device for the specified interface.
 * It initializes the dev structure, verifies the integrity of the communication layer of the serial communication interface, and checks whether the sensor is ready
 *
 * @param[in out] dev Pointer to a XENSIV™ PAS GAS sensor device structure allocated by user,
 * but the init function will initialize its contents
 * @param[in] itf Communication interface (I2C/UART)
 * @param[in] ctx Pointer to the platform-specific I2C communication handler
 * @return XENSIV_PAS_GAS_OK if the initialization was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_init(xensiv_pas_gas_t *dev, xensiv_pas_gas_interface_t itf, void *ctx);

/**
 * @brief Writes the given data buffer into the sensor device.
 * Writes the given data buffer to the sensor register map starting at the register address
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[in] reg_addr Start register address
 * @param[in] data Pointer to the data buffer to be written in the sensor
 * @param[in] len Number of bytes of data to be written
 * @return XENSIV_PAS_GAS_OK if writing to the register was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_set_reg(const xensiv_pas_gas_t *dev, uint8_t reg_addr, const uint8_t *data, uint8_t len);

/**
 * @brief Reads from the sensor device into the given data buffer.
 * Reads from the sensor register map sensor starting at register address into the given data buffer
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[in] reg_addr Start register address
 * @param[out] data Pointer to the data buffer to store the register values of the sensor
 * @param[in] len Number of bytes of data to be read
 * @return XENSIV_PAS_GAS_OK if reading from the register was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_get_reg(const xensiv_pas_gas_t *dev, uint8_t reg_addr, uint8_t *data, uint8_t len);

/**
 * @brief Gets the sensor device product and version ID
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[out] id Pointer to populate with the sensor device product and version ID
 * @note : Refer to the register map description of the XENSIV™ PAS GAS device for detailed information on the ID format
 * @return XENSIV_PAS_GAS_OK if reading the product id was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_get_id(const xensiv_pas_gas_t *dev, xensiv_pas_gas_id_t *id);

/**
 * @brief Gets the sensor device status
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[out] status Pointer to populate with the sensor device status
 * @return XENSIV_PAS_GAS_OK if reading the device status was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_get_status(const xensiv_pas_gas_t *dev, xensiv_pas_gas_status_t *status);

/**
 * @brief Clears the sensor device status bits
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[in] mask ORed combination of the following values:
 *                 @arg @ref XENSIV_PAS_GAS_REG_SENS_STS_ICCER_CLR_MSK Clears the ICCER status sticky bit
 *                 @arg @ref XENSIV_PAS_GAS_REG_SENS_STS_ORVS_CLR_MSK  Clears the ORVS status sticky bit
 *                 @arg @ref XENSIV_PAS_GAS_REG_SENS_STS_ORTMP_CLR_MSK Clears the ORTMP status sticky bit
 * @return XENSIV_PAS_GAS_OK if the status bits clearing was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_clear_status(const xensiv_pas_gas_t *dev, uint8_t mask);

/**
 * @brief Gets the sensor device interrupt configuration
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[out] int_config Pointer to populate with the sensor device interrupt configuration
 * @return XENSIV_PAS_GAS_OK if getting the interrupt configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_get_interrupt_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_interrupt_config_t *int_config);

/**
 * @brief Sets the sensor device interrupt configuration
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[in] int_config New sensor device interrupt configuration to apply
 * @return XENSIV_PAS_GAS_OK if setting the interrupt configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_set_interrupt_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_interrupt_config_t int_config);

/**
 * @brief Gets the sensor device measurement configuration
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[out] meas_config Pointer to populate with the sensor device measurement configuration
 * @return XENSIV_PAS_GAS_OK if getting the measurement configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_get_measurement_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_measurement_config_t *meas_config);

/**
 * @brief Sets the sensor device measurement configuration
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[in] meas_config New sensor device measurement configuration to apply
 * @return XENSIV_PAS_GAS_OK if setting the measurement configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_set_measurement_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_measurement_config_t meas_config);

/**
 * @brief Gets the current GAS ppm values from the sensor device
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[out] val Pointer to populate with the GAS ppm value
 * @return XENSIV_PAS_GAS_OK if obtaining the current GAS value successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_get_result(const xensiv_pas_gas_t *dev, uint16_t *val);

/**
 * @brief Sets the measurement rate for continuous mode
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[in] val New measurement rate to apply [5-4095s]
 * @return XENSIV_PAS_GAS_OK if setting the measurement rate was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_set_measurement_rate(const xensiv_pas_gas_t *dev, uint16_t val);

/**
 * @brief Gets the measurement status of the sensor device.
 * Used to check if a new GAS concentration value is available to be read (status.b.drdy) using \ref xensiv_pas_gas_get_result
 * Used to check if an interrupt is asserted (status.b.int_sts).
 * Used to check if an alarm is triggered (status.b.alarm)
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[out] status Pointer to populate with the sensor device measurement status
 * @return XENSIV_PAS_GAS_OK if getting the measurement was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_get_measurement_status(const xensiv_pas_gas_t *dev, xensiv_pas_gas_meas_status_t *status);

/**
 * @brief Clears the measurement status of the sensor device
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[in] mask ORed combination of the following values:
 *                 @arg @ref XENSIV_PAS_GAS_REG_MEAS_STS_INT_STS_CLR_MSK Clears the sticky bit MEAS_STS.INT_STS and forces the INT pin to inactive level
 *                 @arg @ref XENSIV_PAS_GAS_REG_MEAS_STS_ALARM_CLR_MSK   Clears the sticky bit MEAS_STS.ALARM
 * @return XENSIV_PAS_GAS_OK if clearing the measurement status selected bits was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_clear_measurement_status(const xensiv_pas_gas_t *dev, uint8_t mask);

/**
 * @brief Sets the alarm threshold
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[in] val New alarm threshold value to apply
 * @return XENSIV_PAS_GAS_OK if setting the alarm threshold was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_set_alarm_threshold(const xensiv_pas_gas_t *dev, uint16_t val);

/**
 * @brief Sets the pressure compensation value.
 * The GAS concentration value acquired by the sensor is dependent on the external atmospheric pressure. To
 * compensate for this effect, the application can provide the value of the atmospheric pressure using this
 * function. At the end of a measurement sequence, the device reads the pressure value and applies it for
 * compensation on the GAS concentration value before storing it into the result registers, and can be
 * retrieved using \ref xensiv_pas_gas_get_result
 *
 * @param[in] dev Pointer to a XENSIV™ PAS GAS sensor device
 * @param[in] val New pressure compensation value to apply
 * @return XENSIV_PAS_GAS_OK if setting the pressure reference value was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_set_pressure_compensation(const xensiv_pas_gas_t *dev, uint16_t val);

/**
 * @brief Sets the offset compensation value
 * Defines the reference value used for ABOC and force calibration
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[in] val New pressure calibration value to apply
 * @return XENSIV_PAS_GAS_OK if setting the measurement offset was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_set_offset_compensation(const xensiv_pas_gas_t *dev, uint16_t val);

/**
 * @brief Writes to the scratchpad register
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[in] val New scratchpad register value to apply
 * @return XENSIV_PAS_GAS_OK if writing to the scratch pad register was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_set_scratch_pad(const xensiv_pas_gas_t *dev, uint8_t val);

/**
 * @brief Reads from the scratchpad register
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[out] val Pointer to populate with the sensor device scratchpad register value
 * @return XENSIV_PAS_GAS_OK if reading from the scratch pad register was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_get_scratch_pad(const xensiv_pas_gas_t *dev, uint8_t *val);

/**
 * @brief Triggers a sensor device command
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[in] cmd Command to trigger
 * @return XENSIV_PAS_GAS_OK if triggering the command  was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_cmd(const xensiv_pas_gas_t *dev, xensiv_pas_gas_cmd_t cmd);

/**
 * @brief Triggers a single mode measurement
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @return XENSIV_PAS_GAS_OK if starting the measurement was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_start_single_mode(const xensiv_pas_gas_t *dev);

/**
 * @brief Starts measurements in continuous mode
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[in] val Measurement rate to apply [5-4095s]
 * @return XENSIV_PAS_GAS_OK if starting the measurements was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_start_continuous_mode(const xensiv_pas_gas_t *dev, uint16_t val);

/**
 * @brief Performs force compensation.
 * Used to calculate the offset compensation when the sensor is exposed to a GAS reference value.
 * The device is left in idle mode and the new offset compensation value is stored in non-volatile memory.
 *
 * @param[in] dev Pointer to the XENSIV™ PAS GAS sensor device
 * @param[in] gas_ref GAS reference value
 * @return XENSIV_PAS_GAS_OK if the force compensation was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pas_gas_perform_forced_compensation(const xensiv_pas_gas_t *dev, uint16_t gas_ref);

#ifdef __cplusplus
}
#endif

/** \} group_board_libs */

#endif /* XENSIV_PAS_GAS_H_ */
