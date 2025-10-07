#include <Arduino.h>
#include <xensiv_pas_gas_generic-ino.hpp>

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

#define sensor_type GAS_TYPE_CO2
XENSIV_PAS_GASIno* gassensor = XENSIV_PAS_GASFactory::create_sensor(sensor_type, &Wire, 9); // Use your actual intPin value

float gasrawvalue;
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
    err = gassensor->begin();
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("initialization error: ");
      Serial.println(err);
    }

    // Only call clearForcedCompensation for CO2 sensor
    if (sensor_type == GAS_TYPE_CO2) {
        auto* co2sensor = static_cast<XENSIV_PAS_GASCO2Ino*>(gassensor);
        if (co2sensor) {
            err = co2sensor->clearForcedCompensation();
            if (XENSIV_PAS_GAS_OK != err) {
                Serial.print("clear forced compensation error: ");
                Serial.println(err);
            }
        }
    }

    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("clear forced compensation error: ");
      Serial.println(err);
    }

  err = gassensor->setPressRef(PRESSURE_REFERENCE);
  if (XENSIV_PAS_GAS_OK != err)
  {
    Serial.print("pressure reference error: ");
    Serial.println(err);
  }

    Serial.print("forced compensation started with ");
    Serial.print(FORCED_COMPENSATION_GAS_REFERENCE  );
    Serial.println(" gas concentration baseline reference ...");

    err = gassensor->performForcedCompensation(FORCED_COMPENSATION_GAS_REFERENCE);
    if (XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("forced compensation error: ");
      Serial.println(err);
    }

    Serial.println("forced compensation finished");

    /*
      * Configure the sensor to measureme periodically 
      * every 10 seconds
      */
    err = gassensor->startMeasure(PERIODIC_MEAS_INTERVAL_IN_SECONDS);
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("start measure error: ");
      Serial.println(err);
    }
}

void loop()
{
    /* Wait for the value to be ready. */
    delay(PERIODIC_MEAS_INTERVAL_IN_SECONDS*1000);

    err = gassensor->getGasConcentration(gasrawvalue);
    if(XENSIV_PAS_GAS_OK != err)
    {
      /* Retry in case of timing synch mismatch */
      if(XENSIV_PAS_GAS_ERR_COMM == err)
      {
        delay(600);
        err = gassensor->getGasConcentration(gasrawvalue);
        if(XENSIV_PAS_GAS_OK != err)          
        {
          Serial.print("get  gas error: ");
          Serial.println(err);
        }
      }
    }

    Serial.print(" gas value : ");
    Serial.print(gasrawvalue);
    Serial.print(" " );
    Serial.println(gassensor->getGasConcentrationUnitStr());

    /*
     * Assuming we have some mechanism to obtain a
     * pressure reference (i.e. a pressure sensor),
     * we could compensate again by setting the new reference. 
     * Here we just keep the initial value.
     */
    err = gassensor->setPressRef(PRESSURE_REFERENCE);
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("pressure reference error: ");
      Serial.println(err);
    }
}