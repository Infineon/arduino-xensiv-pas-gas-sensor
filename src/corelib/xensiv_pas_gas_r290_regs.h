/***********************************************************************************************//**
 * \file xensiv_pas_gas_r290_regs.h
 *
 * Description: This file contains the register definitions
 *              for interacting with the XENSIV™ PAS R290 GAS sensor.
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

#ifndef XENSIV_PAS_GAS_R290_REGS_H_
#define XENSIV_PAS_GAS_R290_REGS_H_

#include "xensiv_pas_gas_regs.h"

/**
 * \addtogroup group_board_libs XENSIV™ PAS GAS R290 sensor
 * \{
 */

// --------------------------R290 Sensor Register Addresses---------------------------//
#define XENSIV_PAS_GAS_R290_REG_DEV_ID            (0x23U)                                                                           /**< DEV_ID: Device ID register address */
#define XENSIV_PAS_GAS_R290_REG_ABOC_PREFILL      (0x5DU)                                                                           /**< ABOC_PREFILL: ABOC prefill register address */
#define XENSIV_PAS_GAS_R290_REG_ALARM_CFG         (0x5FU)                                                                           /**< ALARM_CFG: Alarm configuration register address */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST         (0x60U)                                                                           /**< SELF_TEST: Self-test status register address */
#define XENSIV_PAS_GAS_R290_REG_DENOISE_CFG       (0x61U)                                                                           /**< DENOISE_CFG: Denoise configuration register address */
#define XENSIV_PAS_GAS_R290_REG_ABOC_CYCLE        (0x62U)                                                                           /**< ABOC_CYCLE: ABOC cycle register address */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR     (0x63U)                                                                           /**< SELF_TEST_CLR: Self-test clear register address */

// -------------------------------- DEV_ID ----------------------------------- //
#define XENSIV_PAS_GAS_R290_REG_DEV_ID_VAL_POS            (0U)                                                                      /**< DEV_ID: Value position */
#define XENSIV_PAS_GAS_R290_REG_DEV_ID_VAL_MSK            (0xffU << XENSIV_PAS_GAS_R290_REG_DEV_ID_VAL_POS)                         /**< DEV_ID: Value mask */

// -------------------------------- ABOC_PREFILL ----------------------------------- //
#define XENSIV_PAS_GAS_R290_REG_ABOC_PREFILL_VAL_POS      (0U)                                                                      /**< ABOC_PREFILL: Value position */
#define XENSIV_PAS_GAS_R290_REG_ABOC_PREFILL_VAL_MSK      (0xffU << XENSIV_PAS_GAS_R290_REG_ABOC_PREFILL_VAL_POS)                   /**< ABOC_PREFILL: Value mask */

// -------------------------------- ALARM_CFG ----------------------------------- //
#define XENSIV_PAS_GAS_R290_REG_ALARM_CFG_ALARM_POL_POS   (0U)                                                                      /**< ALARM_CFG: Alarm polarity position */
#define XENSIV_PAS_GAS_R290_REG_ALARM_CFG_ALARM_POL_MSK   (0x01U << XENSIV_PAS_GAS_R290_REG_ALARM_CFG_ALARM_POL_POS)                /**< ALARM_CFG: Alarm polarity mask */

// -------------------------------- SELF_TEST ----------------------------------- //
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_VOLTAGE_ERR_POS     (0U)                                                                  /**< SELF_TEST: Voltage error position */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_VOLTAGE_ERR_MSK     (0x01U << XENSIV_PAS_GAS_R290_REG_SELF_TEST_VOLTAGE_ERR_POS)          /**< SELF_TEST: Voltage error mask */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_TEMP_ERR_POS        (1U)                                                                  /**< SELF_TEST: Temperature error position */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_TEMP_ERR_MSK        (0x01U << XENSIV_PAS_GAS_R290_REG_SELF_TEST_TEMP_ERR_POS)             /**< SELF_TEST: Temperature error mask */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_SIMIC_ERR_POS       (2U)                                                                  /**< SELF_TEST: Simic error position */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_SIMIC_ERR_MSK       (0x01U << XENSIV_PAS_GAS_R290_REG_SELF_TEST_SIMIC_ERR_POS)            /**< SELF_TEST: Simic error mask */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_EMITTER_ERR_POS     (3U)                                                                  /**< SELF_TEST: Emitter error position */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_EMITTER_ERR_MSK     (0x01U << XENSIV_PAS_GAS_R290_REG_SELF_TEST_EMITTER_ERR_POS)          /**< SELF_TEST: Emitter error mask */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_ABOC_DRIFT_ERR_POS  (4U)                                                                  /**< SELF_TEST: ABOC drift error position */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_ABOC_DRIFT_ERR_MSK  (0x01U << XENSIV_PAS_GAS_R290_REG_SELF_TEST_ABOC_DRIFT_ERR_POS)       /**< SELF_TEST: ABOC drift error mask */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_LIFETIME_ERR_POS    (5U)                                                                  /**< SELF_TEST: Lifetime error position */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_LIFETIME_ERR_MSK    (0x01U << XENSIV_PAS_GAS_R290_REG_SELF_TEST_LIFETIME_ERR_POS)         /**< SELF_TEST: Lifetime error mask */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_REPLACE_SEN_POS     (7U)                                                                  /**< SELF_TEST: Replace sensor position */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_REPLACE_SEN_MSK     (0x01U << XENSIV_PAS_GAS_R290_REG_SELF_TEST_REPLACE_SEN_POS)          /**< SELF_TEST: Replace sensor mask */

// -------------------------------- DENOISE_CFG ----------------------------------- //
#define XENSIV_PAS_GAS_R290_REG_DENOISE_CFG_SMOOTHING_FACT_POS    (0U)                                                               /**< DENOISE_CFG: Smoothing factor position */
#define XENSIV_PAS_GAS_R290_REG_DENOISE_CFG_SMOOTHING_FACT_MSK    (0x3fU << XENSIV_PAS_GAS_R290_REG_DENOISE_CFG_SMOOTHING_FACT_POS)  /**< DENOISE_CFG: Smoothing factor mask */

// -------------------------------- ABOC_CYCLE ----------------------------------- //
#define XENSIV_PAS_GAS_R290_REG_ABOC_CYCLE_SMOOTHING_FACT_POS     (0U)                                                                /**< ABOC_CYCLE: Smoothing factor position */
#define XENSIV_PAS_GAS_R290_REG_ABOC_CYCLE_SMOOTHING_FACT_MSK     (0x3fU << XENSIV_PAS_GAS_R290_REG_ABOC_CYCLE_SMOOTHING_FACT_POS)    /**< ABOC_CYCLE: Smoothing factor mask */

// -------------------------------- SELF_TEST_CLR ----------------------------------- //
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR_VOLTAGE_ERR_CLR_POS     (0U)                                                                      /**< SELF_TEST_CLR: Voltage error clear position */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR_VOLTAGE_ERR_CLR_MSK     (0x01U << XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR_VOLTAGE_ERR_CLR_POS)      /**< SELF_TEST_CLR: Voltage error clear mask */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR_TEMP_ERR_CLR_POS        (1U)                                                                      /**< SELF_TEST_CLR: Temperature error clear position */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR_TEMP_ERR_CLR_MSK        (0x01U << XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR_TEMP_ERR_CLR_POS)         /**< SELF_TEST_CLR: Temperature error clear mask */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR_SIMIC_ERR_CLR_POS       (2U)                                                                      /**< SELF_TEST_CLR: Simic error clear position */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR_SIMIC_ERR_CLR_MSK       (0x01U << XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR_SIMIC_ERR_CLR_POS)        /**< SELF_TEST_CLR: Simic error clear mask */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR_EMITTER_ERR_CLR_POS     (3U)                                                                      /**< SELF_TEST_CLR: Emitter error clear position */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR_EMITTER_ERR_CLR_MSK     (0x01U << XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR_EMITTER_ERR_CLR_POS)      /**< SELF_TEST_CLR: Emitter error clear mask */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR_ABOC_DRIFT_ERR_CLR_POS  (4U)                                                                      /**< SELF_TEST_CLR: ABOC drift error clear position */
#define XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR_ABOC_DRIFT_ERR_CLR_MSK  (0x01U << XENSIV_PAS_GAS_R290_REG_SELF_TEST_CLR_ABOC_DRIFT_ERR_CLR_POS)   /**< SELF_TEST_CLR: ABOC drift error clear mask */

/** \} group_board_libs */

#endif /* XENSIV_PAS_GAS_R290_REGS_H_ */
