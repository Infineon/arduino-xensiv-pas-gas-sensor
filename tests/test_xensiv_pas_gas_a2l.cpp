#include "test_arduino_includes.hpp"
#include "test_xensiv_pas_gas_globals.h"
#include "xensiv_pas_gas_a2l_ino.hpp"

extern GasType_t currentSensorType;
extern SensorInterfaceType currentInterfaceType;
extern XENSIV_PAS_GASIno *gasSensor;

TEST_GROUP(XENSIV_PAS_GAS_A2L);
static XENSIV_PAS_GASA2LIno *a2lSensor = nullptr;

TEST_SETUP(XENSIV_PAS_GAS_A2L) {}

TEST_TEAR_DOWN(XENSIV_PAS_GAS_A2L) {}

void xensiv_pas_gas_a2l_suiteSetup()
{
    if (currentInterfaceType == SENSOR_IF_I2C)
    {
        Wire.begin();
        a2lSensor = new XENSIV_PAS_GASA2LIno(&Wire);
        gasSensor = a2lSensor;
    }
    else
    {
        Serial.begin(9600);
        a2lSensor = new XENSIV_PAS_GASA2LIno(&Serial);
        gasSensor = a2lSensor;
    }
    Error_t err = a2lSensor->begin();
    TEST_ASSERT_EQUAL(0, err);
}

void xensiv_pas_gas_a2l_suiteTearDown()
{
    Error_t err = a2lSensor->end();
    TEST_ASSERT_EQUAL(0, err);
    delete a2lSensor;
    a2lSensor = nullptr;
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

TEST_IFX(XENSIV_PAS_GAS_A2L, TestGetDeviceID)
{
    uint8_t devID = 0;
    Error_t err = a2lSensor->getDeviceID(devID);
    TEST_ASSERT_EQUAL(0, err);
    uint8_t regData[1];
    a2lSensor->getRegister(XENSIV_PAS_GAS_A2L_REG_DEV_ID, regData, 1);
    uint8_t regDevID = (regData[0] & XENSIV_PAS_GAS_A2L_REG_DEV_ID_VAL_MSK) >> XENSIV_PAS_GAS_A2L_REG_DEV_ID_VAL_POS;
    TEST_ASSERT_EQUAL(regDevID, devID);
}

TEST_IFX(XENSIV_PAS_GAS_A2L, TestGetAlarmConfig)
{
    bool activeHigh = false;
    Error_t err = a2lSensor->getAlarmConfig(activeHigh);
    TEST_ASSERT_EQUAL(0, err);
    uint8_t regData[1];
    a2lSensor->getRegister(XENSIV_PAS_GAS_A2L_REG_ALARM_CFG, regData, 1);
    bool regActiveHigh =
        (regData[0] & XENSIV_PAS_GAS_A2L_REG_ALARM_CFG_ALARM_POL_MSK) >> XENSIV_PAS_GAS_A2L_REG_ALARM_CFG_ALARM_POL_POS;
    TEST_ASSERT_EQUAL(regActiveHigh, activeHigh);
}

TEST_IFX(XENSIV_PAS_GAS_A2L, TestSetGetABOCCycle)
{
    uint8_t daysSet = 7;
    Error_t err = a2lSensor->setABOCCycle(daysSet);
    TEST_ASSERT_EQUAL(0, err);
    uint8_t regData[1];
    a2lSensor->getRegister(XENSIV_PAS_GAS_A2L_REG_ABOC_CYCLE, regData, 1);
    uint8_t regDays = (regData[0] & XENSIV_PAS_GAS_A2L_REG_ABOC_CYCLE_SMOOTHING_FACT_MSK) >>
                      XENSIV_PAS_GAS_A2L_REG_ABOC_CYCLE_SMOOTHING_FACT_POS;
    TEST_ASSERT_EQUAL(daysSet, regDays);
    uint8_t daysGet = 0;
    err = a2lSensor->getABOCCycle(daysGet);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(daysSet, daysGet);
}

TEST_IFX(XENSIV_PAS_GAS_A2L, TestSetABOCPrefill)
{
    uint8_t hours = 12;
    Error_t err = a2lSensor->setABOCPrefill(hours);
    TEST_ASSERT_EQUAL(0, err);
}

TEST_IFX(XENSIV_PAS_GAS_A2L, TestSetGetDenoiseConfig)
{
    uint8_t smoothingSet = 3;
    Error_t err = a2lSensor->setDenoiseConfig(smoothingSet);
    TEST_ASSERT_EQUAL(0, err);
    uint8_t regData[1];
    a2lSensor->getRegister(XENSIV_PAS_GAS_A2L_REG_DENOISE_CFG, regData, 1);
    uint8_t regSmoothing = (regData[0] & XENSIV_PAS_GAS_A2L_REG_DENOISE_CFG_SMOOTHING_FACT_MSK) >>
                           XENSIV_PAS_GAS_A2L_REG_DENOISE_CFG_SMOOTHING_FACT_POS;
    TEST_ASSERT_EQUAL(smoothingSet, regSmoothing);
    uint8_t smoothingGet = 0;
    err = a2lSensor->getDenoiseConfig(smoothingGet);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(smoothingSet, smoothingGet);
}

TEST_IFX(XENSIV_PAS_GAS_A2L, TestSelfTestErrorClear)
{
    Error_t err = a2lSensor->selfTestErrorClear(true);
    TEST_ASSERT_EQUAL(0, err);
}

TEST_IFX(XENSIV_PAS_GAS_A2L, TestGetSelfTestStatusString)
{
    uint8_t regData[1];
    a2lSensor->getRegister(XENSIV_PAS_GAS_A2L_REG_SELF_TEST, regData, 1);
    String status = a2lSensor->getSelfTestStatusString();
    TEST_ASSERT_TRUE(status.length() > 0);
}

TEST_IFX(XENSIV_PAS_GAS_A2L, TestSetGetAlarmHysteresis)
{
    uint16_t hystSet = 1234;
    Error_t err = a2lSensor->setAlarmHysteresis(hystSet);
    TEST_ASSERT_EQUAL(0, err);
    uint8_t regDataH[1], regDataL[1];
    a2lSensor->getRegister(XENSIV_PAS_GAS_A2L_REG_ALARM_HYS_H, regDataH, 1);
    a2lSensor->getRegister(XENSIV_PAS_GAS_A2L_REG_ALARM_HYS_L, regDataL, 1);
    uint16_t regHyst =
        ((regDataH[0] & XENSIV_PAS_GAS_A2L_REG_ALARM_HYS_H_MASK) >> XENSIV_PAS_GAS_A2L_REG_ALARM_HYS_H_POS) << 8 |
        (regDataL[0] & XENSIV_PAS_GAS_A2L_REG_ALARM_HYS_L_MASK) >> XENSIV_PAS_GAS_A2L_REG_ALARM_HYS_L_POS;
    TEST_ASSERT_EQUAL(hystSet, regHyst);
    uint16_t hystGet = 0;
    err = a2lSensor->getAlarmHysteresis(hystGet);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(hystSet, hystGet);
}

TEST_IFX(XENSIV_PAS_GAS_A2L, TestSetGetAbsoluteHumidityRef)
{
    uint16_t humSet = 567;
    Error_t err = a2lSensor->setAbsoluteHumidityRef(humSet);
    TEST_ASSERT_EQUAL(0, err);
    uint8_t regDataH[1], regDataL[1];
    a2lSensor->getRegister(XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_H, regDataH, 1);
    a2lSensor->getRegister(XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_L, regDataL, 1);
    uint16_t regHum =
        ((regDataH[0] & XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_H_MASK) >> XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_H_POS) << 8 |
        (regDataL[0] & XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_L_MASK) >> XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_L_POS;
    TEST_ASSERT_EQUAL(humSet, regHum);
    uint16_t humGet = 0;
    err = a2lSensor->getAbsoluteHumidityRef(humGet);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(humSet, humGet);
}

TEST_IFX(XENSIV_PAS_GAS_A2L, TestEnableIsHumidityCompensation)
{
    Error_t err = a2lSensor->enableHumidityCompensation(true);
    TEST_ASSERT_EQUAL(0, err);
    bool enabled = false;
    err = a2lSensor->isHumidityCompensationEnabled(enabled);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_TRUE(enabled);
}

TEST_IFX(XENSIV_PAS_GAS_A2L, TestSetGetGasSelection)
{
    xensiv_pas_gas_a2l_gas_selection_t gasSet = XENSIV_PAS_GAS_A2L_GAS_R32;
    Error_t err = a2lSensor->setGasSelection(gasSet);
    TEST_ASSERT_EQUAL(0, err);
    uint8_t regData[1];
    a2lSensor->getRegister(XENSIV_PAS_GAS_A2L_REG_GAS_CFG, regData, 1);
    uint8_t regGasSel =
        (regData[0] & XENSIV_PAS_GAS_A2L_REG_GAS_CFG_GAS_SEL_MASK) >> XENSIV_PAS_GAS_A2L_REG_GAS_CFG_GAS_SEL_POS;
    TEST_ASSERT_EQUAL(gasSet, regGasSel);
    xensiv_pas_gas_a2l_gas_selection_t gasGet = XENSIV_PAS_GAS_A2L_GAS_R454B;
    err = a2lSensor->getGasSelection(gasGet);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(gasSet, gasGet);
}

TEST_IFX(XENSIV_PAS_GAS_A2L, TestGetAvailableGases)
{
    uint8_t gasAvail = 0;
    Error_t err = a2lSensor->getAvailableGases(gasAvail);
    TEST_ASSERT_EQUAL(0, err);
    uint8_t regData[1];
    a2lSensor->getRegister(XENSIV_PAS_GAS_A2L_REG_GAS_CFG, regData, 1);
    uint8_t regGasAvail =
        (regData[0] & XENSIV_PAS_GAS_A2L_REG_GAS_CFG_GAS_AVAIL_MASK) >> XENSIV_PAS_GAS_A2L_REG_GAS_CFG_GAS_AVAIL_POS;
    TEST_ASSERT_EQUAL(gasAvail, regGasAvail);
}

TEST_GROUP_RUNNER(XENSIV_PAS_GAS_A2L)
{
    RUN_TEST_GROUP(XENSIV_PAS_GAS);
    RUN_TEST_CASE(XENSIV_PAS_GAS_A2L, TestGetDeviceID);
    RUN_TEST_CASE(XENSIV_PAS_GAS_A2L, TestGetAlarmConfig);
    RUN_TEST_CASE(XENSIV_PAS_GAS_A2L, TestSetGetABOCCycle);
    RUN_TEST_CASE(XENSIV_PAS_GAS_A2L, TestSetABOCPrefill);
    RUN_TEST_CASE(XENSIV_PAS_GAS_A2L, TestSetGetDenoiseConfig);
    RUN_TEST_CASE(XENSIV_PAS_GAS_A2L, TestSelfTestErrorClear);
    RUN_TEST_CASE(XENSIV_PAS_GAS_A2L, TestGetSelfTestStatusString);
    RUN_TEST_CASE(XENSIV_PAS_GAS_A2L, TestSetGetAlarmHysteresis);
    RUN_TEST_CASE(XENSIV_PAS_GAS_A2L, TestSetGetAbsoluteHumidityRef);
    RUN_TEST_CASE(XENSIV_PAS_GAS_A2L, TestEnableIsHumidityCompensation);
    RUN_TEST_CASE(XENSIV_PAS_GAS_A2L, TestSetGetGasSelection);
    RUN_TEST_CASE(XENSIV_PAS_GAS_A2L, TestGetAvailableGases);
}

TEST_GROUP_RUNNER(XENSIV_PAS_GAS_A2L_I2C)
{
    currentSensorType = SENSOR_A2L;
    currentInterfaceType = SENSOR_IF_I2C;
    xensiv_pas_gas_a2l_suiteSetup();
    RUN_TEST_GROUP(XENSIV_PAS_GAS_A2L);
    xensiv_pas_gas_a2l_suiteTearDown();
}

TEST_GROUP_RUNNER(XENSIV_PAS_GAS_A2L_UART)
{
    currentSensorType = SENSOR_A2L;
    currentInterfaceType = SENSOR_IF_UART;
    xensiv_pas_gas_a2l_suiteSetup();
    RUN_TEST_GROUP(XENSIV_PAS_GAS_A2L);
    xensiv_pas_gas_a2l_suiteTearDown();
}