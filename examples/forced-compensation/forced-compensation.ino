#include <Arduino.h>
#include <xensiv_pas_gas_ino.hpp>

/* 
 * The sensor supports 100KHz and 400KHz. 
 * You hardware setup and pull-ups value will
 * also influence the i2c operation. You can 
 * change this value to 100000 in case of 
 * communication issues.
 */
#define I2C_FREQ_HZ 400000
#define PERIODIC_MEAS_INTERVAL_IN_SECONDS  10 /* demo-mode value; not recommended for long-term measurements */
// #define PERIODIC_MEAS_INTERVAL_IN_SECONDS 60L /* specification value for stable operation (uncomment for long-time-measurements) */
#define FORCED_COMPENSATION_GAS_REFERENCE    400
#define PRESSURE_REFERENCE  900

GasType_t sensorType = SENSOR_CO2; // Change to SENSOR_R290 if using R290 sensor
XENSIV_PAS_GASIno gasSensor(sensorType, &Wire);

float gasRawValue;
Error_t err;

void setup()
{
    Serial.begin(9600);
    delay(500);
    Serial.println("serial initialized");

    /* Initialize the i2c interface used by the sensor */
    Wire.begin();
    Wire.setClock(I2C_FREQ_HZ);

    /* Initialize the sensor */
    err = gasSensor.begin();
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("initialization error: ");
      Serial.println(gasSensor.getPasGasErrorStr(err));
    }

      err = gasSensor.clearForcedCompensation();
        if (XENSIV_PAS_GAS_OK != err) {
                Serial.print("clear forced compensation error: ");
                Serial.println(gasSensor.getPasGasErrorStr(err));
            }

    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("clear forced compensation error: ");
      Serial.println(gasSensor.getPasGasErrorStr(err));
    }

  err = gasSensor.setPressRef(PRESSURE_REFERENCE);
  if (XENSIV_PAS_GAS_OK != err)
  {
    Serial.print("pressure reference error: ");
    Serial.println(gasSensor.getPasGasErrorStr(err));
  }

    Serial.print("forced compensation started with ");
    Serial.print(FORCED_COMPENSATION_GAS_REFERENCE  );
    Serial.println(" gas concentration baseline reference ...");

    err = gasSensor.performForcedCompensation(FORCED_COMPENSATION_GAS_REFERENCE);
    if (XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("forced compensation error: ");
      Serial.println(gasSensor.getPasGasErrorStr(err));
    }

    Serial.println("forced compensation finished");

    /*
      * Configure the sensor to measureme periodically 
      * every 10 seconds
      */
    err = gasSensor.startMeasure(PERIODIC_MEAS_INTERVAL_IN_SECONDS);
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("start measure error: ");
      Serial.println(gasSensor.getPasGasErrorStr(err));
    }
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
          Serial.print("get  gas error: ");
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