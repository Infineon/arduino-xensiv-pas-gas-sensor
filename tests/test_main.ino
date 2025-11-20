// test includes
#include "test_arduino_includes.hpp"

    /***
      * Uses flags / defines to determine which groups of tests should be added to this test such that multiple tests can be executed in a single run.
      * Use the '--build-property option of the 'arduino_compile' target to add the flags by defining the respective targets in the makefile.
      * makefile : --build-property "compiler.cpp.extra_flags=\"-D<TEST_SPECIFIER>=1\"" build
    */
void RunAllTests(void)
{
#ifdef TEST_XENSIV_PAS_GAS
    //RUN_TEST_GROUP(XENSIV_PAS_GAS_CO2_I2C);
    RUN_TEST_GROUP(XENSIV_PAS_GAS_CO2_UART);
#endif
#ifdef TEST_XENSIV_PAS_GAS_R290
    //RUN_TEST_GROUP(XENSIV_PAS_GAS_R290_I2C);
    RUN_TEST_GROUP(XENSIV_PAS_GAS_R290_UART);
#endif
#ifdef TEST_XENSIV_PAS_GAS_A2L
    //RUN_TEST_GROUP(XENSIV_PAS_GAS_A2L_I2C);
    RUN_TEST_GROUP(XENSIV_PAS_GAS_A2L_UART);
#endif
}

void setup() {
    Serial.begin(115200);

    while( !Serial ) ;

    delay(3000);

    Serial.print("setup done.\n");
}

void loop() {
    Serial.println("\n");

    const int   argc       = 2;
    const char *argv[argc] = { "", "-v" };

    (void) UnityMain(argc, argv, RunAllTests);
    delay(3000);
}