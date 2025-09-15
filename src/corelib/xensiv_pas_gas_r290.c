/***********************************************************************************************//**
 * \file xensiv_pas_gas_r290.c
 *
 * Description: This file contains the functions for interacting with the
 *              XENSIV™ PAS GAS R290 sensor.
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
#include "xensiv_pas_gas_r290.h"

#define XENSIV_PAS_GAS_R290_FCS_MEAS_RATE_S           (3)

/** Usage of the default functionalities from base class */
extern int32_t xensiv_pas_gas_base_init(xensiv_pas_gas_t *dev, xensiv_pas_gas_interface_t itf, void *ctx);
extern int32_t xensiv_pas_gas_base_perform_forced_compensation(const xensiv_pas_gas_t *dev, uint16_t gas_ref);

int32_t xensiv_pas_gas_r290_get_device_id(const xensiv_pas_gas_t *dev, void *dev_id) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(dev_id != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_R290_REG_DEV_ID, (uint8_t *)dev_id, 1U);
}

int32_t xensiv_pas_gas_r290_aboc_prefill(const xensiv_pas_gas_t *dev, uint8_t prefill) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_R290_REG_ABOC_PREFILL, (uint8_t *)&prefill, 1U);
}

int32_t xensiv_pas_gas_r290_set_alarm_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_r290_alarm_config_t alarm_config) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_R290_REG_ALARM_CFG, &alarm_config.u, 1U);
}

int32_t xensiv_pas_gas_r290_get_alarm_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_r290_alarm_config_t *alarm_config) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(alarm_config != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_R290_REG_ALARM_CFG, &(alarm_config->u), 1U);
}

int32_t xensiv_pas_gas_r290_set_aboc_cycle(const xensiv_pas_gas_t *dev, xensiv_pas_gas_r290_aboc_cycle_config_t aboc_cycle) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_R290_REG_ABOC_CYCLE, &aboc_cycle.u, 1U);
}

int32_t xensiv_pas_gas_r290_get_aboc_cycle(const xensiv_pas_gas_t *dev, xensiv_pas_gas_r290_aboc_cycle_config_t *aboc_cycle) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(aboc_cycle != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_R290_REG_ABOC_CYCLE, &(aboc_cycle->u), 1U);
}

int32_t xensiv_pas_gas_r290_set_denoise_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_r290_denoise_config_t denoise_config) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_R290_REG_DENOISE_CFG, &denoise_config.u, 1U);
}

int32_t xensiv_pas_gas_r290_get_denoise_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_r290_denoise_config_t *denoise_config) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(denoise_config != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_R290_REG_DENOISE_CFG, &denoise_config->u, 1U);
}

int32_t xensiv_pas_gas_r290_get_self_test(const xensiv_pas_gas_t *dev, xensiv_pas_gas_r290_self_test_t *self_test) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(self_test != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_R290_REG_SELF_TEST, &self_test->u, 1U);
}

int32_t xensiv_pas_gas_r290_clr_self_test(const xensiv_pas_gas_t *dev, xensiv_pas_gas_r290_self_test_clr_t self_test_clr) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR, &self_test_clr.u, 1U);
}

int32_t xensiv_pas_gas_r290_init(xensiv_pas_gas_t *dev, xensiv_pas_gas_interface_t itf, void *ctx) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(ctx != NULL);

    dev->fcs_meas_rate_s = XENSIV_PAS_GAS_R290_FCS_MEAS_RATE_S;
    dev->meas_rate_min = XENSIV_PAS_GAS_R290_MEAS_RATE_MIN;
    dev->force_comp = xensiv_pas_gas_base_perform_forced_compensation;

    return xensiv_pas_gas_base_init(dev, itf, ctx);
}
