#include <Arduino.h>
#include <xensiv_pas_gas_ino.hpp>

#define USE_UART_INTERFACE 0  /* Set to 1 to use UART interface; 0 to use I2C interface */

#define PERIODIC_MEAS_INTERVAL_IN_SECONDS  10 /* demo-mode value; not recommended for long-term measurements */

#define PRESSURE_REFERENCE  900

GasType_t sensorType = SENSOR_CO2;  // Change to SENSOR_R290 if using R290 sensor and to SENSOR_A2L for A2L sensor

#if USE_UART_INTERFACE
XENSIV_PAS_GASIno gasSensor(sensorType, &Serial1);
#else
XENSIV_PAS_GASIno gasSensor(sensorType, &Wire);
#endif

float gasRawValue;
Error_t err;

void setup()
{
    Serial.begin(9600);
    delay(800);
    Serial.println("serial initialized");

    #if USE_UART_INTERFACE
    /* Initialize the UART interface used by the sensor */
    Serial1.begin(9600);
    #else
    /* Initialize the i2c interface used by the sensor */
    Wire.begin();
    
    #endif

    /* Initialize the sensor */
    err = gasSensor.begin();
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("initialization error: ");
      Serial.println(gasSensor.getPasGasErrorStr(err));
    }

    /* We can set the reference pressure before starting
     * the measurement
     */
    err = gasSensor.setPressRef(PRESSURE_REFERENCE);
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("pressure reference error: ");
      Serial.println(gasSensor.getPasGasErrorStr(err));
    }

    /*
     * Configure the sensor to measureme periodically 
     * every 60 seconds
     */
    err = gasSensor.startMeasure(PERIODIC_MEAS_INTERVAL_IN_SECONDS);
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("start measure error: ");
      Serial.println(gasSensor.getPasGasErrorStr(err));
    }

    delay(1000);
}

void loop()
{
    /* Wait for the value to be ready. */
    delay(PERIODIC_MEAS_INTERVAL_IN_SECONDS*1000);

    err = gasSensor.getGasConcentration(gasRawValue);
    if(XENSIV_PAS_GAS_OK != err)
    {
      /* Retry in case of timing synch mismatch */
      if(XENSIV_PAS_GAS_ERR_COMM == err)
      {
        delay(600);
        err = gasSensor.getGasConcentration(gasRawValue);
        if(XENSIV_PAS_GAS_OK != err)          
        {
          Serial.print("get gas error: ");
          Serial.println(gasSensor.getPasGasErrorStr(err));
        }
      }
    }

    Serial.print(" gas value : ");
    Serial.print(gasRawValue);
    Serial.print(" " );
    Serial.println(gasSensor.getGasConcentrationUnitStr());
    /*
     * Assuming we have some mechanism to obtain a
     * pressure reference (i.e. a pressure sensor),
     * we could compensate again by setting the new reference. 
     * Here we just keep the initial value.
     */
    err = gasSensor.setPressRef(PRESSURE_REFERENCE);
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("pressure reference error: ");
      Serial.println(gasSensor.getPasGasErrorStr(err));
    }
}