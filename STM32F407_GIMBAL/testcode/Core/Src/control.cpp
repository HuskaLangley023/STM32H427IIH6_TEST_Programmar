//
// Created by zhu_y on 2024/11/13.
//
#include "control.h"
#include "iwdg.h"
#include "usart.h"

uint8_t rx_buffer[18]; //uart通讯缓冲区
extern MotorManagement motors;
uint32_t a = 0;

void mainloop() {
    HAL_IWDG_Refresh(&hiwdg);
    // HAL_UART_Receive_DMA(&huart1, rx_buffer, 18U);

    motors.control();
    a++;


}