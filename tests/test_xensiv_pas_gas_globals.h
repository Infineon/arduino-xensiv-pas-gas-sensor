#ifndef TEST_XENSIV_PAS_GAS_GLOBALS_H
#define TEST_XENSIV_PAS_GAS_GLOBALS_H

#include "xensiv_pas_gas_ino.hpp"

// Interface type enum
enum SensorInterfaceType { SENSOR_IF_I2C, SENSOR_IF_UART };

// Global variables for test parameterization
extern XENSIV_PAS_GASIno *gasSensor;
extern SensorInterfaceType currentInterfaceType;
extern GasType_t currentSensorType;

#endif // TEST_XENSIV_PAS_GAS_GLOBALS_H