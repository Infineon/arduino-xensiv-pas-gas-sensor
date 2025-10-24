/***********************************************************************************************//**
 * \file xensiv_pas_gas.c
 *
 * Description: This file contains the functions for interacting with the
 *              XENSIV™ PAS GAS sensor.
 *
 ***************************************************************************************************
 * \copyright
 * Copyright 2021-2022 Infineon Technologies AG
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

#include "xensiv_pas_gas.h"

#define XENSIV_PAS_GAS_COMM_DELAY_MS             (5U)
#define XENSIV_PAS_GAS_COMM_TEST_VAL             (0xA5U)

#define XENSIV_PAS_GAS_SOFT_RESET_DELAY_MS       (2000U)

#define XENSIV_PAS_GAS_I2C_WRITE_BUFFER_LEN      (17U)
#define XENSIV_PAS_GAS_UART_WRITE_XFER_BUF_SIZE  (8U)
#define XENSIV_PAS_GAS_UART_READ_XFER_BUF_SIZE   (5U)

#define XENSIV_PAS_GAS_UART_WRITE_XFER_RESP_LEN  (2U)
#define XENSIV_PAS_GAS_UART_READ_XFER_RESP_LEN   (3U)
#define XENSIV_PAS_GAS_UART_ACK                  (0x06U)
#define XENSIV_PAS_GAS_UART_NAK                  (0x15U)

static inline uint8_t xensiv_pas_gas_digit_to_ascii(uint8_t digit) {
    xensiv_pas_gas_plat_assert(digit <= 0xFU);

    if (digit < 10U) {
        return (uint8_t)(digit + 0x30U);
    } else {
        return (uint8_t)(digit + 0x37U);
    }
}

static inline uint8_t xensiv_pas_gas_ascii_to_digit(uint8_t ascii) {
    xensiv_pas_gas_plat_assert(((ascii >= (uint8_t)'0') && (ascii <= (uint8_t)'9')) || ((ascii >= (uint8_t)'A') && (ascii <= (uint8_t)'F')));

    if (ascii < (uint8_t)'A') {
        return (uint8_t)(ascii - (uint8_t)'0');
    } else {
        return (uint8_t)(10u + (uint8_t)(ascii - (uint8_t)'A'));
    }
}

static int32_t xensiv_pas_gas_i2c_read(const xensiv_pas_gas_t *dev, uint8_t reg_addr, uint8_t *data, uint8_t len) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(dev->ctx != NULL);
    xensiv_pas_gas_plat_assert(data != NULL);

    return xensiv_pas_gas_plat_i2c_transfer(dev->ctx, XENSIV_PAS_GAS_I2C_ADDR, &reg_addr, 1, data, len);
}

static int32_t xensiv_pas_gas_i2c_write(const xensiv_pas_gas_t *dev, uint8_t reg_addr, const uint8_t *data, uint8_t len) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(dev->ctx != NULL);
    xensiv_pas_gas_plat_assert(data != NULL);
    xensiv_pas_gas_plat_assert((len + 1U) < XENSIV_PAS_GAS_I2C_WRITE_BUFFER_LEN);

    uint8_t w_data[XENSIV_PAS_GAS_I2C_WRITE_BUFFER_LEN];
    w_data[0] = reg_addr;
    for (uint8_t i = 0; i < len; ++i)
    {
        w_data[i + 1U] = data[i];
    }

    uint16_t w_len = (uint16_t)((uint16_t)len + 1U);

    return xensiv_pas_gas_plat_i2c_transfer(dev->ctx, XENSIV_PAS_GAS_I2C_ADDR, w_data, w_len, NULL, 0);
}

static int32_t xensiv_pas_gas_uart_read(const xensiv_pas_gas_t *dev, uint8_t reg_addr, uint8_t *data, uint8_t len) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(dev->ctx != NULL);
    xensiv_pas_gas_plat_assert(reg_addr <= XENSIV_PAS_GAS_REG_SENS_RST);
    xensiv_pas_gas_plat_assert(data != NULL);

    int32_t res = XENSIV_PAS_GAS_OK;

    for (uint8_t i = 0; i < len; ++i)
    {
        uint8_t uart_buf[XENSIV_PAS_GAS_UART_READ_XFER_BUF_SIZE] =
        {
            (uint8_t)'r',
            (uint8_t)',',
            xensiv_pas_gas_digit_to_ascii((reg_addr & (uint8_t)0xF0) >> 4U),
            xensiv_pas_gas_digit_to_ascii(reg_addr & (uint8_t)0x0F),
            (uint8_t)'\n'
        };

        res = xensiv_pas_gas_plat_uart_write(dev->ctx, uart_buf, XENSIV_PAS_GAS_UART_READ_XFER_BUF_SIZE);

        if (XENSIV_PAS_GAS_OK == res) {
            res = xensiv_pas_gas_plat_uart_read(dev->ctx, uart_buf, XENSIV_PAS_GAS_UART_READ_XFER_RESP_LEN);
            if (XENSIV_PAS_GAS_OK == res) {
                data[i] = (uint8_t)((xensiv_pas_gas_ascii_to_digit(uart_buf[0]) << 4) + xensiv_pas_gas_ascii_to_digit(uart_buf[1]));
            }
        }

        if (XENSIV_PAS_GAS_OK != res) {
            break;
        }

        reg_addr++;
    }

    return res;
}

static int32_t xensiv_pas_gas_uart_write(const xensiv_pas_gas_t *dev, uint8_t reg_addr, const uint8_t *data, uint8_t len) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(dev->ctx != NULL);
    xensiv_pas_gas_plat_assert(reg_addr <= XENSIV_PAS_GAS_REG_SENS_RST);
    xensiv_pas_gas_plat_assert(data != NULL);

    int32_t res = XENSIV_PAS_GAS_OK;

    for (uint8_t i = 0; i < len; ++i)
    {
        uint8_t uart_buf[XENSIV_PAS_GAS_UART_WRITE_XFER_BUF_SIZE] =
        {
            (uint8_t)'w',
            (uint8_t)',',
            xensiv_pas_gas_digit_to_ascii((reg_addr & 0xF0U) >> 4U), xensiv_pas_gas_digit_to_ascii(reg_addr & 0x0FU),
            (uint8_t)',',
            xensiv_pas_gas_digit_to_ascii((data[i] & 0xF0U) >> 4U), xensiv_pas_gas_digit_to_ascii(data[i] & 0x0FU),
            (uint8_t)'\n'
        };

        res = xensiv_pas_gas_plat_uart_write(dev->ctx, uart_buf, XENSIV_PAS_GAS_UART_WRITE_XFER_BUF_SIZE);

        if (XENSIV_PAS_GAS_OK == res) {
            res = xensiv_pas_gas_plat_uart_read(dev->ctx, uart_buf, XENSIV_PAS_GAS_UART_WRITE_XFER_RESP_LEN);

            /* If command triggers a software reset ignores the sensor response */
            if ((XENSIV_PAS_GAS_REG_SENS_RST != reg_addr) || ((uint8_t)XENSIV_PAS_GAS_CMD_SOFT_RESET != data[i])) {
                res = (XENSIV_PAS_GAS_OK == res) ?
                    ((XENSIV_PAS_GAS_UART_ACK == uart_buf[0]) ? XENSIV_PAS_GAS_OK : XENSIV_PAS_GAS_ERR_COMM) :
                    XENSIV_PAS_GAS_ERR_COMM;
            } else {
                res = XENSIV_PAS_GAS_OK;
            }
        }

        if (XENSIV_PAS_GAS_OK != res) {
            break;
        }

        reg_addr++;
    }

    return res;
}

int32_t xensiv_pas_gas_base_init(xensiv_pas_gas_t *dev, xensiv_pas_gas_interface_t itf, void *ctx) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(ctx != NULL);

    dev->ctx = ctx;
    if (itf == XENSIV_PAS_GAS_INTERFACE_I2C) {
        dev->read = xensiv_pas_gas_i2c_read;
        dev->write = xensiv_pas_gas_i2c_write;
    } else if (itf == XENSIV_PAS_GAS_INTERFACE_UART) {
        dev->read = xensiv_pas_gas_uart_read;
        dev->write = xensiv_pas_gas_uart_write;
    }

    /* Check communication */
    uint8_t data = XENSIV_PAS_GAS_COMM_TEST_VAL;

    int32_t res = xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_SCRATCH_PAD, &data, 1U);

    if (XENSIV_PAS_GAS_OK == res) {
        res = xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_SCRATCH_PAD, &data, 1U);
    }

    if ((XENSIV_PAS_GAS_OK == res) && (XENSIV_PAS_GAS_COMM_TEST_VAL == data)) {
        /* Soft reset */
        res = xensiv_pas_gas_cmd(dev, XENSIV_PAS_GAS_CMD_SOFT_RESET);
        xensiv_pas_gas_plat_delay(XENSIV_PAS_GAS_SOFT_RESET_DELAY_MS);

        if (XENSIV_PAS_GAS_OK == res) {
            /* Read the sensor status and verify if the sensor is ready */
            res = xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_SENS_STS, &data, 1U);
        }

        if (XENSIV_PAS_GAS_OK == res) {
            if ((data & XENSIV_PAS_GAS_REG_SENS_STS_ICCER_MSK) != 0U) {
                res = XENSIV_PAS_GAS_ICCERR;
            } else if ((data & XENSIV_PAS_GAS_REG_SENS_STS_ORVS_MSK) != 0U) {
                res = XENSIV_PAS_GAS_ORVS;
            } else if ((data & XENSIV_PAS_GAS_REG_SENS_STS_ORTMP_MSK) != 0U) {
                res = XENSIV_PAS_GAS_ORTMP;
            } else if ((data & XENSIV_PAS_GAS_REG_SENS_STS_SEN_RDY_MSK) == 0U) {
                res = XENSIV_PAS_GAS_ERR_NOT_READY;
            } else {
                res = XENSIV_PAS_GAS_OK;
            }
        }
    } else {
        res = XENSIV_PAS_GAS_ERR_COMM;
    }

    return res;
}

int32_t xensiv_pas_gas_base_perform_forced_compensation(const xensiv_pas_gas_t *dev, uint16_t gas_ref) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    xensiv_pas_gas_measurement_config_t meas_config;
    int32_t res = xensiv_pas_gas_get_measurement_config(dev, &meas_config);

    if (XENSIV_PAS_GAS_OK == res) {
        meas_config.b.op_mode = XENSIV_PAS_GAS_OP_MODE_IDLE;
        res = xensiv_pas_gas_set_measurement_config(dev, meas_config);
    }

    if (XENSIV_PAS_GAS_OK == res) {
        res = xensiv_pas_gas_set_measurement_rate(dev, dev->fcs_meas_rate_s);
    }

    if (XENSIV_PAS_GAS_OK == res) {
        res = xensiv_pas_gas_set_offset_compensation(dev, gas_ref);
    }

    if (XENSIV_PAS_GAS_OK == res) {
        meas_config.b.op_mode = XENSIV_PAS_GAS_OP_MODE_CONTINUOUS;
        meas_config.b.boc_cfg = XENSIV_PAS_GAS_BOC_CFG_FORCED;
        res = xensiv_pas_gas_set_measurement_config(dev, meas_config);
    }

    if (XENSIV_PAS_GAS_OK == res) {
        /* wait until the FCS is finished */
        do
        {
            res = xensiv_pas_gas_get_measurement_config(dev, &meas_config);
        } while ((XENSIV_PAS_GAS_OK != res) || (XENSIV_PAS_GAS_BOC_CFG_FORCED == meas_config.b.boc_cfg));
    }

    if (XENSIV_PAS_GAS_OK == res) {
        meas_config.b.op_mode = XENSIV_PAS_GAS_OP_MODE_IDLE;
        res = xensiv_pas_gas_set_measurement_config(dev, meas_config);
    }

    return res;
}

int32_t xensiv_pas_gas_set_reg(const xensiv_pas_gas_t *dev, uint8_t reg_addr, const uint8_t *data, uint8_t len) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(data != NULL);

    int32_t res = dev->write(dev, reg_addr, data, len);
    xensiv_pas_gas_plat_delay(XENSIV_PAS_GAS_COMM_DELAY_MS);

    return res;
}

int32_t xensiv_pas_gas_get_reg(const xensiv_pas_gas_t *dev, uint8_t reg_addr, uint8_t *data, uint8_t len) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(data != NULL);

    int32_t res = dev->read(dev, reg_addr, data, len);
    xensiv_pas_gas_plat_delay(XENSIV_PAS_GAS_COMM_DELAY_MS);

    return res;
}

int32_t xensiv_pas_gas_get_id(const xensiv_pas_gas_t *dev, xensiv_pas_gas_id_t *id) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(id != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_PROD_ID, &(id->u), 1U);
}

int32_t xensiv_pas_gas_get_status(const xensiv_pas_gas_t *dev, xensiv_pas_gas_status_t *status) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(status != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_SENS_STS, &(status->u), 1U);
}

int32_t xensiv_pas_gas_clear_status(const xensiv_pas_gas_t *dev, uint8_t mask) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_SENS_STS, &mask, 1U);
}

int32_t xensiv_pas_gas_get_interrupt_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_interrupt_config_t *int_config) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(int_config != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_INT_CFG, &(int_config->u), 1U);
}

int32_t xensiv_pas_gas_set_interrupt_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_interrupt_config_t int_config) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_INT_CFG, &(int_config.u), 1U);
}

int32_t xensiv_pas_gas_get_measurement_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_measurement_config_t *meas_config) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(meas_config != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_MEAS_CFG, &(meas_config->u), 1U);
}

int32_t xensiv_pas_gas_set_measurement_config(const xensiv_pas_gas_t *dev, xensiv_pas_gas_measurement_config_t meas_config) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_MEAS_CFG, &(meas_config.u), 1U);
}

int32_t xensiv_pas_gas_get_result(const xensiv_pas_gas_t *dev, uint16_t *val) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(val != NULL);

    xensiv_pas_gas_meas_status_t meas_status;
    int32_t res = xensiv_pas_gas_get_measurement_status(dev, &meas_status);

    if (XENSIV_PAS_GAS_OK == res) {
        if (meas_status.b.drdy != 0U) {
            res = xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_GASCONC_H, (uint8_t *)val, 2U);
            *val = xensiv_pas_gas_plat_htons(*val);
        } else {
            res = XENSIV_PAS_GAS_READ_NRDY;
        }
    }

    return res;
}

int32_t xensiv_pas_gas_set_measurement_rate(const xensiv_pas_gas_t *dev, uint16_t val) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert((val >= dev->meas_rate_min) && (val <= XENSIV_PAS_GAS_MEAS_RATE_MAX));

    val = xensiv_pas_gas_plat_htons(val);
    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_MEAS_RATE_H, (uint8_t *)&val, 2U);
}

int32_t xensiv_pas_gas_get_measurement_status(const xensiv_pas_gas_t *dev, xensiv_pas_gas_meas_status_t *status) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(status != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_MEAS_STS, &(status->u), 1U);
}

int32_t xensiv_pas_gas_clear_measurement_status(const xensiv_pas_gas_t *dev, uint8_t mask) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_MEAS_STS, &mask, 1U);
}

int32_t xensiv_pas_gas_set_alarm_threshold(const xensiv_pas_gas_t *dev, uint16_t val) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    val = (uint16_t)xensiv_pas_gas_plat_htons(val);
    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_ALARM_TH_H, (uint8_t *)&val, 2U);
}

int32_t xensiv_pas_gas_set_pressure_compensation(const xensiv_pas_gas_t *dev, uint16_t val) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    val = (uint16_t)xensiv_pas_gas_plat_htons(val);
    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_PRESS_REF_H, (uint8_t *)&val, 2U);
}

int32_t xensiv_pas_gas_set_offset_compensation(const xensiv_pas_gas_t *dev, uint16_t val) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    val = (uint16_t)xensiv_pas_gas_plat_htons(val);
    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_CALIB_REF_H, (uint8_t *)&val, 2U);
}

int32_t xensiv_pas_gas_set_scratch_pad(const xensiv_pas_gas_t *dev, uint8_t val) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_SCRATCH_PAD, &val, 1U);
}

int32_t xensiv_pas_gas_get_scratch_pad(const xensiv_pas_gas_t *dev, uint8_t *val) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert(val != NULL);

    return xensiv_pas_gas_get_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_SCRATCH_PAD, val, 1U);
}

int32_t xensiv_pas_gas_cmd(const xensiv_pas_gas_t *dev, xensiv_pas_gas_cmd_t cmd) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_SENS_RST, (const uint8_t *)&cmd, 1U);
}

int32_t xensiv_pas_gas_start_single_mode(const xensiv_pas_gas_t *dev) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    xensiv_pas_gas_measurement_config_t meas_config;
    int32_t res = xensiv_pas_gas_get_measurement_config(dev, &meas_config);

    if (XENSIV_PAS_GAS_OK == res) {
        if (meas_config.b.op_mode != XENSIV_PAS_GAS_OP_MODE_IDLE) {
            meas_config.b.op_mode = XENSIV_PAS_GAS_OP_MODE_IDLE;
            res = xensiv_pas_gas_set_measurement_config(dev, meas_config);
        }
    }

    if (XENSIV_PAS_GAS_OK == res) {
        meas_config.b.op_mode = XENSIV_PAS_GAS_OP_MODE_SINGLE;
        meas_config.b.boc_cfg = XENSIV_PAS_GAS_BOC_CFG_AUTOMATIC;
        res = xensiv_pas_gas_set_measurement_config(dev, meas_config);
    }

    return res;
}

int32_t xensiv_pas_gas_start_continuous_mode(const xensiv_pas_gas_t *dev, uint16_t val) {
    xensiv_pas_gas_plat_assert(dev != NULL);
    xensiv_pas_gas_plat_assert((val >= dev->meas_rate_min) && (val <= XENSIV_PAS_GAS_MEAS_RATE_MAX));

    xensiv_pas_gas_measurement_config_t meas_config;
    int32_t res = xensiv_pas_gas_get_measurement_config(dev, &meas_config);

    if (XENSIV_PAS_GAS_OK == res) {
        if (meas_config.b.op_mode != XENSIV_PAS_GAS_OP_MODE_IDLE) {
            meas_config.b.op_mode = XENSIV_PAS_GAS_OP_MODE_IDLE;
            res = xensiv_pas_gas_set_measurement_config(dev, meas_config);
        }
    }

    if (XENSIV_PAS_GAS_OK == res) {
        val = xensiv_pas_gas_plat_htons(val);
        res = xensiv_pas_gas_set_reg(dev, (uint8_t)XENSIV_PAS_GAS_REG_MEAS_RATE_H, (uint8_t *)&val, 2U);
    }

    if (XENSIV_PAS_GAS_OK == res) {
        meas_config.b.op_mode = XENSIV_PAS_GAS_OP_MODE_CONTINUOUS;
        meas_config.b.boc_cfg = XENSIV_PAS_GAS_BOC_CFG_AUTOMATIC;
        res = xensiv_pas_gas_set_measurement_config(dev, meas_config);
    }

    return res;
}

int32_t xensiv_pas_gas_perform_forced_compensation(const xensiv_pas_gas_t *dev, uint16_t gas_ref) {
    xensiv_pas_gas_plat_assert(dev != NULL);

    return dev->force_comp(dev, gas_ref);
}
