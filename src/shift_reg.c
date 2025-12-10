/**
 ******************************************************************************
 * @file    shift_reg.c
 * @author  Aki
 * @date    2025-12-10
 * @version V1.0.0
 * @brief   移位寄存器驱动
 ******************************************************************************
 */

#include "shift_reg.h"

#define SET_PIN(p, n) HAL_GPIO_WritePin(p, n, GPIO_PIN_SET)
#define CLR_PIN(p, n) HAL_GPIO_WritePin(p, n, GPIO_PIN_RESET)
#define READ_PIN(p, n) HAL_GPIO_ReadPin(p, n)

// ---------------- 74HC595 ----------------

void hc595_init(hc595_t *dev)
{
    if (!dev)
        return;
    CLR_PIN(dev->shcp_port, dev->shcp_pin);
    CLR_PIN(dev->stcp_port, dev->stcp_pin);
    CLR_PIN(dev->ds_port, dev->ds_pin);
}

// 锁存脉冲
static void _595_latch(hc595_t *dev)
{
    SET_PIN(dev->stcp_port, dev->stcp_pin);
    SHIFT_DELAY();
    CLR_PIN(dev->stcp_port, dev->stcp_pin);
}

static void _595_send(hc595_t *dev, uint8_t val)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (val & 0x80)
            SET_PIN(dev->ds_port, dev->ds_pin);
        else
            CLR_PIN(dev->ds_port, dev->ds_pin);
        SHIFT_DELAY();

        // SCK 上升沿
        SET_PIN(dev->shcp_port, dev->shcp_pin);
        SHIFT_DELAY();
        CLR_PIN(dev->shcp_port, dev->shcp_pin);

        val <<= 1;
    }
}

void hc595_write_byte(hc595_t *dev, uint8_t data)
{
    if (!dev)
        return;
    _595_send(dev, data);
    _595_latch(dev);
}

void hc595_write_buf(hc595_t *dev, const uint8_t *buf, uint16_t len)
{
    if (!dev || !buf)
        return;
    for (uint16_t i = 0; i < len; i++)
    {
        _595_send(dev, buf[i]);
    }
    _595_latch(dev);
}

// ---------------- 74HC165 ----------------

void hc165_init(hc165_t *dev)
{
    if (!dev)
        return;
    SET_PIN(dev->pl_port, dev->pl_pin); // PL常高，允许移位
    CLR_PIN(dev->sck_port, dev->sck_pin);
}

static uint8_t _165_read(hc165_t *dev)
{
    uint8_t data = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        data <<= 1;
        if (READ_PIN(dev->miso_port, dev->miso_pin))
        {
            data |= 0x01;
        }

        // SCK 移出下一位
        SET_PIN(dev->sck_port, dev->sck_pin);
        SHIFT_DELAY();
        CLR_PIN(dev->sck_port, dev->sck_pin);
        SHIFT_DELAY();
    }
    return data;
}

// 加载并行数据
static void _165_load(hc165_t *dev)
{
    CLR_PIN(dev->pl_port, dev->pl_pin);
    SHIFT_DELAY();
    SET_PIN(dev->pl_port, dev->pl_pin);
    SHIFT_DELAY();
}

uint8_t hc165_read_byte(hc165_t *dev)
{
    if (!dev)
        return 0;
    _165_load(dev);
    return _165_read(dev);
}

void hc165_read_buf(hc165_t *dev, uint8_t *buf, uint16_t len)
{
    if (!dev || !buf)
        return;
    _165_load(dev);
    for (uint16_t i = 0; i < len; i++)
    {
        buf[i] = _165_read(dev);
    }
}
