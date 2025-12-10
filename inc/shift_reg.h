/**
 ******************************************************************************
 * @file    shift_reg.h
 * @author  Aki
 * @date    2025-12-10
 * @version V1.0.0
 * @brief   移位寄存器驱动
 ******************************************************************************
 */

#ifndef SHIFT_REG_H
#define SHIFT_REG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

// 延时计数：H7/G4 等高频 MCU 建议设为 10~20，F1/F4 设为 1 或 2 即可
#ifndef SHIFT_DELAY_CNT
#define SHIFT_DELAY_CNT 10
#endif

#define SHIFT_DELAY()                          \
    do                                         \
    {                                          \
        volatile uint32_t i = SHIFT_DELAY_CNT; \
        while (i--)                            \
            __NOP();                           \
    } while (0)

    typedef struct
    {
        GPIO_TypeDef *ds_port;
        uint16_t ds_pin;
        GPIO_TypeDef *shcp_port;
        uint16_t shcp_pin;
        GPIO_TypeDef *stcp_port;
        uint16_t stcp_pin;
    } hc595_t;

    typedef struct
    {
        GPIO_TypeDef *miso_port;
        uint16_t miso_pin;
        GPIO_TypeDef *sck_port;
        uint16_t sck_pin;
        GPIO_TypeDef *pl_port;
        uint16_t pl_pin;
    } hc165_t;

    // 595 API
    void hc595_init(hc595_t *dev);
    void hc595_write_byte(hc595_t *dev, uint8_t data);
    void hc595_write_buf(hc595_t *dev, const uint8_t *buf, uint16_t len);

    // 165 API
    void hc165_init(hc165_t *dev);
    uint8_t hc165_read_byte(hc165_t *dev);
    void hc165_read_buf(hc165_t *dev, uint8_t *buf, uint16_t len);

#ifdef __cplusplus
}
#endif
#endif
