# shift-reg-driver

基于STM32 HAL库的74HC595（输出）和74HC165（输入）移位寄存器驱动程序

## 快速开始

### 1. 添加文件到工程

将 `inc/` 和 `src/` 文件夹内容复制到你的STM32工程

### 2. 配置CubeMX

在CubeMX中配置GPIO引脚，并设置User Label（如 `HC595_DS`）

### 3. 代码调用

**初始化：**

```c
#include "shift_reg.h"

hc595_t hc595;
hc165_t hc165;

void bsp_shift_reg_init(void)
{
    // 配置595
    hc595.ds_port = HC595_DS_GPIO_Port;
    hc595.ds_pin = HC595_DS_Pin;
    hc595.shcp_port = HC595_SHCP_GPIO_Port;
    hc595.shcp_pin = HC595_SHCP_Pin;
    hc595.stcp_port = HC595_STCP_GPIO_Port;
    hc595.stcp_pin = HC595_STCP_Pin;
    hc595_init(&hc595);

    // 配置165
    hc165.miso_port = HC165_MISO_GPIO_Port;
    hc165.miso_pin = HC165_MISO_Pin;
    hc165.sck_port = HC165_SCK_GPIO_Port;
    hc165.sck_pin = HC165_SCK_Pin;
    hc165.pl_port = HC165_PL_GPIO_Port;
    hc165.pl_pin = HC165_PL_Pin;
    hc165_init(&hc165);
}

bsp_shift_reg_init();
```

**读写：**

```c
// 写595
hc595_write_byte(&hc595, 0x01);

// 读165
uint8_t key = hc165_read_byte(&hc165);

// 级联写法
uint8_t tx[2] = {0xAA, 0x55};
hc595_write_buf(&hc595, tx, 2);

uint8_t rx[2];
hc165_read_buf(&hc165, rx, 2);
```

## 常见问题

### 74HC595 输出不稳定

检查 `SHIFT_DELAY_COUNT` 是否足够（高速MCU需要增大）

## 许可证

MIT许可证