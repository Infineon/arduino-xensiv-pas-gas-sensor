/**
 * @file        xensiv_pas_gas_a2l_ino.hpp
 * @brief       XENSIV™ PAS GAS A2L Arduino API
 * @copyright   Copyright (c) 2025-2026 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_GAS_A2L_INO_HPP_
#define PAS_GAS_A2L_INO_HPP_

#include "corelib/xensiv_pas_gas_a2l.h"
#include "xensiv_pas_gas_ino.hpp"

static const char *XENSIV_PAS_GAS_A2L_GAS_SELECTION_NAMES[] = {"R454b", "R32"};
#define XENSIV_PAS_GAS_A2L_ALARM_HYS_MAX (32767U) // Maximum allowed value for alarm hysteresis

/**
 * @addtogroup gasinoapi
 * @{
 */

class XENSIV_PAS_GASA2LIno : public XENSIV_PAS_GASIno
{
  public:
    XENSIV_PAS_GASA2LIno(TwoWire *wire = &Wire, uint8_t intPin = unusedPin)
        : XENSIV_PAS_GASIno(SENSOR_A2L, wire, intPin){};
    XENSIV_PAS_GASA2LIno(HardwareSerial *serial, uint8_t intPin = unusedPin)
        : XENSIV_PAS_GASIno(SENSOR_A2L, serial, intPin){};
    ~XENSIV_PAS_GASA2LIno();

    /******************************** A2L specific functions *****************************/
    Error_t getDeviceID(uint8_t &devID);
    Error_t getAlarmConfig(bool &activeHigh);
    Error_t getABOCCycle(uint8_t &days);
    Error_t setABOCCycle(uint8_t days);
    Error_t setABOCPrefill(uint8_t hours);
    Error_t getDenoiseConfig(uint8_t &smoothing_factor);
    Error_t setDenoiseConfig(uint8_t smoothing_factor);
    Error_t selfTestErrorClear(bool clear);
    String getSelfTestStatusString();

    Error_t getAlarmHysteresis(uint16_t &hysteresis);
    Error_t setAlarmHysteresis(uint16_t hysteresis);
    Error_t getAbsoluteHumidityRef(uint16_t &abs_humidity);
    Error_t setAbsoluteHumidityRef(uint16_t abs_humidity);
    Error_t enableHumidityCompensation(bool enable);
    Error_t isHumidityCompensationEnabled(bool &enable);
    Error_t setGasSelection(xensiv_pas_gas_a2l_gas_selection_t gas);
    Error_t getGasSelection(xensiv_pas_gas_a2l_gas_selection_t &gas);
    Error_t getAvailableGases(uint8_t &gas_avail);

    /***************************************************************************************/
};

#endif /** PAS_GAS_A2L_INO_HPP_ */