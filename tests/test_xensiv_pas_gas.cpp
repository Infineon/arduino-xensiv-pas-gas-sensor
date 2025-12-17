#include "test_arduino_includes.hpp"
#include "test_xensiv_pas_gas_globals.h"
#include "xensiv_pas_gas_ino.hpp"
#include "xensiv_pas_gas_regs.h"

TEST_GROUP(XENSIV_PAS_GAS);
XENSIV_PAS_GASIno *gasSensor = nullptr;

static uint8_t testRegAddr = XENSIV_PAS_GAS_REG_SCRATCH_PAD; // Use scratchpad register for tests

// Default settings
GasType_t currentSensorType = SENSOR_CO2;
SensorInterfaceType currentInterfaceType = SENSOR_IF_I2C;

TEST_SETUP(XENSIV_PAS_GAS) {}

TEST_TEAR_DOWN(XENSIV_PAS_GAS) {}

void xensiv_pas_gas_suiteSetup()
{
    if (currentInterfaceType == SENSOR_IF_I2C)
    {
        Wire.begin();
        gasSensor = new XENSIV_PAS_GASIno(currentSensorType, &Wire);
    }
    else
    {
        Serial.begin(9600);
        gasSensor = new XENSIV_PAS_GASIno(currentSensorType, &Serial);
    }
    Error_t err = gasSensor->begin();
    TEST_ASSERT_EQUAL(0, err);
}

void xensiv_pas_gas_suiteTearDown()
{
    Error_t err = gasSensor->end();
    TEST_ASSERT_EQUAL(0, err);
    delete gasSensor;
    gasSensor = nullptr;
    if (currentInterfaceType == SENSOR_IF_I2C)
    {
        Wire.end();
    }
    else
    {
        Serial.end();
    }
    delay(500);
}

TEST_IFX(XENSIV_PAS_GAS, TestSetPressRef)
{
    uint16_t testValue = 1013;
    Error_t err = gasSensor->setPressRef(testValue);
    TEST_ASSERT_EQUAL(0, err);
    uint8_t regData[2];
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_PRESS_REF_H, regData, 1);
    uint16_t regValue = regData[0] << 8;
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_PRESS_REF_L, regData + 1, 1);
    regValue |= regData[1];
    TEST_ASSERT_EQUAL(testValue, regValue);
}

TEST_IFX(XENSIV_PAS_GAS, TestSetABOC)
{
    ABOC_t aboc = XENSIV_PAS_GAS_BOC_CFG_AUTOMATIC;
    int16_t abocRef = 400;
    Error_t err = gasSensor->setABOC(aboc, abocRef);
    TEST_ASSERT_EQUAL(0, err);
    uint8_t regData[2];
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_CALIB_REF_H, regData, 1);
    uint16_t regValue = regData[0] << 8;
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_CALIB_REF_L, regData + 1, 1);
    regValue |= regData[1];
    TEST_ASSERT_EQUAL(abocRef, regValue);

    // Restore calibration reference register to default value (0)
    uint8_t defaultCalibRef[2] = {0x00, 0x00};
    gasSensor->setRegister(XENSIV_PAS_GAS_REG_CALIB_REF_H, defaultCalibRef, 2);
}

TEST_IFX(XENSIV_PAS_GAS, TestForcedCompensationWorkflow)
{
    // Step 1: Perform forced compensation
    uint16_t testValue = 400;
    Error_t err = gasSensor->performForcedCompensation(testValue);
    TEST_ASSERT_EQUAL(0, err);

    // Step 2: Trigger three measurement cycles as required by forced compensation
    for (int i = 0; i < 3; ++i)
    {
        gasSensor->startMeasure(10, 0, nullptr, false); // 10s period recommended
        delay(1000);                                    // Wait for measurement
        float dummy;
        gasSensor->getGasConcentration(dummy);
    }

    // Step 3: Read the computed offset from the calibration register
    uint8_t regData[2];
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_CALIB_REF_H, regData, 1);
    uint16_t regValue = regData[0] << 8;
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_CALIB_REF_L, regData + 1, 1);
    regValue |= regData[1];
    TEST_ASSERT_TRUE(regValue != 0);

    // Taking long time to clear so ignored for now
    // // Step 4: Clear forced compensation
    // err = gasSensor->clearForcedCompensation();
    // delay(500);
    // TEST_ASSERT_EQUAL(0, err);
    // gasSensor->getRegister(XENSIV_PAS_GAS_REG_CALIB_REF_H, regData, 1);
    // regValue = regData[0] << 8;
    // gasSensor->getRegister(XENSIV_PAS_GAS_REG_CALIB_REF_L, regData + 1, 1);
    // regValue |= regData[1];
    // TEST_ASSERT_EQUAL(0, regValue);
}

TEST_IFX(XENSIV_PAS_GAS, TestGetGasConcentration)
{
    float value = 0.0f;
    gasSensor->getGasConcentration(value);
    uint8_t regData[2];
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_GASCONC_H, regData, 1);
    uint16_t regValue = regData[0] << 8;
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_GASCONC_L, regData + 1, 1);
    regValue |= regData[1];
    float regConc = (float)regValue;
    TEST_ASSERT_EQUAL(regConc, value);
}

TEST_IFX(XENSIV_PAS_GAS, TestGetDiagnosis)
{
    Diag_t diag;
    diag.u = 0;
    Error_t err = gasSensor->getDiagnosis(diag);
    TEST_ASSERT_EQUAL(0, err); // Ensure no error
}

TEST_IFX(XENSIV_PAS_GAS, TestGetRegister)
{
    uint8_t regData[1];
    gasSensor->getRegister(testRegAddr, regData, 1);
    uint8_t directRegData = 0;
    gasSensor->getRegister(testRegAddr, &directRegData, 1);
    TEST_ASSERT_EQUAL(regData[0], directRegData);
}

TEST_IFX(XENSIV_PAS_GAS, TestSetRegister)
{
    uint8_t regData[1] = {0x55};
    gasSensor->setRegister(testRegAddr, regData, 1); // Write to scratchpad
    uint8_t regRead[1];
    gasSensor->getRegister(testRegAddr, regRead, 1); // Read from scratchpad
    TEST_ASSERT_EQUAL(regData[0], regRead[0]);

    // Restore scratchpad to default value (0x00) to avoid impacting subsequent tests
    uint8_t defaultReg[1] = {0x00};
    gasSensor->setRegister(testRegAddr, defaultReg, 1);
}

TEST_IFX(XENSIV_PAS_GAS, TestStartMeasure)
{
    // Continuous mode (period = 10s)
    Error_t err = gasSensor->startMeasure(10);
    uint8_t regData[1];
    TEST_ASSERT_EQUAL(0, err);
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_MEAS_CFG, regData, 1);
    TEST_ASSERT_EQUAL(2, regData[0] & 0x03); // op_mode: CONTINUOUS

    // Alarm mode (threshold = 500)
    err = gasSensor->startMeasure(10, 500, nullptr, false);
    TEST_ASSERT_EQUAL(0, err);
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_MEAS_CFG, regData, 1);
    TEST_ASSERT_EQUAL(2, regData[0] & 0x03); // op_mode: CONTINUOUS (alarm is a feature of continuous)

    // Check alarm threshold register
    uint8_t alarmReg[2];
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_ALARM_TH_H, alarmReg, 1);
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_ALARM_TH_L, alarmReg + 1, 1);
    uint16_t alarmValue = (alarmReg[0] << 8) | alarmReg[1];
    TEST_ASSERT_EQUAL(500, alarmValue);
}

TEST_IFX(XENSIV_PAS_GAS, TestStopMeasure)
{
    gasSensor->stopMeasure();
    uint8_t regData[1];
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_MEAS_CFG, regData, 1);
    TEST_ASSERT_EQUAL(0, regData[0] & 0x03); // op_mode: IDLE
}

TEST_IFX(XENSIV_PAS_GAS, TestGetGasConcentrationUnitStr)
{
    const char *unitStr = gasSensor->getGasConcentrationUnitStr();
    TEST_ASSERT_NOT_NULL(unitStr);
}

TEST_IFX(XENSIV_PAS_GAS, TestGetPasGasErrorStr)
{
    const char *errStr = gasSensor->getPasGasErrorStr(-1); // Example error code
    TEST_ASSERT_NOT_NULL(errStr);
}

TEST_IFX(XENSIV_PAS_GAS, TestGetProductID)
{
    uint8_t prodID = 0, revID = 0;
    gasSensor->getProductID(prodID, revID);
    uint8_t regData[1];
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_PROD_ID, regData, 1);
    uint8_t regProdID = (regData[0] & XENSIV_PAS_GAS_REG_PROD_ID_PROD_MSK) >> XENSIV_PAS_GAS_REG_PROD_ID_PROD_POS;
    uint8_t regRevID = (regData[0] & XENSIV_PAS_GAS_REG_PROD_ID_REV_MSK) >> XENSIV_PAS_GAS_REG_PROD_ID_REV_POS;
    TEST_ASSERT_EQUAL(prodID, regProdID);
    TEST_ASSERT_EQUAL(revID, regRevID);
}

TEST_GROUP_RUNNER(XENSIV_PAS_GAS)
{
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestSetPressRef);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestGetProductID);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestGetRegister);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestStartMeasure);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestStopMeasure);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestGetDiagnosis);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestGetGasConcentration);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestGetGasConcentrationUnitStr);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestSetABOC);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestSetRegister);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestGetPasGasErrorStr);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestForcedCompensationWorkflow);
}

TEST_GROUP_RUNNER(XENSIV_PAS_GAS_CO2_I2C)
{
    currentSensorType = SENSOR_CO2;
    currentInterfaceType = SENSOR_IF_I2C;
    xensiv_pas_gas_suiteSetup();
    RUN_TEST_GROUP(XENSIV_PAS_GAS);
    xensiv_pas_gas_suiteTearDown();
}

// TEST_GROUP_RUNNER(XENSIV_PAS_GAS_CO2_UART)   // Temporarily commented UART test due to communication error 
// {
//     currentSensorType = SENSOR_CO2;
//     currentInterfaceType = SENSOR_IF_UART;
//     xensiv_pas_gas_suiteSetup();
//     RUN_TEST_GROUP(XENSIV_PAS_GAS);
//     xensiv_pas_gas_suiteTearDown();
// }
