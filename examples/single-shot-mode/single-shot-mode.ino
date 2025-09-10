#include <Arduino.h>
#include <xensiv_pas_gas-ino.hpp>
#include <xensiv_pas_gas_r290-ino.hpp>

/* 
 * The sensor supports 100KHz and 400KHz. 
 * You hardware setup and pull-ups value will
 * also influence the i2c operation. You can 
 * change this value to 100000 in case of 
 * communication issues.
 */
#define I2C_FREQ_HZ 400000  
#define MEAS_INTERVAL_IN_SECONDS  10 /* demo-mode value; not recommended for long-term measurements */
// #define MEAS_INTERVAL_IN_SECONDS 60L /* specification value for stable operation (uncomment for long-time-measurements) */

/**
 * Create gas sensor object. Unless otherwise specified,
 * using the Wire interface
 */
XENSIV_PAS_GASR290Ino gassensor;

int16_t gasrawvalue;
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
  err = gassensor.begin();
  if(XENSIV_PAS_GAS_OK != err)
  {
    Serial.print("initialization error: ");
    Serial.println(err);
  }

}

void loop()
{

  /* 
   * Trigger a one shot measurement
   */
  err = gassensor.startMeasure();
  if(XENSIV_PAS_GAS_OK != err)
  {
    Serial.print("error: ");
    Serial.println(err);
  }

  /* Wait for the value to be ready. */
  delay(MEAS_INTERVAL_IN_SECONDS*1000);

  /**
   *  getGAS_conc() is called until the value is 
   *  available.  
   *  getGAS_conc() returns 0 when no measurement 
   *  result is yet available or an error has
   *  occurred.
   */

  do
  {
    err = gassensor.getGAS_conc(gasrawvalue);
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("error: ");
      Serial.println(err);
      break;
    }
  } while (0 == gasrawvalue);

  Serial.print("GAS value : ");
  Serial.println(gasrawvalue);
}