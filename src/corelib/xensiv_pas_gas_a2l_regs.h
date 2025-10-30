/***********************************************************************************************//**
 * \file xensiv_pas_gas_a2l_regs.h
 *
 * Description: This file contains the register definitions
 *              for interacting with the XENSIV™ PAS A2L gas sensor.
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

#ifndef XENSIV_PAS_GAS_A2L_REGS_H_
#define XENSIV_PAS_GAS_A2L_REGS_H_

#include "xensiv_pas_gas_regs.h"

/**
 * \addtogroup group_board_libs XENSIV™ PAS GAS A2L sensor
 * \{
 */

// --------------------------A2L Sensor Register Addresses--------------------------- //
#define XENSIV_PAS_GAS_A2L_REG_CFG_SAVE             (0x20U)                                                                                 /**< CFG_SAVE: Save configuration register address */
#define XENSIV_PAS_GAS_A2L_REG_DEV_ID_IDX           (0x22U)                                                                                 /**< DEV_ID_IDX: Device ID index register address */
#define XENSIV_PAS_GAS_A2L_REG_DEV_ID               (0x23U)                                                                                 /**< DEV_ID: Device ID register address */
#define XENSIV_PAS_GAS_A2L_REG_ABOC_PREFILL         (0x5DU)                                                                                 /**< ABOC_PREFILL: ABOC prefill register address */
#define XENSIV_PAS_GAS_A2L_REG_GAS_CFG              (0x5EU)                                                                                 /**< GAS_CFG: Gas configuration register address */
#define XENSIV_PAS_GAS_A2L_REG_ALARM_CFG            (0x5FU)                                                                                 /**< ALARM_CFG: Alarm configuration register address */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST            (0x60U)                                                                                 /**< SELF_TEST: Self-test status register address */
#define XENSIV_PAS_GAS_A2L_REG_DENOISE_CFG          (0x61U)                                                                                 /**< DENOISE_CFG: Denoise configuration register address */
#define XENSIV_PAS_GAS_A2L_REG_ABOC_CYCLE           (0x62U)                                                                                 /**< ABOC_CYCLE: ABOC cycle register address */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR        (0x63U)                                                                                 /**< SELF_TEST_CLR: Self-test clear register address */
#define XENSIV_PAS_GAS_A2L_REG_ALARM_HYS_H          (0x64U)                                                                                 /**< ALARM_HYS_H: Alarm hysteresis high register address */
#define XENSIV_PAS_GAS_A2L_REG_ALARM_HYS_L          (0x65U)                                                                                 /**< ALARM_HYS_L: Alarm hysteresis low register address */
#define XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_H        (0x66U)                                                                                 /**< ABS_HUM_REF_H: Absolute humidity reference high register address */
#define XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_L        (0x67U)                                                                                 /**< ABS_HUM_REF_L: Absolute humidity reference low register address */
#define XENSIV_PAS_GAS_A2L_REG_HC_CTRL              (0x68U)                                                                                 /**< HC_CTRL: Humidity compensation control register address */


// ---------------------------- CFG_SAVE --------------------------- //
#define XENSIV_PAS_GAS_A2L_REG_CFG_SAVE_VAL_POS         (0x3U)                                                                              /**< CFG_SAVE: Value position */
#define XENSIV_PAS_GAS_A2L_REG_CFG_SAVE_VAL_MASK        (0x03U << XENSIV_PAS_GAS_A2L_REG_CFG_SAVE_VAL_POS)                                  /**< CFG_SAVE: Value mask */

// ------------------------------- DEV_ID_IDX ------------------------------- //
#define XENSIV_PAS_GAS_A2L_REG_DEV_ID_IDX_POS          (0x0U)                                                                               /**< DEV_ID_IDX: Index position */
#define XENSIV_PAS_GAS_A2L_REG_DEV_ID_IDX_MASK         (0xffU << XENSIV_PAS_GAS_A2L_REG_DEV_ID_IDX_POS)                                     /**< DEV_ID_IDX: Index mask */

// --------------------------------------- DEV_ID --------------------------- //
#define XENSIV_PAS_GAS_A2L_REG_DEV_ID_VAL_POS            (0U)                                                                               /**< DEV_ID: Value position */
#define XENSIV_PAS_GAS_A2L_REG_DEV_ID_VAL_MSK            (0xffU << XENSIV_PAS_GAS_A2L_REG_DEV_ID_VAL_POS)                                   /**< DEV_ID: Value mask */

// -------------------------------- ABOC_PREFILL ----------------------------------- //
#define XENSIV_PAS_GAS_A2L_REG_ABOC_PREFILL_VAL_POS      (0U)                                                                               /**< ABOC_PREFILL: Value position */
#define XENSIV_PAS_GAS_A2L_REG_ABOC_PREFILL_VAL_MSK      (0xffU << XENSIV_PAS_GAS_A2L_REG_ABOC_PREFILL_VAL_POS)                             /**< ABOC_PREFILL: Value mask */

// ------------------------------- GAS_CFG ------------------------------- //
#define XENSIV_PAS_GAS_A2L_REG_GAS_CFG_GAS_SEL_POS      (0U)                                                                                /**< GAS_CFG: Gas selection position */
#define XENSIV_PAS_GAS_A2L_REG_GAS_CFG_GAS_SEL_MASK     (0x03U << XENSIV_PAS_GAS_A2L_REG_GAS_CFG_GAS_SEL_POS)                               /**< GAS_CFG: Gas selection mask */
#define XENSIV_PAS_GAS_A2L_REG_GAS_CFG_GAS_AVAIL_POS    (4U)                                                                                /**< GAS_CFG: Gas available position */
#define XENSIV_PAS_GAS_A2L_REG_GAS_CFG_GAS_AVAIL_MASK   (0x0FU << XENSIV_PAS_GAS_A2L_REG_GAS_CFG_GAS_AVAIL_POS)                             /**< GAS_CFG: Gas available mask */

// ------------------------------- ALARM_CFG ------------------------------- //
#define XENSIV_PAS_GAS_A2L_REG_ALARM_CFG_ALARM_POL_POS   (0U)                                                                               /**< ALARM_CFG: Alarm polarity position */
#define XENSIV_PAS_GAS_A2L_REG_ALARM_CFG_ALARM_POL_MSK   (0x01U << XENSIV_PAS_GAS_A2L_REG_ALARM_CFG_ALARM_POL_POS)                          /**< ALARM_CFG: Alarm polarity mask */

// ------------------------------- SELF_TEST ------------------------------- //
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_VOLTAGE_ERR_POS     (0U)                                                                           /**< SELF_TEST: Voltage error position */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_VOLTAGE_ERR_MSK     (0x01U << XENSIV_PAS_GAS_A2L_REG_SELF_TEST_VOLTAGE_ERR_POS)                    /**< SELF_TEST: Voltage error mask */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_TEMP_ERR_POS        (1U)                                                                           /**< SELF_TEST: Temperature error position */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_TEMP_ERR_MSK        (0x01U << XENSIV_PAS_GAS_A2L_REG_SELF_TEST_TEMP_ERR_POS)                       /**< SELF_TEST: Temperature error mask */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_SIMIC_ERR_POS       (2U)                                                                           /**< SELF_TEST: Simic error position */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_SIMIC_ERR_MSK       (0x01U << XENSIV_PAS_GAS_A2L_REG_SELF_TEST_SIMIC_ERR_POS)                      /**< SELF_TEST: Simic error mask */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_EMITTER_ERR_POS     (3U)                                                                           /**< SELF_TEST: Emitter error position */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_EMITTER_ERR_MSK     (0x01U << XENSIV_PAS_GAS_A2L_REG_SELF_TEST_EMITTER_ERR_POS)                    /**< SELF_TEST: Emitter error mask */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_ABOC_DRIFT_ERR_POS  (4U)                                                                           /**< SELF_TEST: ABOC drift error position */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_ABOC_DRIFT_ERR_MSK  (0x01U << XENSIV_PAS_GAS_A2L_REG_SELF_TEST_ABOC_DRIFT_ERR_POS)                 /**< SELF_TEST: ABOC drift error mask */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_LIFETIME_ERR_POS    (5U)                                                                           /**< SELF_TEST: Lifetime error position */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_LIFETIME_ERR_MSK    (0x01U << XENSIV_PAS_GAS_A2L_REG_SELF_TEST_LIFETIME_ERR_POS)                   /**< SELF_TEST: Lifetime error mask */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_REPLACE_SEN_POS     (7U)                                                                           /**< SELF_TEST: Replace sensor position */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_REPLACE_SEN_MSK     (0x01U << XENSIV_PAS_GAS_A2L_REG_SELF_TEST_REPLACE_SEN_POS)                    /**< SELF_TEST: Replace sensor mask */

// ------------------------ DENOISE_CFG ------------------------//
#define XENSIV_PAS_GAS_A2L_REG_DENOISE_CFG_SMOOTHING_FACT_POS    (0U)                                                                       /**< DENOISE_CFG: Smoothing factor position */
#define XENSIV_PAS_GAS_A2L_REG_DENOISE_CFG_SMOOTHING_FACT_MSK    (0x3fU << XENSIV_PAS_GAS_A2L_REG_DENOISE_CFG_SMOOTHING_FACT_POS)           /**< DENOISE_CFG: Smoothing factor mask */

// ---------------------------- ABOC_CYCLE ---------------------------- //
#define XENSIV_PAS_GAS_A2L_REG_ABOC_CYCLE_SMOOTHING_FACT_POS     (0U)                                                                       /**< ABOC_CYCLE: Smoothing factor position */
#define XENSIV_PAS_GAS_A2L_REG_ABOC_CYCLE_SMOOTHING_FACT_MSK     (0x3fU << XENSIV_PAS_GAS_A2L_REG_ABOC_CYCLE_SMOOTHING_FACT_POS)            /**< ABOC_CYCLE: Smoothing factor mask */

// ---------------------------- SELF_TEST_CLR ---------------------------- //
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR_VOLTAGE_ERR_CLR_POS     (0U)                                                                   /**< SELF_TEST_CLR: Voltage error clear position */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR_VOLTAGE_ERR_CLR_MSK     (0x01U << XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR_VOLTAGE_ERR_CLR_POS)    /**< SELF_TEST_CLR: Voltage error clear mask */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR_TEMP_ERR_CLR_POS        (1U)                                                                   /**< SELF_TEST_CLR: Temperature error clear position */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR_TEMP_ERR_CLR_MSK        (0x01U << XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR_TEMP_ERR_CLR_POS)       /**< SELF_TEST_CLR: Temperature error clear mask */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR_SIMIC_ERR_CLR_POS       (2U)                                                                   /**< SELF_TEST_CLR: Simic error clear position */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR_SIMIC_ERR_CLR_MSK       (0x01U << XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR_SIMIC_ERR_CLR_POS)      /**< SELF_TEST_CLR: Simic error clear mask */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR_EMITTER_ERR_CLR_POS     (3U)                                                                   /**< SELF_TEST_CLR: Emitter error clear position */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR_EMITTER_ERR_CLR_MSK     (0x01U << XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR_EMITTER_ERR_CLR_POS)    /**< SELF_TEST_CLR: Emitter error clear mask */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR_ABOC_DRIFT_ERR_CLR_POS  (4U)                                                                   /**< SELF_TEST_CLR: ABOC drift error clear position */
#define XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR_ABOC_DRIFT_ERR_CLR_MSK  (0x01U << XENSIV_PAS_GAS_A2L_REG_SELF_TEST_CLR_ABOC_DRIFT_ERR_CLR_POS) /**< SELF_TEST_CLR: ABOC drift error clear mask */

// ---------------------------- ALARM_HYS_H ---------------------------- //
#define XENSIV_PAS_GAS_A2L_REG_ALARM_HYS_H_POS         (0x0U)                                                                               /**< ALARM_HYS_H: Hysteresis high position */
#define XENSIV_PAS_GAS_A2L_REG_ALARM_HYS_H_MASK        (0x7fU << XENSIV_PAS_GAS_A2L_REG_ALARM_HYS_H_POS)                                    /**< ALARM_HYS_H: Hysteresis high mask */

// ---------------------------- ALARM_HYS_L ---------------------------- //
#define XENSIV_PAS_GAS_A2L_REG_ALARM_HYS_L_POS         (0x0U)                                                                               /**< ALARM_HYS_L: Hysteresis low position */
#define XENSIV_PAS_GAS_A2L_REG_ALARM_HYS_L_MASK        (0xffU << XENSIV_PAS_GAS_A2L_REG_ALARM_HYS_L_POS)                                    /**< ALARM_HYS_L: Hysteresis low mask */

// ---------------------------- ABS_HUM_REF_H ---------------------------- //
#define XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_H_POS       (0x0U)                                                                               /**< ABS_HUM_REF_H: Absolute humidity high position */
#define XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_H_MASK      (0x03U << XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_H_POS)                                  /**< ABS_HUM_REF_H: Absolute humidity high mask */

// ---------------------------- ABS_HUM_REF_L ---------------------------- //
#define XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_L_POS       (0x0U)                                                                               /**< ABS_HUM_REF_L: Absolute humidity low position */
#define XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_L_MASK      (0xffU << XENSIV_PAS_GAS_A2L_REG_ABS_HUM_REF_L_POS)                                  /**< ABS_HUM_REF_L: Absolute humidity low mask */

// ---------------------------- HC_CTRL ---------------------------- //
#define XENSIV_PAS_GAS_A2L_REG_HC_CTRL_ENABLE_POS       (0x0U)                                                                              /**< HC_CTRL: Enable position */
#define XENSIV_PAS_GAS_A2L_REG_HC_CTRL_ENABLE_MASK      (0x0U << XENSIV_PAS_GAS_A2L_REG_HC_CTRL_ENABLE_POS)                                 /**< HC_CTRL: Enable mask */
#define XENSIV_PAS_GAS_A2L_REG_HC_CTRL_ERR_CLR_POS      (1U)                                                                                /**< HC_CTRL: Error clear position */
#define XENSIV_PAS_GAS_A2L_REG_HC_CTRL_ERR_CLR_MASK     (0x0U << XENSIV_PAS_GAS_A2L_REG_HC_CTRL_ERR_CLR_POS)                                /**< HC_CTRL: Error clear mask */
#define XENSIV_PAS_GAS_A2L_REG_HC_CTRL_HUM_ERR_POS      (2U)                                                                                /**< HC_CTRL: Humidity error position */
#define XENSIV_PAS_GAS_A2L_REG_HC_CTRL_HUM_ERR_MASK     (0x0U << XENSIV_PAS_GAS_A2L_REG_HC_CTRL_HUM_ERR_POS)                                /**< HC_CTRL: Humidity error mask */
#define XENSIV_PAS_GAS_A2L_REG_HC_CTRL_STALE_HUM_POS    (3U)                                                                                /**< HC_CTRL: Stale humidity position */
#define XENSIV_PAS_GAS_A2L_REG_HC_CTRL_STALE_HUM_MASK   (0x0U << XENSIV_PAS_GAS_A2L_REG_HC_CTRL_STALE_HUM_POS)                              /**< HC_CTRL: Stale humidity mask */
#define XENSIV_PAS_GAS_A2L_REG_HC_CTRL_ABS_HUM_POS      (4U)                                                                                /**< HC_CTRL: Absolute humidity position */
#define XENSIV_PAS_GAS_A2L_REG_HC_CTRL_ABS_HUM_MASK     (0x0U << XENSIV_PAS_GAS_A2L_REG_HC_CTRL_ABS_HUM_POS)                                /**< HC_CTRL: Absolute humidity mask */

/** \} group_board_libs */

#endif /* XENSIV_PAS_GAS_A2L_REGS_H_ */
