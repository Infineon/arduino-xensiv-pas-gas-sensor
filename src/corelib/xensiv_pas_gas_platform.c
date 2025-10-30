/***********************************************************************************************//**
 * \file xensiv_pas_gas_platform.c
 *
 * Description: Dummy implementations for XENSIV PAS GAS sensor platform functions.
 *              These functions do nothing and always return 0 (success).Implement these in the respective platforms.
 *  ***************************************************************************************************
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
 ***************************************************************************************************/
#include "xensiv_pas_gas_platform.h"

#ifndef __weak
#if defined(__GNUC__) || defined(__ARMCC_VERSION)
#define __weak __attribute__((weak))
#else
#define __weak
#endif
#endif

__weak int32_t xensiv_pas_gas_plat_i2c_transfer(void *ctx, uint16_t dev_addr, const uint8_t *tx_buffer, size_t tx_len, uint8_t *rx_buffer, size_t rx_len) {
    (void)ctx;
    (void)dev_addr;
    (void)tx_buffer;
    (void)tx_len;
    (void)rx_buffer;
    (void)rx_len;
    return 0;
}

__weak int32_t xensiv_pas_gas_plat_uart_read(void *ctx, uint8_t *data, size_t len) {
    (void)ctx;
    (void)data;
    (void)len;
    return 0;
}

__weak int32_t xensiv_pas_gas_plat_uart_write(void *ctx, uint8_t *data, size_t len) {
    (void)ctx;
    (void)data;
    (void)len;
    return 0;
}

__weak void xensiv_pas_gas_plat_delay(uint32_t ms) {
    (void)ms;
}

__weak uint16_t xensiv_pas_gas_plat_htons(uint16_t x) {
    (void)x;
    return 0;
}

__weak void xensiv_pas_gas_plat_assert(int expr) {
    (void)expr;
}
