#include <Arduino.h>
#include <xensiv_pas_gas_generic-ino.hpp>

/* 
 * The sensor supports 100KHz and 400KHz. 
 * You hardware setup and pull-ups value will
 * also influence the i2c operation. You can 
 * change this value to 100000 in case of 
 * communication issues.
 */
#define I2C_FREQ_HZ  400000                     
#define PERIODIC_MEAS_INTERVAL_IN_SECONDS  10 /* demo-mode value; not recommended for long-term measurements */
// #define PERIODIC_MEAS_INTERVAL_IN_SECONDS 60L /* specification value for stable operation (uncomment for long-time-measurements) */
#define PRESSURE_REFERENCE  900

GasType_t sensorType = GAS_TYPE_CO2;
XENSIV_PAS_GASIno* gassensor = nullptr;

float gasrawvalue;
Error_t err;

void setup()
{
    Serial.begin(9600);
    delay(800);
    Serial.println("serial initialized");

    /* Initialize the i2c interface used by the sensor */
    Wire.begin();
    Wire.setClock(I2C_FREQ_HZ);

    gassensor = create_sensor(sensorType, &Wire);
    
    /* Initialize the sensor */
    err = gassensor->begin();
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("initialization error: ");
      Serial.println(err);
    }

    /* We can set the reference pressure before starting
     * the measurement
     */
    err = gassensor->setPressRef(PRESSURE_REFERENCE);
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("pressure reference error: ");
      Serial.println(err);
    }

    /*
     * Configure the sensor to measureme periodically 
     * every 60 seconds
     */
    err = gassensor->startMeasure(PERIODIC_MEAS_INTERVAL_IN_SECONDS);
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("start measure error: ");
      Serial.println(err);
    }

    delay(1000);
}

void loop()
{
    /* Wait for the value to be ready. */
    delay(PERIODIC_MEAS_INTERVAL_IN_SECONDS*1000);

    err = gassensor->getGAS_conc(gasrawvalue);
    if(XENSIV_PAS_GAS_OK != err)
    {
      /* Retry in case of timing synch mismatch */
      if(XENSIV_PAS_GAS_ERR_COMM == err)
      {
        delay(600);
        err = gassensor->getGAS_conc(gasrawvalue);
        if(XENSIV_PAS_GAS_OK != err)          
        {
          Serial.print("get gas error: ");
          Serial.println(err);
        }
      }
    }

    Serial.print(" gas value : ");
    Serial.print(gasrawvalue);
    Serial.print(" " );
    Serial.println(gassensor->getGAS_UnitStr());
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