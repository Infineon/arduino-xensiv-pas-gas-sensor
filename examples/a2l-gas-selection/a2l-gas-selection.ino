#include <Arduino.h>
#include <xensiv_pas_gas_a2l_ino.hpp>

#define MEAS_INTERVAL_IN_SECONDS  10

// Use the A2L sensor class
XENSIV_PAS_GASA2LIno gasSensor(&Wire);

float gasRawValue;
Error_t err;

void setup()
{
  Serial.begin(9600);
  delay(500);
  Serial.println("serial initialized");

  Wire.begin();

  err = gasSensor.begin();
  if(XENSIV_PAS_GAS_OK != err)
  {
    Serial.print("initialization error: ");
    Serial.println(gasSensor.getPasGasErrorStr(err));
    return;
  }

  // get current gas selection

  xensiv_pas_gas_a2l_gas_selection_t currentGas;
  
  err = gasSensor.getGasSelection(currentGas);
  if (XENSIV_PAS_GAS_OK == err) {
    Serial.print("Current gas selection: ");
    Serial.println(XENSIV_PAS_GAS_A2L_GAS_SELECTION_NAMES[currentGas]);
  } else {
    Serial.print("getGasSelection error: ");
    Serial.println(gasSensor.getPasGasErrorStr(err));
  }

  // Select gas type: R454b or R32
  xensiv_pas_gas_a2l_gas_selection_t gasType = XENSIV_PAS_GAS_A2L_GAS_R32; // or XENSIV_PAS_GAS_A2L_GAS_R454B

  err = gasSensor.setGasSelection(gasType);
  if (XENSIV_PAS_GAS_OK != err) {
    Serial.print("Gas selection error: ");
    Serial.println(gasSensor.getPasGasErrorStr(err));
  } else {
    Serial.print("Selected gas: ");
    Serial.println(XENSIV_PAS_GAS_A2L_GAS_SELECTION_NAMES[gasType]);
  }

}

void loop()
{
  err = gasSensor.startMeasure();
  if(XENSIV_PAS_GAS_OK != err)
  {
    Serial.print("error: ");
    Serial.println(gasSensor.getPasGasErrorStr(err));
  }

  delay(MEAS_INTERVAL_IN_SECONDS*1000);

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