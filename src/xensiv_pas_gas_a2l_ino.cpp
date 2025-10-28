/**
 * @file        xensiv_pas_gas_a2l_ino.cpp
 * @brief       XENSIV™ PAS GAS A2L Arduino API
 * @copyright   Copyright (c) 2025-2026 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#include "xensiv_pas_gas_a2l_ino.hpp"

/**
 * @brief   Assertion of XENSIV™ PAS GAS return code
 */
#define GASINO_ASSERT_RET(x)                                                                                           \
    if (x != XENSIV_PAS_GAS_OK)                                                                                        \
    {                                                                                                                  \
        return x;                                                                                                      \
    }

/**
 * @brief       Gets device product identifier
 *
 * @param[out]  devID  Device identifier
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         begin()
 */
Error_t XENSIV_PAS_GASA2LIno::getDeviceID(uint8_t &devID)
{
    int32_t ret = XENSIV_PAS_GAS_OK;
    ret = xensiv_pas_gas_a2l_get_device_id(&dev, &devID);
    GASINO_ASSERT_RET(ret);

    return ret;
}

/**
 * @brief       Gets the alarm pin configuration
 * @param[out]  activeHigh true for active high, false for active low
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         begin()
 */

Error_t XENSIV_PAS_GASA2LIno::getAlarmConfig(bool &activeHigh)
{
    int32_t ret = XENSIV_PAS_GAS_OK;
    uint8_t regval = 0;

    ret = xensiv_pas_gas_get_reg(&dev, XENSIV_PAS_GAS_A2L_REG_ALARM_CFG, &regval, 1U);
    GASINO_ASSERT_RET(ret);

    activeHigh = (regval & 0x01) ? true : false; // 0x00 = active low, 0x01 = active high
    return ret;
}

/**
 * @brief       Sets the ABOC cycle in days
 *
 * @param[in]   days  ABOC cycle in days (7 to 70 days)
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         begin()
 */

Error_t XENSIV_PAS_GASA2LIno::setABOCCycle(uint8_t days)
{
    int32_t ret = XENSIV_PAS_GAS_OK;
    xensiv_pas_gas_a2l_aboc_cycle_config_t abocCycle;

    if ((days < 7) || (days > 70))
    {
        return XENSIV_PAS_GAS_INVALID_PARAMETER;
    }

    abocCycle.b.aboc_cycle = days;

    ret = xensiv_pas_gas_a2l_set_aboc_cycle(&dev, abocCycle);
    GASINO_ASSERT_RET(ret);

    return ret;
}

/**
 * @brief       Gets the ABOC cycle in days
 *
 * @param[out]  days  ABOC cycle in days (7 to 70 days)
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         begin()
 */

Error_t XENSIV_PAS_GASA2LIno::getABOCCycle(uint8_t &days)
{
    int32_t ret = XENSIV_PAS_GAS_OK;
    xensiv_pas_gas_a2l_aboc_cycle_config_t abocCycle;

    ret = xensiv_pas_gas_a2l_get_aboc_cycle(&dev, &abocCycle);
    GASINO_ASSERT_RET(ret);

    days = abocCycle.b.aboc_cycle;
    return ret;
}

/**
 * @brief       Sets the ABOC prefill value (in hours)
 * @details     Prefills the ABOC flash table to shorten the current ABOC cycle.
 *              The value is capped at (ABOC cycle - 1) hours.
 * @param[in]   hours  Number of hours to prefill (0 to ABOC cycle - 1)
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         begin()
 */
Error_t XENSIV_PAS_GASA2LIno::setABOCPrefill(uint8_t hours)
{
    int32_t ret = XENSIV_PAS_GAS_OK;
    // Get current ABOC cycle to determine max allowed value
    uint8_t aboc_cycle = 0;

    ret = getABOCCycle(aboc_cycle);
    GASINO_ASSERT_RET(ret);

    uint8_t max_hours = (aboc_cycle > 1) ? (aboc_cycle - 1) : 0;
    if (hours > max_hours)
    {
        hours = max_hours; // Cap at max allowed value
    }

    // Write to ABOC_PREFILL register
    ret = xensiv_pas_gas_set_reg(&dev, XENSIV_PAS_GAS_A2L_REG_ABOC_PREFILL, &hours, 1U);
    GASINO_ASSERT_RET(ret);

    return ret;
}

/**
 * @brief       Sets the denoise configuration
 *
 * @param[in]   smoothing_factor
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         begin()
 */

Error_t XENSIV_PAS_GASA2LIno::setDenoiseConfig(uint8_t smoothing_factor)
{
    int32_t ret = XENSIV_PAS_GAS_OK;
    xensiv_pas_gas_a2l_denoise_config_t denoiseConfig;

    denoiseConfig.b.smoothing_factor = smoothing_factor;

    ret = xensiv_pas_gas_a2l_set_denoise_config(&dev, denoiseConfig);

    GASINO_ASSERT_RET(ret);
    return ret;
}

/**
 * @brief       Gets the denoise configuration
 *
 * @param[out]  smoothing_factor
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         begin()
 */
Error_t XENSIV_PAS_GASA2LIno::getDenoiseConfig(uint8_t &smoothing_factor)
{
    int32_t ret = XENSIV_PAS_GAS_OK;
    xensiv_pas_gas_a2l_denoise_config_t denoiseConfig;

    ret = xensiv_pas_gas_a2l_get_denoise_config(&dev, &denoiseConfig);

    GASINO_ASSERT_RET(ret);

    smoothing_factor = denoiseConfig.b.smoothing_factor;

    return ret;
}

/**
 * @brief       Gets a human-readable self-test status string
 *
 * @return      String describing self-test errors, or "Self-test passed: No errors."
 * @pre         begin()
 */

String XENSIV_PAS_GASA2LIno::getSelfTestStatusString()
{
    int32_t ret = XENSIV_PAS_GAS_OK;
    xensiv_pas_gas_a2l_self_test_t status;

    ret = xensiv_pas_gas_a2l_get_self_test(&dev, &status);
    if (ret != XENSIV_PAS_GAS_OK)
    {
        return "Self-test read error";
    }

    String result = "";
    if (status.b.voltage_err)
        result += "Voltage error; ";
    if (status.b.temp_err)
        result += "Temperature error; ";
    if (status.b.simic_err)
        result += "Simic error; ";
    if (status.b.emitter_err)
        result += "Emitter error; ";
    if (status.b.aboc_drift_err)
        result += "ABOC drift error; ";
    if (status.b.lifetime_err)
        result += "Lifetime error; ";
    if (status.b.replace_s_en)
        result += "Replace sensor enabled; ";
    if (result.length() == 0)
        result = "Self-test passed: No errors.";
    return result;
}

/**
 * @brief       Configures the self-test routine
 *
 * @param[in]   enable  true to enable, false to disable
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         begin()
 */
Error_t XENSIV_PAS_GASA2LIno::selfTestErrorClear(bool clear)
{
    int32_t ret = XENSIV_PAS_GAS_OK;
    xensiv_pas_gas_a2l_self_test_clr_t clr = {0};
    if (clear)
    {
        clr.b.voltage_err_clr = 1;
        clr.b.temp_err_clr = 1;
        clr.b.simic_err_clr = 1;
        clr.b.emitter_err_clr = 1;
        clr.b.aboc_drift_err_clr = 1;
    }

    ret = xensiv_pas_gas_a2l_clr_self_test(&dev, clr);
    GASINO_ASSERT_RET(ret);

    return ret;
}

/**
 * @brief       Sets the alarm hysteresis value
 * @param[in]   alarm_hys  Alarm hysteresis value (0–32767)
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         begin()
 */
Error_t XENSIV_PAS_GASA2LIno::setAlarmHysteresis(uint16_t alarm_hys)
{
    int32_t ret = XENSIV_PAS_GAS_OK;
    if (alarm_hys > 32767)
    {
        return XENSIV_PAS_GAS_INVALID_PARAMETER;
    }
    ret = xensiv_pas_gas_a2l_set_alarm_hysteresis(&dev, alarm_hys);

    return ret;
}

/**
 * @brief       Gets the alarm hysteresis value
 * @param[out]  alarm_hys  Alarm hysteresis value (0–32767)
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         begin()
 */
Error_t XENSIV_PAS_GASA2LIno::getAlarmHysteresis(uint16_t &alarm_hys)
{
    int32_t ret = XENSIV_PAS_GAS_OK;
    ret = xensiv_pas_gas_a2l_get_alarm_hysteresis(&dev, &alarm_hys);

    return ret;
}

/**
 * @brief       Gets the absolute humidity reference value
 * @param[out]  abs_humidity  Absolute humidity reference value
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         begin()
 */
Error_t XENSIV_PAS_GASA2LIno::getAbsoluteHumidityRef(uint16_t &abs_humidity)
{
    int32_t ret = XENSIV_PAS_GAS_OK;
    ret = xensiv_pas_gas_a2l_get_absolute_humidity_ref(&dev, &abs_humidity);

    return ret;
}

/**
 * @brief       Sets the absolute humidity reference value
 * @param[in]   abs_humidity  Absolute humidity reference value
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         begin()
 */
Error_t XENSIV_PAS_GASA2LIno::setAbsoluteHumidityRef(uint16_t abs_humidity)
{
    int32_t ret = XENSIV_PAS_GAS_OK;
    ret = xensiv_pas_gas_a2l_set_absolute_humidity_ref(&dev, abs_humidity);

    return ret;
}

/**
 * @brief       Enables or disables humidity compensation
 * @param[in]   enable  true to enable, false to disable
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         begin()
 */
Error_t XENSIV_PAS_GASA2LIno::enableHumidityCompensation(bool enable)
{

    int32_t ret = XENSIV_PAS_GAS_OK;
    xensiv_pas_gas_a2l_humidity_control_t humidityControl;

    humidityControl.b.hc_enable = enable;

    ret = xensiv_pas_gas_a2l_set_humidity_control(&dev, humidityControl);

    return ret;
}

/**
 * @brief       Checks if humidity compensation is enabled
 * @param[out]  enable  true if enabled, false otherwise
 * @return      XENSIV™ PAS GAS error code
 * @retval      XENSIV_PAS_GAS_OK if success
 * @pre         begin()
 */
Error_t XENSIV_PAS_GASA2LIno::isHumidityCompensationEnabled(bool enable)
{
    xensiv_pas_gas_a2l_humidity_control_t hc_ctrl = {0};
    int32_t ret = xensiv_pas_gas_a2l_get_humidity_control(&dev, &hc_ctrl);
    if (ret != XENSIV_PAS_GAS_OK)
    {
        enable = false;
        return ret;
    }
    enable = hc_ctrl.b.hc_enable;
    return XENSIV_PAS_GAS_OK;
}

/**
 * @brief Sets the gas selection (GAS_SEL field)
 * @param[in] gas  Gas to select (enum value)
 * @return XENSIV™ PAS GAS error code
 */
Error_t XENSIV_PAS_GASA2LIno::setGasSelection(xensiv_pas_gas_a2l_gas_selection_t gas)
{
    xensiv_pas_gas_a2l_gas_config_t gas_cfg = {0};

    int32_t ret = xensiv_pas_gas_a2l_get_gas_config(&dev, &gas_cfg);
    if (ret != XENSIV_PAS_GAS_OK)
        return ret;

    gas_cfg.b.gas_select = (uint8_t)gas;

    return xensiv_pas_gas_a2l_set_gas_config(&dev, gas_cfg);
}

/**
 * @brief Gets the current gas selection (GAS_SEL field)
 * @param[out] gas  Gas selection (enum value)
 * @return XENSIV™ PAS GAS error code
 */
Error_t XENSIV_PAS_GASA2LIno::getGasSelection(xensiv_pas_gas_a2l_gas_selection_t &gas)
{
    return xensiv_pas_gas_a2l_get_gas_selection(&dev, &gas);
}

/**
 * @brief Gets the available gases bitmap (GAS_AVAIL field, bits 7:4)
 * @param[out] gas_avail  Bitmap of available gases (bits 7:4)
 * @return XENSIV™ PAS GAS error code
 */
Error_t XENSIV_PAS_GASA2LIno::getAvailableGases(uint8_t &gas_avail)
{
    return xensiv_pas_gas_a2l_get_available_gases(&dev, &gas_avail);
}

/**
 * @brief       Destructor of the XENSIV_PAS_GASA2LIno class
 */
XENSIV_PAS_GASA2LIno::~XENSIV_PAS_GASA2LIno() {}