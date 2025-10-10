/** 
 * @file        xensiv_pas_gas_a2l_ino.hpp
 * @brief       XENSIV™ PAS GAS A2L Arduino API
 * @copyright   Copyright (c) 2025-2026 Infineon Technologies AG
 *              
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_GAS_A2L_INO_HPP_
#define PAS_GAS_A2L_INO_HPP_

#include "xensiv_pas_gas-ino.hpp"
#include "corelib/xensiv_pas_gas_a2l.h"

/**
 * @addtogroup gasinoapi
 * @{
 */

class XENSIV_PAS_GASA2LIno : public XENSIV_PAS_GASIno
{
    public:
        XENSIV_PAS_GASA2LIno (TwoWire * wire = &Wire, uint8_t intPin = unusedPin) : XENSIV_PAS_GASIno(sensor_a2l, wire, intPin) {};
        XENSIV_PAS_GASA2LIno (HardwareSerial * serial, uint8_t intPin = unusedPin) : XENSIV_PAS_GASIno(sensor_a2l, serial, intPin) {};
        ~XENSIV_PAS_GASA2LIno();

        /******************************** A2L specific functions *****************************/
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