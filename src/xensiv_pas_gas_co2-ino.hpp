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
#include "xensiv_pas_gas_co2.h"

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
        // Error_t startMeasure    (int16_t  periodInSec = 0, int16_t alarmTh = 0, void (*cback) (void *) = nullptr, bool earlyNotification = false) override;
        // Error_t stopMeasure     () override;
        // Error_t setABOC         (ABOC_t aboc, int16_t abocRef) override;
};

#endif /** PAS_GAS_CO2_INO_HPP_ */