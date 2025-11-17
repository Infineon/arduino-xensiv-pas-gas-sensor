#include "test_arduino_includes.hpp"
#include "xensiv_pas_gas_r290_ino.hpp"

TEST_GROUP(XENSIV_PAS_GAS_R290);
static XENSIV_PAS_GASR290Ino *r290Sensor = nullptr;
static Error_t r290Err;
static uint8_t testRegAddr = XENSIV_PAS_GAS_REG_PROD_ID;

TEST_SETUP(XENSIV_PAS_GAS_R290)
{
    r290Sensor = new XENSIV_PAS_GASR290Ino(&Wire);
    Wire.begin();
}
TEST_TEAR_DOWN(XENSIV_PAS_GAS_R290)
{
    delete r290Sensor;
    r290Sensor = nullptr;
}

TEST_IFX(XENSIV_PAS_GAS_R290, TestGetDeviceID)
{
    uint8_t devID = 0;
    r290Sensor->getDeviceID(devID);
    uint8_t regData[1];
    r290Sensor->getRegister(XENSIV_PAS_GAS_R290_REG_DEV_ID, regData, 1);
    uint8_t regDevID = (regData[0] & XENSIV_PAS_GAS_R290_REG_DEV_ID_VAL_MSK) >> XENSIV_PAS_GAS_R290_REG_DEV_ID_VAL_POS;
    TEST_ASSERT_EQUAL(regDevID, devID);
}

TEST_IFX(XENSIV_PAS_GAS_R290, TestGetAlarmConfig)
{
    bool activeHigh = false;
    r290Sensor->getAlarmConfig(activeHigh);
    uint8_t regData[1];
    r290Sensor->getRegister(XENSIV_PAS_GAS_R290_REG_ALARM_CFG, regData, 1);
    bool regActiveHigh = (regData[0] & XENSIV_PAS_GAS_R290_REG_ALARM_CFG_ALARM_POL_MSK) >>
                         XENSIV_PAS_GAS_R290_REG_ALARM_CFG_ALARM_POL_POS;
    TEST_ASSERT_EQUAL(regActiveHigh, activeHigh);
}

TEST_IFX(XENSIV_PAS_GAS_R290, TestSetGetABOCCycle)
{
    uint8_t daysSet = 7;
    r290Sensor->setABOCCycle(daysSet);
    uint8_t regData[1];
    r290Sensor->getRegister(XENSIV_PAS_GAS_R290_REG_ABOC_CYCLE, regData, 1);
    uint8_t regDays = (regData[0] & XENSIV_PAS_GAS_R290_REG_ABOC_CYCLE_SMOOTHING_FACT_MSK) >>
                      XENSIV_PAS_GAS_R290_REG_ABOC_CYCLE_SMOOTHING_FACT_POS;
    TEST_ASSERT_EQUAL(daysSet, regDays);
    uint8_t daysGet = 0;
    r290Sensor->getABOCCycle(daysGet);
    TEST_ASSERT_EQUAL(daysSet, daysGet);
}

TEST_IFX(XENSIV_PAS_GAS_R290, TestSetABOCPrefill)
{
    uint8_t hours = 12;
    r290Sensor->setABOCPrefill(hours);
    uint8_t regData[1];
    r290Sensor->getRegister(XENSIV_PAS_GAS_R290_REG_ABOC_PREFILL, regData, 1);
    uint8_t regHours =
        (regData[0] & XENSIV_PAS_GAS_R290_REG_ABOC_PREFILL_VAL_MSK) >> XENSIV_PAS_GAS_R290_REG_ABOC_PREFILL_VAL_POS;
    TEST_ASSERT_EQUAL(hours, regHours);
}

TEST_IFX(XENSIV_PAS_GAS_R290, TestSetGetDenoiseConfig)
{
    uint8_t smoothingSet = 3;
    r290Sensor->setDenoiseConfig(smoothingSet);
    uint8_t regData[1];
    r290Sensor->getRegister(XENSIV_PAS_GAS_R290_REG_DENOISE_CFG, regData, 1);
    uint8_t regSmoothing = (regData[0] & XENSIV_PAS_GAS_R290_REG_DENOISE_CFG_SMOOTHING_FACT_MSK) >>
                           XENSIV_PAS_GAS_R290_REG_DENOISE_CFG_SMOOTHING_FACT_POS;
    TEST_ASSERT_EQUAL(smoothingSet, regSmoothing);
    uint8_t smoothingGet = 0;
    r290Sensor->getDenoiseConfig(smoothingGet);
    TEST_ASSERT_EQUAL(smoothingSet, smoothingGet);
}

TEST_IFX(XENSIV_PAS_GAS_R290, TestSelfTestErrorClear)
{
    r290Sensor->selfTestErrorClear(true);
    uint8_t regData[1];
    r290Sensor->getRegister(XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR, regData, 1);
    // Optionally check specific bits if needed
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
    RUN_TEST_CASE(XENSIV_PAS_GAS_R290, TestGetDeviceID);
    RUN_TEST_CASE(XENSIV_PAS_GAS_R290, TestGetAlarmConfig);
    RUN_TEST_CASE(XENSIV_PAS_GAS_R290, TestSetGetABOCCycle);
    RUN_TEST_CASE(XENSIV_PAS_GAS_R290, TestSetABOCPrefill);
    RUN_TEST_CASE(XENSIV_PAS_GAS_R290, TestSetGetDenoiseConfig);
    RUN_TEST_CASE(XENSIV_PAS_GAS_R290, TestSelfTestErrorClear);
    RUN_TEST_CASE(XENSIV_PAS_GAS_R290, TestGetSelfTestStatusString);
}
