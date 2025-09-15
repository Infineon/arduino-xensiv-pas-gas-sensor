/***********************************************************************************************//**
 * \file xensiv_pas_gas_platform.c
 *
 * Description: Dummy implementations for XENSIV PAS GAS sensor platform functions.
 *              These functions do nothing and always return 0 (success).Implement these in the respective platforms.
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
