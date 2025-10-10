#include <Arduino.h>
#include <xensiv_pas_gas-ino.hpp>

/* 
 * The sensor supports 100KHz and 400KHz. 
 * You hardware setup and pull-ups value will
 * also influence the i2c operation. You can 
 * change this value to 100000 in case of 
 * communication issues.
 */
#define I2C_FREQ_HZ 400000  
/**
 * Create  GAS object. Unless otherwise specified,
 * using the Wire interface
 */
gastype_t sensor_type = sensor_co2; // Change to sensor_r290 if using R290 sensor
XENSIV_PAS_GASIno gassensor(sensor_type, &Wire);

uint8_t prodId, revId;
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

  err = gassensor.getProductID(prodId, revId);
  if(XENSIV_PAS_GAS_OK != err)
  {
    Serial.print("error: ");
    Serial.println(err);
  }

  Serial.print("product id  : ");
  Serial.println(prodId);
  Serial.print("revision id : ");
  Serial.println(revId);
}

void loop()
{

}