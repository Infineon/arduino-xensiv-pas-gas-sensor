/**
 * @file        xensiv_pas_gas_generic-ino.cpp
 * @brief       XENSIV™ PAS GAS selection Arduino API
 * @copyright   Copyright (c) 2025-2026 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#include "xensiv_pas_gas_generic-ino.hpp"

XENSIV_PAS_GASIno* XENSIV_PAS_GASFactory::create_sensor(GasType_t type, TwoWire* wire, uint8_t intPin) {
        if (type == GAS_TYPE_CO2) {
            return new XENSIV_PAS_GASCO2Ino(wire, intPin);
        } else if (type == GAS_TYPE_R290) {
            return new XENSIV_PAS_GASR290Ino(wire, intPin);
        }
        return nullptr;
    }

XENSIV_PAS_GASIno* XENSIV_PAS_GASFactory::create_sensor(GasType_t type, HardwareSerial* serial, uint8_t intPin) {
        if (type == GAS_TYPE_CO2) {
            return new XENSIV_PAS_GASCO2Ino(serial, intPin);
        } else if (type == GAS_TYPE_R290) {
            return new XENSIV_PAS_GASR290Ino(serial, intPin);
        }
        return nullptr;
    }
