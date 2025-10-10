/** 
 * @file        xensiv_pas_gas_co2-ino.cpp
 * @brief       XENSIV™ PAS GAS CO2 Arduino API
 * @copyright   Copyright (c) 2025-2026 Infineon Technologies AG
 *              
 * SPDX-License-Identifier: MIT
 */

#include "xensiv_pas_gas_co2-ino.hpp"

/**
 * @brief   Assertion of XENSIV™ PAS GAS return code
 */
#define GASINO_ASSERT_RET(x)   if( x != XENSIV_PAS_GAS_OK ) { return x; }

/**
 * @brief       Resets the forced calibration correction factor
 * 
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         begin()
 */
Error_t XENSIV_PAS_GASCO2Ino::clearForcedCompensation()
{
    return xensiv_pas_gas_cmd(&dev,  (xensiv_pas_gas_cmd_t)XENSIV_PAS_GAS_CO2_CMD_RESET_FCS);
}

XENSIV_PAS_GASCO2Ino::~XENSIV_PAS_GASCO2Ino() {}