/***********************************************************************************************//**
 * \file xensiv_pas_gas_a2l.c
 *
 * Description: This file contains the functions for interacting with the
 *              XENSIV™ PAS GAS A2L sensor.
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
#include "xensiv_pas_gas_a2l.h"

/** Usage of the default functionalities from base class */
extern int32_t xensiv_pas_gas_base_init(xensiv_pas_gas_t *dev, xensiv_pas_gas_interface_t itf, void *ctx);
extern int32_t xensiv_pas_gas_base_perform_forced_compensation(const xensiv_pas_gas_t *dev, uint16_t gas_ref);


int32_t xenisv_pas_gas_a2l_set_dev_idx(const xensiv_pas_gas_t *dev, uint8_t dev_idx) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_DEV_ID_IDX, (uint8_t *)&dev_idx, 1U);
}

int32_t xensiv_pas_gas_a2l_get_dev_idx(const xensiv_pas_gas_t *dev, uint8_t *dev_idx) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(dev_idx != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_DEV_ID_IDX, dev_idx, 1U);
}

int32_t xensiv_pas_gas_a2l_get_device_id(const xensiv_pas_gas_t *dev, void *dev_id) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(dev_id != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_DEV_ID, (uint8_t *)dev_id, 1U);
}

int32_t xensiv_pas_gas_a2l_aboc_prefill(const xensiv_pas_gas_t *dev, uint8_t prefill) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_ABOC_PREFILL, (uint8_t *)&prefill, 1U);
}

int32_t xensiv_pas_gas_a2l_set_alarm_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_alarm_config_t alarm_config) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_ALARM_CFG, &alarm_config.u, 1U);
}

int32_t xensiv_pas_gas_a2l_get_alarm_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_alarm_config_t *alarm_config) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(alarm_config != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_ALARM_CFG, &(alarm_config->u), 1U);
}

int32_t xensiv_pas_gas_a2l_set_aboc_cycle(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_aboc_cycle_config_t aboc_cycle) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_ABOC_CYCLE, &aboc_cycle.u, 1U);
}

int32_t xensiv_pas_gas_a2l_get_aboc_cycle(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_aboc_cycle_config_t *aboc_cycle) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(aboc_cycle != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_ABOC_CYCLE, &(aboc_cycle->u), 1U);
}

int32_t xensiv_pas_gas_a2l_set_denoise_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_denoise_config_t denoise_config) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_DENOISE_CFG, &denoise_config.u, 1U);
}

int32_t xensiv_pas_gas_a2l_get_denoise_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_denoise_config_t *denoise_config) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(denoise_config != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_DENOISE_CFG, &denoise_config->u, 1U);
}

int32_t xensiv_pas_gas_a2l_get_self_test(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_self_test_t *self_test) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(self_test != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_SELF_TEST, &self_test->u, 1U);
}

int32_t xensiv_pas_gas_a2l_clr_self_test(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_self_test_clr_t self_test_clr) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR, &self_test_clr.u, 1U);
}

int32_t xensiv_pas_gas_a2l_set_gas_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_gas_config_t gas_config) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    // Ensure reserved bits 3:2 are zero before writing, only bits 1:0 are set
    uint8_t regval = (gas_config.u & 0xF3) | (gas_config.u & 0x03);
    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_GAS_CFG, &regval, 1U);
}

int32_t xensiv_pas_gas_a2l_get_gas_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_gas_config_t *gas_config) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(gas_config != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_GAS_CFG, &gas_config->u, 1U);
}

int32_t xensiv_pas_gas_a2l_get_gas_selection(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_gas_selection_t *gas) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(gas != NULL);
    xensiv_pas_gas_a2l_gas_config_t gas_cfg = {0};
    int32_t ret = xensiv_pas_gas_a2l_get_gas_config(dev, &gas_cfg);
    if (ret != XENSIV_PAS_GAS_OK) {
        return ret;
    }
    *gas = (xensiv_pas_gas_a2l_gas_selection_t)(gas_cfg.b.gas_select & 0x03);
    return XENSIV_PAS_GAS_OK;
}

int32_t xensiv_pas_gas_a2l_get_available_gases(const xensiv_pas_gas_t *dev, uint8_t *gas_avail) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(gas_avail != NULL);
    xensiv_pas_gas_a2l_gas_config_t gas_cfg = {0};
    int32_t ret = xensiv_pas_gas_a2l_get_gas_config(dev, &gas_cfg);
    if (ret != XENSIV_PAS_GAS_OK) {
        return ret;
    }
    *gas_avail = gas_cfg.b.gas_avail;
    return XENSIV_PAS_GAS_OK;
}

int32_t xensiv_pas_gas_a2l_set_alarm_hysteresis(const xensiv_pas_gas_t *dev, uint16_t alarm_hys) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    // Only 15 bits are valid, mask input
    alarm_hys &= 0x7FFF;

    // Prepare register values
    uint8_t reg_h = (uint8_t)((alarm_hys >> 8) & 0x7F); // Only bits 6:0 for VAL
    uint8_t reg_l = (uint8_t)(alarm_hys & 0xFF);        // All bits for VAL
    uint8_t buf[2] = {reg_h, reg_l};

    int32_t ret = xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_ALARM_HYS_H, buf, 2U);
    return ret;
}

int32_t xensiv_pas_gas_a2l_get_alarm_hysteresis(const xensiv_pas_gas_t *dev, uint16_t *alarm_hys) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(alarm_hys != NULL);

    uint8_t buf[2] = {0};
    int32_t ret = xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_ALARM_HYS_H, buf, 2U);

    // Only 7 bits from H, 8 bits from L
    *alarm_hys = (((uint16_t)(buf[0] & 0x7F)) << 8) | buf[1];
    return ret;
}

int32_t xensiv_pas_gas_a2l_set_absolute_humidity_ref(const xensiv_pas_gas_t *dev, uint16_t abs_hum_ref) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    // Only 10 bits are valid
    abs_hum_ref &= 0x03FF;

    // Prepare register values
    uint8_t reg_h = (uint8_t)((abs_hum_ref >> 8) & 0x03); // bits 1:0
    uint8_t reg_l = (uint8_t)(abs_hum_ref & 0xFF);        // bits 7:0
    int32_t ret = xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_H, &reg_h, 1U);
    if (ret == 0) {
        ret = xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_L, &reg_l, 1U);
    }
    return ret;
}

int32_t  xensiv_pas_gas_a2l_get_absolute_humidity_ref(const xensiv_pas_gas_t *dev, uint16_t *abs_hum_ref) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(abs_hum_ref != NULL);

    uint8_t reg_h = 0, reg_l = 0;
    int32_t ret = xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_H, &reg_h, 1U);
    if (ret == 0) {
        ret = xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_L, &reg_l, 1U);
    }
    *abs_hum_ref = (((uint16_t)(reg_h & 0x03)) << 8) | reg_l;
    return ret;
}

int32_t xensiv_pas_gas_a2l_set_humidity_control(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_humidity_control_t hum_control) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_HC_CTRL, &hum_control.u, 1U);
}

int32_t xensiv_pas_gas_a2l_get_humidity_control(const xensiv_pas_gas_t *dev, xensiv_pas_gas_a2l_humidity_control_t *hum_control) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(hum_control != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_A2L_REG_HC_CTRL, &(hum_control->u), 1U);
}

int32_t xensiv_pas_gas_a2l_init(xensiv_pas_gas_t *dev, xensiv_pas_gas_interface_t itf, void *ctx) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(ctx != NULL);

    dev->fcs_meas_rate_s = XENSIV_PAS_GAS_A2L_FCS_MEAS_RATE_S;
    dev->meas_rate_min = XENSIV_PAS_GAS_A2L_MEAS_RATE_MIN;
    dev->force_comp = xensiv_pas_gas_base_perform_forced_compensation;

    return xensiv_pas_gas_base_init(dev, itf, ctx);
}
