#include <Arduino.h>
#include <xensiv_pas_gas-ino.hpp>
#include <xensiv_pas_gas_co2-ino.hpp>  // Change to xensiv_pas_gas_r290-ino.hpp if using R290 sensor

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
#define ALARM_PPM_THRESHOLD  1200

uint8_t interrupt_pin = 9;      /* For XMC2Go. Change it for your hardware setup */

/*
 * The constructor takes the Wire instance as i2c interface,
 * and the controller interrupt pin
 */
XENSIV_PAS_GASCO2Ino gassensor(&Wire, interrupt_pin);

int16_t gasrawvalue;
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
    err = gassensor.begin(); // Change to XENSIV_PAS_GAS_CO2 if using CO2 sensor
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("initialization error: ");
      Serial.println(err);
    }

    /*
    * Periodic measurement every 10 seconds.
    * Interrupt alarm when the  GAS value 
    * goes over 1200 ppm. The isr function is 
    * passed enabling the sensor interrupt mode.
    */
    err = gassensor.startMeasure(PERIODIC_MEAS_INTERVAL_IN_SECONDS, ALARM_PPM_THRESHOLD, isr);
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("start measure error: ");
      Serial.println(err);
    }
}

void loop()
{
    /* Interrupt alarm requires a  GAS concentration above the threshold */
    Serial.println("USER ACTION REQUIRED --> increase  gas to 1200 PPM to trigger the alarm!!");
    while(false == intFlag) { };

    /* Clear the interrupt flag */
    Serial.println("int occurred");
    intFlag = false;

    err = gassensor.getGAS_conc(gasrawvalue);
    if(XENSIV_PAS_GAS_OK != err)
    {
      Serial.print("get gas error: ");
      Serial.println(err);
    }

    Serial.print("GAS value : ");
    Serial.println(gasrawvalue);
}