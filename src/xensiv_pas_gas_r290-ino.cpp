/** 
 * @file        xensiv_pas_gas_r290-ino.cpp
 * @brief       XENSIV™ PAS GAS R290 Arduino API
 * @copyright   Copyright (c) 2025-2026 Infineon Technologies AG
 *              
 * SPDX-License-Identifier: MIT
 */

#include "xensiv_pas_gas_r290-ino.hpp"

/**
 * @brief   Assertion of XENSIV™ PAS GAS return code
 */
#define GASINO_ASSERT_RET(x)   if( x != XENSIV_PAS_GAS_OK ) { return x; }

// /**
//  * @brief   Begins the sensor
//  * 
//  * @details Initializes the serial interface if the initialization
//  *          is delegated to the XENSIV_PAS_GASR290 class.
//  *          Sets the I2C freq or UART baudrate to the default values 
//  *          prior the serial interface initialization.
//  *          Initializes the interrupt pin if used.
//  * 
//  * @return  XENSIV™ PAS GAS error code
//  * @retval  XENSIV_PAS_GAS_OK if success 
//  * @pre     None
//  */
Error_t XENSIV_PAS_GASR290Ino::begin()
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
    ret = xensiv_pas_gas_r290_init(&dev, itf, ctx);
    if (ret != XENSIV_PAS_GAS_OK) return ret;

    if (unusedPin != intPin)
    {
        pinMode(intPin, INPUT_PULLUP);
    }

    // Set sensor to idle mode
    xensiv_pas_gas_measurement_config_t measConf;
    ret = xensiv_pas_gas_get_measurement_config(&dev, &measConf);
    if (ret != XENSIV_PAS_GAS_OK) return ret;

    measConf.b.op_mode = XENSIV_PAS_GAS_OP_MODE_IDLE;
    ret = xensiv_pas_gas_set_measurement_config(&dev, measConf);

    return ret;
}

/**
 * @brief       Gets the GAS concentration measured
 *               
 * 
 * @details     The value read is zero when no measurement is 
 *              yet available or an error has occurred.
 * 
 * @param[out]  gasrawvalue  GAS concentration read in LFL
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         startMeasure()
 */
Error_t XENSIV_PAS_GASR290Ino::getGasConecentration(float & GASRAWVALUE)
{
    int32_t ret = XENSIV_PAS_GAS_OK;  

    /* Initially set to 0.*/
    GASRAWVALUE = 0;
    int16_t raw = 0;

    /* Read the data */
    ret = xensiv_pas_gas_get_result(&dev, (uint16_t*)&raw);
    GASINO_ASSERT_RET(ret);

    // /* Convert to float */
    GASRAWVALUE = static_cast<float>(raw) / 100.0f; // R290: convert to %LFL

    /* Clear masks from status register */
    ret = xensiv_pas_gas_clear_measurement_status(&dev,(XENSIV_PAS_GAS_REG_MEAS_STS_INT_STS_CLR_MSK | XENSIV_PAS_GAS_REG_MEAS_STS_ALARM_CLR_MSK));
    GASINO_ASSERT_RET(ret);

    return ret;
}

XENSIV_PAS_GASR290Ino::~XENSIV_PAS_GASR290Ino() {}