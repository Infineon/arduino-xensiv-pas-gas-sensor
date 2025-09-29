/** 
 * @file        xensiv_pas_gas_r290-ino.hpp
 * @brief       XENSIV™ PAS GAS R290 Arduino API
 * @copyright   Copyright (c) 2025-2026 Infineon Technologies AG
 *              
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_GAS_R290_INO_HPP_
#define PAS_GAS_R290_INO_HPP_

#include "xensiv_pas_gas-ino.hpp"
#include "corelib/xensiv_pas_gas_r290.h"

/**
 * @addtogroup gasinoapi
 * @{
 */

class XENSIV_PAS_GASR290Ino : public XENSIV_PAS_GASIno
{
    public:
        XENSIV_PAS_GASR290Ino (TwoWire * wire = &Wire, uint8_t intPin = unusedPin) : XENSIV_PAS_GASIno(wire, intPin) {};
        XENSIV_PAS_GASR290Ino (HardwareSerial * serial, uint8_t intPin = unusedPin) : XENSIV_PAS_GASIno(serial, intPin) {};
        ~XENSIV_PAS_GASR290Ino();

        Error_t begin           () override;
        Error_t getGasConecentration     (float & GASRAWVALUE) override;
        virtual const char* getGasConecentrationUnitStr() const override { return "%LFL"; } // R290 specific
};

#endif /** PAS_GAS_R290_INO_HPP_ */