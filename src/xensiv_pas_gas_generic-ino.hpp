/** 
 * @file        xensiv_pas_gas_generic-ino.hpp
 * @brief       XENSIV™ PAS GAS selection Arduino API
 * @copyright   Copyright (c) 2025-2026 Infineon Technologies AG
 *              
 * SPDX-License-Identifier: MIT
 */

 #ifndef PAS_GAS_GENERIC_INO_HPP_
 #define PAS_GAS_GENERIC_INO_HPP_

#include "xensiv_pas_gas_co2-ino.hpp"
#include "xensiv_pas_gas_r290-ino.hpp"

/**
 * @addtogroup gasinoapi
 * @{
 */

enum GasType_t { GAS_TYPE_CO2, GAS_TYPE_R290 };

class XENSIV_PAS_GASFactory {
public:
    static XENSIV_PAS_GASIno* create_sensor(GasType_t type, TwoWire* wire, uint8_t intPin = XENSIV_PAS_GASIno::unusedPin);
    static XENSIV_PAS_GASIno* create_sensor(GasType_t type, HardwareSerial* serial, uint8_t intPin = XENSIV_PAS_GASIno::unusedPin);
};

#endif /** PAS_GAS_GENERIC_INO_HPP_ */