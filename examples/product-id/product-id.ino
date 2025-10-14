#include <Arduino.h>
#include <xensiv_pas_gas_ino.hpp>

/**
 * Create  GAS object. Unless otherwise specified,
 * using the Wire interface
 */
GasType_t sensorType = SENSOR_CO2; // Change to SENSOR_R290 if using R290 sensor
XENSIV_PAS_GASIno gasSensor(sensorType, &Wire);

uint8_t prodId, revId;
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

  err = gasSensor.getProductID(prodId, revId);
  if(XENSIV_PAS_GAS_OK != err)
  {
    Serial.print("error: ");
    Serial.println(gasSensor.getPasGasErrorStr(err));
  }

  Serial.print("product id  : ");
  Serial.println(prodId);
  Serial.print("revision id : ");
  Serial.println(revId);
}

void loop()
{

}