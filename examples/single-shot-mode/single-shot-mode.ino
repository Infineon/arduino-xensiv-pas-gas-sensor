#include <Arduino.h>
#include <xensiv_pas_gas_ino.hpp>

#define MEAS_INTERVAL_IN_SECONDS  10 /* demo-mode value; not recommended for long-term measurements */

/**
 * Create gas sensor object. Unless otherwise specified,
 * using the Wire interface
 */
GasType_t sensorType = SENSOR_R290; // Change to SENSOR_CO2 if using co2 sensor and to SENSOR_A2L for A2L sensor
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
  

  /* Initialize the sensor */
  err = gasSensor.begin();
  if(XENSIV_PAS_GAS_OK != err)
  {
    Serial.print("initialization error: ");
    Serial.println(gasSensor.getPasGasErrorStr(err));
  }

}

void loop()
{

  /* 
   * Trigger a one shot measurement
   */
  err = gasSensor.startMeasure();
  if(XENSIV_PAS_GAS_OK != err)
  {
    Serial.print("error: ");
    Serial.println(gasSensor.getPasGasErrorStr(err));
  }

  /* Wait for the value to be ready. */
  delay(MEAS_INTERVAL_IN_SECONDS*1000);

  /**
   *  getGasConcentration() is called until the value is 
   *  available.  
   *  getGasConcentration() returns 0 when no measurement 
   *  result is yet available or an error has
   *  occurred.
   */

  do
  {
    err = gasSensor.getGasConcentration(gasRawValue);
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("error: ");
      Serial.println(gasSensor.getPasGasErrorStr(err));
      break;
    }
  } while (0 == gasRawValue);

  Serial.print("GAS value : ");
  Serial.print(gasRawValue);
  Serial.print(" ");
  Serial.println(gasSensor.getGasConcentrationUnitStr());
}