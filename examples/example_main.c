#include "main.h"
#include "shift_reg.h"

// 定义对象
hc595_t hc595;
hc165_t hc165;

// 初始化
void bsp_shift_reg_init(void)
{
    // 配置 595
    hc595.ds_port = HC595_DS_GPIO_Port;
    hc595.ds_pin = HC595_DS_Pin;
    hc595.shcp_port = HC595_SHCP_GPIO_Port;
    hc595.shcp_pin = HC595_SHCP_Pin;
    hc595.stcp_port = HC595_STCP_GPIO_Port;
    hc595.stcp_pin = HC595_STCP_Pin;
    hc595_init(&hc595);

    // 配置 165
    hc165.miso_port = HC165_MISO_GPIO_Port;
    hc165.miso_pin = HC165_MISO_Pin;
    hc165.sck_port = HC165_SCK_GPIO_Port;
    hc165.sck_pin = HC165_SCK_Pin;
    hc165.pl_port = HC165_PL_GPIO_Port;
    hc165.pl_pin = HC165_PL_Pin;
    hc165_init(&hc165);
}

// 测试任务
void app_test_loop(void)
{
    static uint8_t led = 0x01;

    // 写跑马灯
    hc595_write_byte(&hc595, led);
    led = (led == 0x80) ? 0x01 : (led << 1);

    // 读拨码开关
    uint8_t key = hc165_read_byte(&hc165);

    // 如果 Bit0 按下 (低电平)
    if (!(key & 0x01))
    {
        // TODO
    }

    /*
    // 级联测试 (若各接了2片)
    uint8_t tx[2] = {0xAA, 0x55};
    hc595_write_buf(&hc595, tx, 2);

    uint8_t rx[2];
    hc165_read_buf(&hc165, rx, 2);
    */

    HAL_Delay(100);
}
