/** 
 * @file        xensiv_pas_gas_co2-ino.cpp
 * @brief       XENSIV™ PAS GAS CO2 Arduino API
 * @copyright   Copyright (c) 2025-2026 Infineon Technologies AG
 *              
 * SPDX-License-Identifier: MIT
 */

#include "xensiv_pas_gas_co2-ino.hpp"

/**
 * @brief   Assertion of XENSIV™ PAS GAS return code
 */
#define GASINO_ASSERT_RET(x)   if( x != XENSIV_PAS_GAS_OK ) { return x; }

// /**
//  * @brief   Begins the sensor
//  * 
//  * @details Initializes the serial interface if the initialization
//  *          is delegated to the XENSIV_PAS_GAS class.
//  *          Sets the I2C freq or UART baudrate to the default values 
//  *          prior the serial interface initialization.
//  *          Initializes the interrupt pin if used.
//  * 
//  * @return  XENSIV™ PAS GAS error code
//  * @retval  XENSIV_PAS_GAS_OK if success 
//  * @pre     None
//  */
Error_t XENSIV_PAS_GASCO2Ino::begin()
{
    int32_t ret = XENSIV_PAS_GAS_OK;
    xensiv_pas_gas_interface_t itf;
    void* ctx;

    if (nullptr != i2c)
    {
        #ifndef PAS_GAS_SERIAL_PAL_INIT_EXTERNAL
        i2c->begin();
        i2c->setClock(freqHz);
        #endif
        itf = XENSIV_PAS_GAS_INTERFACE_I2C;
        ctx = i2c;
    }
    else if (nullptr != uart)
    {
        #ifndef PAS_GAS_SERIAL_PAL_INIT_EXTERNAL
        uart->begin(baudrateBps);
        #endif
        itf = XENSIV_PAS_GAS_INTERFACE_UART;
        ctx = uart;
    }
    else
    {
        return XENSIV_PAS_GAS_INVALID_SENSOR_INTERFACE;
    }

    // R290-specific init
    ret = xensiv_pas_gas_co2_init(&dev, itf, ctx);
    if (ret != XENSIV_PAS_GAS_OK) return ret;

    if (unusedPin != intPin)
    {
        pinMode(intPin, INPUT_PULLUP);
    }

    // Set sensor to idle mode
    xensiv_pas_gas_co2_measurement_config_t measConf;
    ret = xensiv_pas_gas_get_measurement_config(&dev, &measConf.base);
    if (ret != XENSIV_PAS_GAS_OK) return ret;

    measConf.b.op_mode = XENSIV_PAS_GAS_OP_MODE_IDLE;
    ret = xensiv_pas_gas_set_measurement_config(&dev, measConf.base);

    return ret;
}

/**
 * @brief       Resets the forced calibration correction factor
 * 
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         begin()
 */
Error_t XENSIV_PAS_GASCO2Ino::clearForcedCompensation()
{
    return xensiv_pas_gas_cmd(&dev,  (xensiv_pas_gas_cmd_t)XENSIV_PAS_GAS_CO2_CMD_RESET_FCS);
}

XENSIV_PAS_GASCO2Ino::~XENSIV_PAS_GASCO2Ino() {}