#include "test_arduino_includes.hpp"
#include "xensiv_pas_gas_ino.hpp"

TEST_GROUP(XENSIV_PAS_GAS);


static XENSIV_PAS_GASIno* gasSensor = nullptr;
static Error_t err;

TEST_SETUP(XENSIV_PAS_GAS) {
    // Create sensor instance with R290 type, no sensor connected but with I2C interface
    GasType_t sensorType = SENSOR_R290;
    gasSensor = new XENSIV_PAS_GASIno(sensorType, &Wire);
    Wire.begin();
}

TEST_TEAR_DOWN(XENSIV_PAS_GAS) {
    delete gasSensor;
    gasSensor = nullptr;
}

// Test sensor initialization without hardware
TEST_IFX(XENSIV_PAS_GAS, TestNoHardwareInit) {
    err = gasSensor->begin();
    TEST_ASSERT_NOT_EQUAL(XENSIV_PAS_GAS_OK, err);
    // Verify error string is available
    TEST_ASSERT_NOT_NULL(gasSensor->getPasGasErrorStr(err));
}


TEST_GROUP_RUNNER(XENSIV_PAS_GAS) {
    RUN_TEST_CASE(XENSIV_PAS_GAS, TestNoHardwareInit);
}