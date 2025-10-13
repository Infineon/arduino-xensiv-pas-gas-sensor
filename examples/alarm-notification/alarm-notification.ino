#include <Arduino.h>
#include <xensiv_pas_gas_ino.hpp>
/* 
 * The sensor supports 100KHz and 400KHz. 
 * You hardware setup and pull-ups value will
 * also influence the i2c operation. You can 
 * change this value to 100000 in case of 
 * communication issues.
 */
#define I2C_FREQ_HZ     400000  
#define PERIODIC_MEAS_INTERVAL_IN_SECONDS  10 /* demo-mode value; not recommended for long-term measurements */
// #define PERIODIC_MEAS_INTERVAL_IN_SECONDS 60L /* specification value for stable operation (uncomment for long-time-measurements) */
#define ALARM_GAS_THRESHOLD  1200  
// #define ALARM_GAS_THRESHOLD  1000  /* for R290 sensor */

uint8_t interruptPin = 9;      /* For XMC2Go. Change it for your hardware setup */

GasType_t sensorType = SENSOR_CO2;    // Change to SENSOR_R290 if using R290 sensor
XENSIV_PAS_GASIno gasSensor(sensorType, &Wire, interruptPin);

float gasRawValue;
Error_t err;

/* 
 * A simple interrupt service routine. This callback will be 
 * executed every time that the sensor alarm is triggered,
 * and set the flag to true. In the main loop we use that
 * flag to synchronize the  gas read out when a new value is
 * available.
 */
volatile bool intFlag = false;
void isr (void * )
{
  intFlag = true;
}

void setup()
{
    Serial.begin(9600);
    delay(500);
    Serial.println("serial initialized");

    /* Initialize the i2c serial interface used by the sensor */
    Wire.begin();
    Wire.setClock(I2C_FREQ_HZ);
    
    /*
    * No need to initialized the interrupt pin. This is done 
    * in the sensor begin() function 
    */
    err = gasSensor.begin();
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("initialization error: ");
      Serial.println(gasSensor.getPasGasErrorStr(err));
    }

    /*
    * Periodic measurement every 10 seconds.
    * Interrupt alarm when the  CO2 GAS value 
    * goes over 1200 ppm or the R290 GAS value goes over 1000 ie, 10 % LFL.
    * The isr function is 
    * passed enabling the sensor interrupt mode.
    */
    err = gasSensor.startMeasure(PERIODIC_MEAS_INTERVAL_IN_SECONDS, ALARM_GAS_THRESHOLD, isr);
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("start measure error: ");
      Serial.println(gasSensor.getPasGasErrorStr(err));
    }
}

void loop()
{
    /* Interrupt alarm requires a  GAS concentration above the threshold */
    Serial.println("USER ACTION REQUIRED --> increase co2 gas to 1200 PPM or r290 gas to 10 percent LFL to trigger the alarm!!");
    while(false == intFlag) { };

    /* Clear the interrupt flag */
    Serial.println("int occurred");
    intFlag = false;

    err = gasSensor.getGasConcentration(gasRawValue);
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("get gas error: ");
      Serial.println(gasSensor.getPasGasErrorStr(err));
    }

    Serial.print("GAS value : ");
    Serial.print(gasRawValue);
    Serial.print(" " );
    Serial.println(gasSensor.getGasConcentrationUnitStr());

}