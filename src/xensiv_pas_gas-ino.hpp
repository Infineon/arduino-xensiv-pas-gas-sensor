/** 
 * @file        xensiv_pas_gas-ino.hpp
 * @brief       XENSIV™ PAS GAS Arduino API
 * @copyright   Copyright (c) 2020-2021 Infineon Technologies AG
 *              
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_INO_HPP_
#define PAS_CO2_INO_HPP_

#include <Arduino.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include "xensiv_pas_gas_platf-ino.hpp"
#include "corelib/xensiv_pas_gas.h"
#include "corelib/xensiv_pas_gas_r290.h"
#include "corelib/xensiv_pas_gas_co2.h"

/**
 * @addtogroup gasinoapi
 * @{
 */

typedef int32_t Error_t;
typedef xensiv_pas_gas_status_t Diag_t;
typedef xensiv_pas_gas_boc_cfg_t ABOC_t;
typedef enum { sensor_co2, sensor_r290 } gastype_t;

#ifndef XENSIV_PAS_GAS_INVALID_SENSOR_TYPE
#define XENSIV_PAS_GAS_INVALID_SENSOR_TYPE   10
#endif

class XENSIV_PAS_GASIno
{
    public:

        static constexpr uint8_t       unusedPin = 0xFFU; /**< Unused pin */        

                XENSIV_PAS_GASIno (gastype_t gasType, TwoWire * wire = &Wire, uint8_t intPin = unusedPin);
                XENSIV_PAS_GASIno (gastype_t gasType, HardwareSerial * serial, uint8_t intPin = unusedPin);
                ~XENSIV_PAS_GASIno();
        Error_t begin   ();
        Error_t end             ();
        Error_t startMeasure    (int16_t  periodInSec = 0, int16_t alarmTh = 0, void (*cback) (void *) = nullptr, bool earlyNotification = false);
        Error_t stopMeasure     ();
        virtual Error_t getGasConecentration     (float & value) ; 
        virtual const char* getGasConecentrationUnitStr() const { return ""; } 
        Error_t getDiagnosis    (Diag_t & diagnosis);
        Error_t setABOC         (ABOC_t aboc, int16_t abocRef);
        Error_t setPressRef     (uint16_t pressRef);
        Error_t performForcedCompensation(uint16_t GASRef);
        Error_t reset           ();
        Error_t getProductID     (uint8_t & prodID, uint8_t & revID);
        virtual Error_t clearForcedCompensation  () { return XENSIV_PAS_GAS_OK; };
        Error_t getRegister     (uint8_t regAddr, uint8_t * data, uint8_t len);
        Error_t setRegister     (uint8_t regAddr, const uint8_t * data, uint8_t len);

    protected:

        TwoWire         * i2c;          /**< I2C interface*/
        HardwareSerial  * uart;         /**< UART interface */   
        uint8_t           intPin;       /**< Interrupt pin */

        static constexpr uint16_t baudrateBps = 9600;      /**< UART baud rate in bps */
        static constexpr uint32_t freqHz      = 100000;    /**< I2C frequency in Hz*/

        xensiv_pas_gas_t   dev;          /**< XENSIV™ PAS GAS corelib object */
        gastype_t gasType;               /**< Sensor type (CO2 or R290) */
};

/** @} */

#endif /** PAS_CO2_INO_HPP_ **/