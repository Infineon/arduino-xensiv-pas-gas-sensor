/** 
 * @file        xensiv_pas_gas_co2-ino.hpp
 * @brief       XENSIV™ PAS GAS CO2 Arduino API
 * @copyright   Copyright (c) 2025-2026 Infineon Technologies AG
 *              
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_GAS_CO2_INO_HPP_
#define PAS_GAS_CO2_INO_HPP_

#include "xensiv_pas_gas-ino.hpp"
#include "corelib/xensiv_pas_gas_co2.h"

/**
 * @addtogroup gasinoapi
 * @{
 */

class XENSIV_PAS_GASCO2Ino : public XENSIV_PAS_GASIno
{
    public:
        XENSIV_PAS_GASCO2Ino(TwoWire * wire = &Wire, uint8_t intPin = unusedPin): XENSIV_PAS_GASIno(wire, intPin) {};
        XENSIV_PAS_GASCO2Ino(HardwareSerial * serial, uint8_t intPin = unusedPin): XENSIV_PAS_GASIno(serial, intPin) {};

        ~XENSIV_PAS_GASCO2Ino();

        Error_t begin           () override;
        Error_t clearForcedCompensation  ();
        virtual const char* getGAS_UnitStr() const override { return "ppm"; } 
};

#endif /** PAS_GAS_CO2_INO_HPP_ */