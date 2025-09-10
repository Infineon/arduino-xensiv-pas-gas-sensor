/***********************************************************************************************//**
 * \file xensiv_pas_gas_co2.c
 *
 * Description: This file contains the functions for interacting with the
 *              XENSIV™ PAS GAS CO2 sensor.
 *
 ***************************************************************************************************
 * \copyright
 * Copyright 2025-2026 Infineon Technologies AG
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **************************************************************************************************/

#include "xensiv_pas_gas_co2.h"

#define XENSIV_PAS_GAS_CO2_FCS_MEAS_RATE_S           (10)

/** Usage of the default functionalities from base class */
extern int32_t xensiv_pas_gas_base_init(xensiv_pas_gas_t *dev, xensiv_pas_gas_interface_t itf, void *ctx);
extern int32_t xensiv_pas_gas_base_perform_forced_compensation(const xensiv_pas_gas_t *dev, uint16_t gas_ref);

static int32_t xensiv_pas_gas_co2_perform_forced_compensation(const xensiv_pas_gas_t *dev, uint16_t gas_ref) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    int32_t res = xensiv_pas_gas_base_perform_forced_compensation(dev, gas_ref);

    if (XENSIV_PAS_GAS_OK == res) {
        res = xensiv_pas_gas_cmd(dev, XENSIV_PAS_GAS_CO2_CMD_SAVE_FCS_CALIB_OFFSET);
    }

    return res;
}

int32_t xensiv_pas_gas_co2_init(xensiv_pas_gas_t *dev, xensiv_pas_gas_interface_t itf, void *ctx) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(ctx != NULL);

    dev->fcs_meas_rate_s = XENSIV_PAS_GAS_CO2_FCS_MEAS_RATE_S;
    dev->meas_rate_min = XENSIV_PAS_GAS_CO2_MEAS_RATE_MIN;
    dev->force_comp = xensiv_pas_gas_co2_perform_forced_compensation;

    return xensiv_pas_gas_base_init(dev, itf, ctx);
}
