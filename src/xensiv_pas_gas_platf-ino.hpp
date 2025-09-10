/** 
 * @file        xensiv_pas_gas-platf-ino.hpp
 * @brief       XENSIV™ PAS GAS Arduino Hardware Platforms
 * @copyright   Copyright (c) 2020-2021 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_PLATF_INO_HPP_
#define PAS_CO2_PLATF_INO_HPP_

#include <Arduino.h>
#include <Wire.h>

/**
 * @addtogroup co2inohw
 * @{
 */

/**
 * @brief Default platform selection by conditional compiling
 */ 

#if defined(XMC1100_XMC2GO)             /**< XMC2Go */

#define XENSIV_PAS_GAS_INO_I2C      &Wire
#define XENSIV_PAS_GAS_INO_UART     nullptr
#define XENSIV_PAS_GAS_INO_INT      9

#elif defined(XMC4700_Relax_Kit)        /**< XMC4700 relax kit */

#define XENSIV_PAS_GAS_INO_I2C      &Wire
#define XENSIV_PAS_GAS_INO_UART     &Serial1
#define XENSIV_PAS_GAS_INO_INT      2

#elif defined(XMC1100_Boot_Kit)         /**< XMC1100 boot kit */

#define XENSIV_PAS_GAS_INO_I2C      &Wire
#define XENSIV_PAS_GAS_INO_UART     nullptr
#define XENSIV_PAS_GAS_INO_INT      2

#elif defined(ARDUINO_FEATHER_ESP32)    /**< Adafruit ESP32 Feather */

#define XENSIV_PAS_GAS_INO_I2C      &Wire
#define XENSIV_PAS_GAS_INO_UART     nullptr
#define XENSIV_PAS_GAS_INO_INT      14

#else                                   /**<  Default Arduino */

#define XENSIV_PAS_GAS_INO_I2C      &Wire
#define XENSIV_PAS_GAS_INO_UART     nullptr
#define XENSIV_PAS_GAS_INO_INT      0xFF        /**< unused pin */

#endif

/** @} */

#endif /** PAS_CO2_PLATF_INO_HPP_ **/