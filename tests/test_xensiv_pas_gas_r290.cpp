#include "test_arduino_includes.hpp"
#include "test_xensiv_pas_gas_globals.h"
#include "xensiv_pas_gas_r290_ino.hpp"

extern GasType_t currentSensorType;
extern SensorInterfaceType currentInterfaceType;

TEST_GROUP(XENSIV_PAS_GAS_R290);
static XENSIV_PAS_GASR290Ino *r290Sensor = nullptr;

TEST_SETUP(XENSIV_PAS_GAS_R290) {}
TEST_TEAR_DOWN(XENSIV_PAS_GAS_R290) {}

void xensiv_pas_gas_r290_suiteSetup()
{
    if (currentInterfaceType == SENSOR_IF_I2C)
    {
        Wire.begin();
        r290Sensor = new XENSIV_PAS_GASR290Ino(&Wire);
        gasSensor = r290Sensor;
    }
    else
    {
        Serial.begin(9600);
        r290Sensor = new XENSIV_PAS_GASR290Ino(&Serial);
        gasSensor = r290Sensor;
    }
    Error_t err = r290Sensor->begin();
    TEST_ASSERT_EQUAL(0, err);
}

void xensiv_pas_gas_r290_suiteTearDown()
{
    Error_t err = r290Sensor->end();
    TEST_ASSERT_EQUAL(0, err);
    delete r290Sensor;
    r290Sensor = nullptr;
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

TEST_IFX(XENSIV_PAS_GAS_R290, TestGetDeviceID)
{
    uint8_t devID = 0;
    Error_t err = r290Sensor->getDeviceID(devID);
    TEST_ASSERT_EQUAL(0, err);
    uint8_t regData[1];
    r290Sensor->getRegister(XENSIV_PAS_GAS_R290_REG_DEV_ID, regData, 1);
    uint8_t regDevID = (regData[0] & XENSIV_PAS_GAS_R290_REG_DEV_ID_VAL_MSK) >> XENSIV_PAS_GAS_R290_REG_DEV_ID_VAL_POS;
    TEST_ASSERT_EQUAL(regDevID, devID);
}

TEST_IFX(XENSIV_PAS_GAS_R290, TestGetAlarmConfig)
{
    bool activeHigh = false;
    Error_t err = r290Sensor->getAlarmConfig(activeHigh);
    TEST_ASSERT_EQUAL(0, err);
    uint8_t regData[1];
    r290Sensor->getRegister(XENSIV_PAS_GAS_R290_REG_ALARM_CFG, regData, 1);
    bool regActiveHigh = (regData[0] & XENSIV_PAS_GAS_R290_REG_ALARM_CFG_ALARM_POL_MSK) >>
                         XENSIV_PAS_GAS_R290_REG_ALARM_CFG_ALARM_POL_POS;
    TEST_ASSERT_EQUAL(regActiveHigh, activeHigh);
}

TEST_IFX(XENSIV_PAS_GAS_R290, TestSetGetABOCCycle)
{
    uint8_t daysSet = 7;
    Error_t err = r290Sensor->setABOCCycle(daysSet);
    TEST_ASSERT_EQUAL(0, err);
    uint8_t regData[1];
    r290Sensor->getRegister(XENSIV_PAS_GAS_R290_REG_ABOC_CYCLE, regData, 1);
    uint8_t regDays = (regData[0] & XENSIV_PAS_GAS_R290_REG_ABOC_CYCLE_SMOOTHING_FACT_MSK) >>
                      XENSIV_PAS_GAS_R290_REG_ABOC_CYCLE_SMOOTHING_FACT_POS;
    TEST_ASSERT_EQUAL(daysSet, regDays);
    uint8_t daysGet = 0;
    err = r290Sensor->getABOCCycle(daysGet);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(daysSet, daysGet);
}

TEST_IFX(XENSIV_PAS_GAS_R290, TestSetABOCPrefill)
{
    uint8_t hours = 12;
    Error_t err = r290Sensor->setABOCPrefill(hours);
    TEST_ASSERT_EQUAL(0, err);
}

TEST_IFX(XENSIV_PAS_GAS_R290, TestSetGetDenoiseConfig)
{
    uint8_t smoothingSet = 3;
    Error_t err = r290Sensor->setDenoiseConfig(smoothingSet);
    TEST_ASSERT_EQUAL(0, err);
    uint8_t regData[1];
    r290Sensor->getRegister(XENSIV_PAS_GAS_R290_REG_DENOISE_CFG, regData, 1);
    uint8_t regSmoothing = (regData[0] & XENSIV_PAS_GAS_R290_REG_DENOISE_CFG_SMOOTHING_FACT_MSK) >>
                           XENSIV_PAS_GAS_R290_REG_DENOISE_CFG_SMOOTHING_FACT_POS;
    TEST_ASSERT_EQUAL(smoothingSet, regSmoothing);
    uint8_t smoothingGet = 0;
    err = r290Sensor->getDenoiseConfig(smoothingGet);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(smoothingSet, smoothingGet);
}

TEST_IFX(XENSIV_PAS_GAS_R290, TestSelfTestErrorClear)
{
    Error_t err = r290Sensor->selfTestErrorClear(true);
    TEST_ASSERT_EQUAL(0, err);
}

TEST_IFX(XENSIV_PAS_GAS_R290, TestGetSelfTestStatusString)
{
    uint8_t regData[1];
    r290Sensor->getRegister(XENSIV_PAS_GAS_R290_REG_SELF_TEST, regData, 1);
    String status = r290Sensor->getSelfTestStatusString();
    TEST_ASSERT_TRUE(status.length() > 0);
}

TEST_GROUP_RUNNER(XENSIV_PAS_GAS_R290)
{

    // RUN_TEST_GROUP(XENSIV_PAS_GAS);
    RUN_TEST_CASE(XENSIV_PAS_GAS_R290, TestGetDeviceID);
    RUN_TEST_CASE(XENSIV_PAS_GAS_R290, TestGetAlarmConfig);
    RUN_TEST_CASE(XENSIV_PAS_GAS_R290, TestSetGetABOCCycle);
    RUN_TEST_CASE(XENSIV_PAS_GAS_R290, TestSetABOCPrefill);
    RUN_TEST_CASE(XENSIV_PAS_GAS_R290, TestSetGetDenoiseConfig);
    RUN_TEST_CASE(XENSIV_PAS_GAS_R290, TestSelfTestErrorClear);
    RUN_TEST_CASE(XENSIV_PAS_GAS_R290, TestGetSelfTestStatusString);
}

TEST_GROUP_RUNNER(XENSIV_PAS_GAS_R290_I2C)
{
    currentSensorType = SENSOR_R290;
    currentInterfaceType = SENSOR_IF_I2C;
    xensiv_pas_gas_r290_suiteSetup();
    RUN_TEST_GROUP(XENSIV_PAS_GAS_R290);
    xensiv_pas_gas_r290_suiteTearDown();
}

// TEST_GROUP_RUNNER(XENSIV_PAS_GAS_R290_UART)
// {
//     currentSensorType = SENSOR_R290;
//     currentInterfaceType = SENSOR_IF_UART;
//     xensiv_pas_gas_r290_suiteSetup();
//     RUN_TEST_GROUP(XENSIV_PAS_GAS_R290);
//     xensiv_pas_gas_r290_suiteTearDown();
// }