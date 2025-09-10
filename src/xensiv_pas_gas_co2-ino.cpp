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


// /**
//  * @brief       Triggers the internal measuring of the sensor 
//  * 
//  * @details     The function start the measurement controlling the different
//  *              sensor modes and features depending on the configured arguments.
//  *              
//  *              Single shot
//  *              ---------------------------------------------------------------
//  *              If the function is called with no arguments, the sensor
//  *              will be triggered to perform a single shot measurement. 
//  *              The user needs to poll with getGAS_conc() until the CO2 value is 
//  *              available and has been read out from the sensor.
//  *              The CO2 concentration value read will be zero as long as 
//  *              no value is available or if any error occurred in the 
//  *              readout attempt. 
//  *              Polling example:
//  * 
//  *              @code
//  *              XENSIV_PAS_GASIno gassensor(serial_intf); 
//  *              int16_t   gasrawvalue;
//  * 
//  *              serial_intf.begin();
//  * 
//  *              gassensor.begin();              
//  * 
//  *              gassensor.startMeasure();
//  * 
//  *              do{ gassensor.getGAS_conc(gasrawvalue); } while (gasrawvalue == 0);  
//  *              @endcode
//  * 
//  *              Continuous measurement
//  *              ---------------------------------------------------------------
//  *              Continuous measurements (periodInSec) will configure the sensor
//  *              to perform a measurement every desired period. Between 5 and
//  *              4095 seconds.
//  *              Without further arguments, the user has to poll with getGAS_conc()
//  *              until the value is available. Any super loop or thread 
//  *              routine, can just consists on reading the CO2 (getGAS_conc()). 
//  *              For example, measure every 5 minutes:
//  * 
//  *              @code
//  *              XENSIV_PAS_GASIno gassensor(serial_intf);
//  *              int16_t   gasrawvalue;
//  * 
//  *              serial_intf.begin();
//  * 
//  *              gassensor.begin();  
//  * 
//  *              gassensor.startMeasure(300);
//  * 
//  *              while(1)
//  *              {
//  *                  delay(300000); // Measure will be ready every 5 min
//  * 
//  *                  do{ gassensor.getGAS_conc(gasrawvalue); } while (gasrawvalue == 0);  
//  *                  // ... do something with the co2 value ... 
//  *              }
//  *              @endcode
//  *
//  *              Syncing readouts with the hardware interrupt
//  *              ---------------------------------------------------------------
//  *              In order not to saturate the sensor with constant serial 
//  *              requests, especially in continuous mode, it is recommended 
//  *              to synch the readout with a timer. Or even better using 
//  *              the hardware GPIO hardware interrupt.
//  *              If the interrupt pin has been provided, passing
//  *              a callback function will enable the interrupt mode. The 
//  *              type of interrupt is decided depending on the value of the
//  *              rest of the arguments and operations modes.
//  *              Some example:
//  * 
//  *              @code
//  *              volatile bool intFlag = false;
//  *              void cback(void *)
//  *              {
//  *                  intFlag = true;
//  *              }
//  * 
//  *              XENSIV_PAS_GASIno gassensor(serial_intf, interrupt);
//  *              int16_t   gasrawvalue;
//  *              
//  *              serial_intf.begin();
//  *              
//  *              gassensor.begin();  
//  * 
//  *              gassensor.startMeasure(300,0,cback);
//  * 
//  *              while(1)
//  *              {
//  *                  while(!intFlag) { // block or yield() };
//  *                  gassensor.getGAS_conc(gasrawvalue);   
//  *                  // ... do something with the co2 value ... 
//  *                  intFlag = false;
//  *              }
//  *              @endcode
//  *                               
//  *              Alarm mode
//  *              ---------------------------------------------------------------
//  *              If the alarm threshold argument is non-zero, the alarm mode 
//  *              is activated, and the sensor internal flag will be enabled 
//  *              if the concentration of CO2 goes above the specified value.
//  *              This option is better combined with the interrupt mode. Thus,
//  *              if the interrupt mode is available and a callback function
//  *              is passed, the interrupt will occur only when the co2 
//  *              concentration goes above the threshold. 
//  *              This makes mostly sense for continuous measurement configuration. 
//  *              But it can be used as well for a single shot configuration
//  * 
//  *              Early notification
//  *              ---------------------------------------------------------------
//  *              The early notification mode can be used for battery power
//  *              solutions. The interrupt signal can trigger the enablement of
//  *              the 12V emitter power supply just before the measurement is
//  *              performed, and switch it off as the interrupt signal is disabled.
//  *              Therefore, the power supply 12V only needs to be on during the
//  *              CO2 sensing.
//  *                  
//  *              When this flag is set, the alarm interrupt functionality is not 
//  *              available. Both options cannot be combined.
//  *              
//  *                 
//  * @param[in]   periodInSec         Enables continuous measurement with the specified period. 
//  *                                  The default value is 0, meaning single shot operation. 
//  *                                  The valid period range goes between 5 and 4095 seconds
//  * @param[in]   alarmTh             Enables upper alarm threshold mode for the specified
//  *                                  ppm value 
//  *                                  The default value is 0, meaning no alarm mode. 
//  *                                  For any non-zero value, the sensor will internally set 
//  *                                  the alarm flag. If an interrupt callback function is 
//  *                                  provided, then the interrupt will occur only when the 
//  *                                  defined threshold has been trespassed
//  * @param[in]   cback               Pointer to the callback function to be called upon
//  *                                  interrupt
//  * @param[in]   earlyNotification   Enables early notification interrupt. Disabled (false) by default 
//  * @return      XENSIV™ PAS GAS error code
//  * @retval      XENSIV_PAS_GAS_OK if success
//  * @pre         begin()
//  */
// Error_t XENSIV_PAS_GASCO2Ino::startMeasure(int16_t periodInSec, int16_t alarmTh, void (*cback) (void *), bool earlyNotification)
// {   
//     xensiv_pas_gas_co2_measurement_config_t measConf;
//     xensiv_pas_gas_interrupt_config_t intConf; 
//     int32_t ret = XENSIV_PAS_GAS_OK;   

//     /* Get meas configuration*/
//     ret = xensiv_pas_gas_get_measurement_config(&dev, &measConf.base);
//     GASINO_ASSERT_RET(ret);

//     /**
//      * Set the device in idle mode to avoid 
//      * any conflict if stopMeasure() was not
//      * previously called.
//      */
//     measConf.b.op_mode = XENSIV_PAS_GAS_OP_MODE_IDLE;

//     ret = xensiv_pas_gas_set_measurement_config(&dev, measConf.base);
//     GASINO_ASSERT_RET(ret);

//     /* Get int configuration */
//     ret = xensiv_pas_gas_get_interrupt_config(&dev, &intConf);
//     GASINO_ASSERT_RET(ret);

//     /* Default configuration */
//     measConf.b.op_mode = XENSIV_PAS_GAS_OP_MODE_SINGLE;
//     intConf.b.int_func = XENSIV_PAS_GAS_INTERRUPT_FUNCTION_DRDY;
             

//     if( periodInSec > 0 )
//     {   
//         ret = xensiv_pas_gas_set_measurement_rate(&dev, periodInSec);
//         GASINO_ASSERT_RET(ret);

//         measConf.b.op_mode = XENSIV_PAS_GAS_OP_MODE_CONTINUOUS;
//     }

//     if( alarmTh >  0 )
//     {
//         ret = xensiv_pas_gas_set_alarm_threshold(&dev, alarmTh);
//         GASINO_ASSERT_RET(ret);

//         intConf.b.alarm_typ = XENSIV_PAS_GAS_ALARM_TYPE_LOW_TO_HIGH;
//         intConf.b.int_func  = XENSIV_PAS_GAS_INTERRUPT_FUNCTION_ALARM;
//     }
//     else
//     {
//         ret = xensiv_pas_gas_set_alarm_threshold(&dev, 0x0000);
//         GASINO_ASSERT_RET(ret);

//         intConf.b.alarm_typ = XENSIV_PAS_GAS_ALARM_TYPE_HIGH_TO_LOW;       
//     }

//     if(cback != nullptr)
//     {
//         /* Enable sensor interrupt */
//         intConf.b.int_typ = XENSIV_PAS_GAS_INTERRUPT_TYPE_HIGH_ACTIVE;
//         #if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_RENESAS) || defined(ARDUINO_ARCH_PSOC6)
//             PinStatus int_event;
//         #else
//             uint8_t int_event;
//         #endif        
//         int_event = RISING;

//         if(true == earlyNotification)
//         {
//            /* In this case it would be useful to have an interrupt
//               for both the rising and falling edge. */
//             int_event = CHANGE;
//         }

//         /* Enable mcu interrupt */
//         attachInterrupt(digitalPinToInterrupt(intPin), (void (*)())cback, int_event);
//     }
//     else
//     {
//         /* Disable sensor interrupt */
//         intConf.b.int_func = XENSIV_PAS_GAS_INTERRUPT_FUNCTION_NONE;

//         if(unusedPin != intPin)
//         {
//             /* Disable mcu interrupt */
//             detachInterrupt(digitalPinToInterrupt(intPin));
//         }
//     }

//     /* This option will disable the alarm interrupt function */ 
//     if(true == earlyNotification)
//     {
//         intConf.b.int_func = XENSIV_PAS_GAS_INTERRUPT_FUNCTION_EARLY;
//     }

//     ret = xensiv_pas_gas_set_interrupt_config(&dev, intConf);
//     GASINO_ASSERT_RET(ret);

//     ret = xensiv_pas_gas_set_measurement_config(&dev, measConf.base);

//     return ret;
// }

// /**
//  * @brief       Stops the internal measuring of the sensor 
//  * 
//  * @details     Sets operation mode to idle
//  * 
//  * @return      XENSIV™ PAS GAS error code
//  * @retval      XENSIV_PAS_GAS_OK if success
//  * @pre         begin()
//  */
// Error_t XENSIV_PAS_GASCO2Ino::stopMeasure()
// {
//     int32_t ret = XENSIV_PAS_GAS_OK;  
//     xensiv_pas_gas_co2_measurement_config_t measConf;

//     /* Get meas configuration*/
//     ret = xensiv_pas_gas_get_measurement_config(&dev, &measConf.base);
//     GASINO_ASSERT_RET(ret);

//     /* Set meas configuration to idle mode */
//     measConf.b.op_mode = XENSIV_PAS_GAS_OP_MODE_IDLE;
//     ret = xensiv_pas_gas_set_measurement_config(&dev, measConf.base);

//     return ret;
// }

// /**
//  * @brief       Configures the sensor automatic baseline compensation
//  * 
//  * @param[in]   aboc        Automatic baseline compenstation mode  
//  * @param[in]   abocRef     Automatic baseline compensation reference
//  * @return      XENSIV™ PAS GAS error code
//  * @retval      XENSIV_PAS_GAS_OK if success
//  * @pre         begin()
//  */
// Error_t XENSIV_PAS_GASCO2Ino::setABOC(ABOC_t aboc, int16_t abocRef)
// {   
//     xensiv_pas_gas_co2_measurement_config_t measConf;
//     int32_t ret = XENSIV_PAS_GAS_OK; 

//     /* Get meas configuration */
//     ret = xensiv_pas_gas_get_measurement_config(&dev, &measConf.base);
//     GASINO_ASSERT_RET(ret);

//     /* Set compensation offset */
//     ret = xensiv_pas_gas_set_offset_compensation(&dev, (uint16_t) abocRef);
//     GASINO_ASSERT_RET(ret);

//     /* Set meas configuration with ABOC */
//     measConf.b.boc_cfg = aboc;
//     ret = xensiv_pas_gas_set_measurement_config(&dev, measConf.base);

//     return ret;
// }

XENSIV_PAS_GASCO2Ino::~XENSIV_PAS_GASCO2Ino() {}