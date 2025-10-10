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
// #include "corelib/xensiv_pas_gas_r290.h"

/**
 * @addtogroup gasinoapi
 * @{
 */

class XENSIV_PAS_GASR290Ino : public XENSIV_PAS_GASIno
{
    public:
        XENSIV_PAS_GASR290Ino (TwoWire * wire = &Wire, uint8_t intPin = unusedPin) : XENSIV_PAS_GASIno(sensor_r290, wire, intPin) {};
        XENSIV_PAS_GASR290Ino (HardwareSerial * serial, uint8_t intPin = unusedPin) : XENSIV_PAS_GASIno(sensor_r290, serial, intPin) {};
        ~XENSIV_PAS_GASR290Ino();

        Error_t getGasConcentration     (float & GASRAWVALUE) override;
        virtual const char* getGasConcentrationUnitStr() const override { return "%LFL"; } 

        /******************************** R290 specific functions *****************************/
        Error_t getDeviceID     (uint8_t & devID);
        Error_t getAlarmConfig  (bool & activeHigh);
        Error_t getABOCCycle    (uint8_t & days);
        Error_t setABOCCycle    (uint8_t days);
        Error_t setABOCPrefill  (uint8_t hours);
        Error_t getDenoiseConfig(uint8_t & smoothing_factor);
        Error_t setDenoiseConfig(uint8_t smoothing_factor);
        Error_t selfTestErrorClear(bool clear);
        String getSelfTestStatusString();
        /***************************************************************************************/
};

#endif /** PAS_GAS_R290_INO_HPP_ */