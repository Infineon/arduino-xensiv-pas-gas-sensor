// Advanced HIL: Validate alarm threshold registers after startMeasure
TEST_IFX(XENSIV_PAS_GAS, TestStartMeasureAlarmThreshold)
{
    int16_t alarmTh = 1234;
    gasSensor->startMeasure(0, alarmTh, nullptr, false);
    uint8_t regDataH[1], regDataL[1];
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_ALARM_TH_H, regDataH, 1);
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_ALARM_TH_L, regDataL, 1);
    uint16_t regAlarmTh = (regDataH[0] << 8) | regDataL[0];
    TEST_ASSERT_EQUAL(alarmTh, regAlarmTh);
}

// Advanced HIL: Validate INT_CFG after startMeasure with callback
TEST_IFX(XENSIV_PAS_GAS, TestStartMeasureInterruptConfig)
{
    volatile bool intFlag = false;
    auto cback = [](void *) { intFlag = true; };
    gasSensor->startMeasure(0, 0, cback, false);
    uint8_t regData[1];
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_INT_CFG, regData, 1);
    // Optionally check INT_CFG bits
    TEST_ASSERT_TRUE(regData[0] != 0);
}

// Advanced HIL: Validate status flag clearing after getDiagnosis
TEST_IFX(XENSIV_PAS_GAS, TestGetDiagnosisClearsFlags)
{
    Diag_t diag = 0;
    gasSensor->getDiagnosis(diag);
    uint8_t regData[1];
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_SENS_STS, regData, 1);
    // Optionally check that error/status flags are cleared
    TEST_ASSERT_TRUE((regData[0] & (XENSIV_PAS_GAS_REG_SENS_STS_ICCER_MSK | XENSIV_PAS_GAS_REG_SENS_STS_ORVS_MSK |
                                    XENSIV_PAS_GAS_REG_SENS_STS_ORTMP_MSK)) == 0);
}

// Advanced HIL: Validate MEAS_STS flags cleared after getGasConcentration
TEST_IFX(XENSIV_PAS_GAS, TestGetGasConcentrationClearsMeasSts)
{
    float value = 0.0f;
    gasSensor->getGasConcentration(value);
    uint8_t regData[1];
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_MEAS_STS, regData, 1);
    // Optionally check that INT_STS_CLR and ALARM_CLR bits are cleared
    TEST_ASSERT_TRUE(
        (regData[0] & (XENSIV_PAS_GAS_REG_MEAS_STS_INT_STS_CLR_MSK | XENSIV_PAS_GAS_REG_MEAS_STS_ALARM_CLR_MSK)) == 0);
}

// Advanced HIL: Boundary value test for setPressRef
TEST_IFX(XENSIV_PAS_GAS, TestSetPressRefBoundary)
{
    uint16_t minVal = 750, maxVal = 1150;
    gasSensor->setPressRef(minVal);
    uint8_t regData[2];
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_PRESS_REF_H, regData, 1);
    uint16_t regValue = regData[0] << 8;
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_PRESS_REF_L, regData + 1, 1);
    regValue |= regData[1];
    TEST_ASSERT_EQUAL(minVal, regValue);
    gasSensor->setPressRef(maxVal);
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_PRESS_REF_H, regData, 1);
    regValue = regData[0] << 8;
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_PRESS_REF_L, regData + 1, 1);
    regValue |= regData[1];
    TEST_ASSERT_EQUAL(maxVal, regValue);
}

// Advanced HIL: Multi-step sequence test
TEST_IFX(XENSIV_PAS_GAS, TestMeasurementSequence)
{
    gasSensor->startMeasure(10, 0, nullptr, false);
    float value = 0.0f;
    gasSensor->getGasConcentration(value);
    gasSensor->stopMeasure();
    uint8_t regData[1];
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_MEAS_CFG, regData, 1);
    // Check that op mode is idle after stopMeasure
    TEST_ASSERT_TRUE((regData[0] & XENSIV_PAS_GAS_REG_MEAS_CFG_OP_MODE_MSK) == XENSIV_PAS_GAS_OP_MODE_IDLE);
}
#include "corelib/xensiv_pas_gas_regs.h"
#include "test_arduino_includes.hpp"
#include "xensiv_pas_gas_ino.hpp"

TEST_GROUP(XENSIV_PAS_GAS);
static XENSIV_PAS_GASIno *gasSensor = nullptr;
static Error_t err;
static float testGasConc = 400.0f;                       // Example value for gas concentration
static uint8_t testRegAddr = XENSIV_PAS_GAS_REG_PROD_ID; // Example register

TEST_SETUP(XENSIV_PAS_GAS)
{
    GasType_t sensorType = SENSOR_CO2;
    gasSensor = new XENSIV_PAS_GASIno(sensorType, &Wire);
    Wire.begin();
}
TEST_TEAR_DOWN(XENSIV_PAS_GAS)
{
    delete gasSensor;
    gasSensor = nullptr;
}

TEST_IFX(XENSIV_PAS_GAS, TestSetPressRef)
{
    uint16_t testValue = 1013;
    gasSensor->setPressRef(testValue);
    uint8_t regData[2];
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_PRESS_REF_H, regData, 1);
    uint16_t regValue = regData[0] << 8;
    gasSensor->getRegister(XENSIV_PAS_GAS_REG_PRESS_REF_L, regData + 1, 1);
    regValue |= regData[1];
    TEST_ASSERT_EQUAL(testValue, regValue);

    TEST_IFX(XENSIV_PAS_GAS, TestSetABOC)
    {
        ABOC_t aboc = {0};
        int16_t abocRef = 400;
        gasSensor->setABOC(aboc, abocRef);
        uint8_t regData[2];
        gasSensor->getRegister(XENSIV_PAS_GAS_REG_ABOC_REF_H, regData, 1);
        uint16_t regValue = regData[0] << 8;
        gasSensor->getRegister(XENSIV_PAS_GAS_REG_ABOC_REF_L, regData + 1, 1);
        regValue |= regData[1];
        TEST_ASSERT_EQUAL(abocRef, regValue);
    }

    TEST_IFX(XENSIV_PAS_GAS, TestPerformForcedCompensation)
    {
        uint16_t testValue = 400;
        gasSensor->performForcedCompensation(testValue);
        uint8_t regData[2];
        gasSensor->getRegister(XENSIV_PAS_GAS_REG_GAS_REF_H, regData, 1);
        uint16_t regValue = regData[0] << 8;
        gasSensor->getRegister(XENSIV_PAS_GAS_REG_GAS_REF_L, regData + 1, 1);
        regValue |= regData[1];
        TEST_ASSERT_EQUAL(testValue, regValue);
    }

    TEST_IFX(XENSIV_PAS_GAS, TestClearForcedCompensation)
    {
        gasSensor->clearForcedCompensation();
        uint8_t regData[2];
        gasSensor->getRegister(XENSIV_PAS_GAS_REG_GAS_REF_H, regData, 1);
        uint16_t regValue = regData[0] << 8;
        gasSensor->getRegister(XENSIV_PAS_GAS_REG_GAS_REF_L, regData + 1, 1);
        regValue |= regData[1];
        TEST_ASSERT_EQUAL(0, regValue);
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
        float regConc = (float)regValue; // Conversion may be needed
        TEST_ASSERT_EQUAL(regConc, value);
    }

    TEST_IFX(XENSIV_PAS_GAS, TestGetDiagnosis)
    {
        Diag_t diag = 0;
        gasSensor->getDiagnosis(diag);
        uint8_t regData[1];
        gasSensor->getRegister(XENSIV_PAS_GAS_REG_DIAG, regData, 1);
        TEST_ASSERT_EQUAL(regData[0], diag);
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
        uint8_t regData[1] = {0xAA};
        gasSensor->setRegister(testRegAddr, regData, 1);
        uint8_t regRead[1];
        gasSensor->getRegister(testRegAddr, regRead, 1);
        TEST_ASSERT_EQUAL(regData[0], regRead[0]);
    }

    // All tests below are HIL (hardware-in-the-loop)

    TEST_IFX(XENSIV_PAS_GAS, TestEnd)
    {
        gasSensor->end();
        uint8_t regData[1];
        gasSensor->getRegister(XENSIV_PAS_GAS_REG_SENS_RST, regData, 1);
    }

    TEST_IFX(XENSIV_PAS_GAS, TestStartMeasure)
    {
        gasSensor->startMeasure(1, 0, nullptr, false);
        uint8_t regDataH[1], regDataL[1];
        gasSensor->getRegister(XENSIV_PAS_GAS_REG_MEAS_RATE_H, regDataH, 1);
        gasSensor->getRegister(XENSIV_PAS_GAS_REG_MEAS_RATE_L, regDataL, 1);
    }

    TEST_IFX(XENSIV_PAS_GAS, TestStopMeasure)
    {
        gasSensor->stopMeasure();
        uint8_t regData[1];
        gasSensor->getRegister(XENSIV_PAS_GAS_REG_MEAS_CFG, regData, 1);
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
}

// Example HIL-style test for getProductID
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
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestNoHardwareInit);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestSetPressRef);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestEnd);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestStartMeasure);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestStopMeasure);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestGetDiagnosis);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestSetABOC);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestPerformForcedCompensation);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestReset);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestGetProductID);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestGetRegister);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestSetRegister);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestClearForcedCompensation);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestGetGasConcentration);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestGetGasConcentrationUnitStr);
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestGetPasGasErrorStr);
}
