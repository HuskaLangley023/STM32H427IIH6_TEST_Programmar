//
// Created by zhu_y on 2024/11/13.
//
#include "control.h"
#include "iwdg.h"
#include "usart.h"
#include "motor.h"
#include "imu.h"

// uint8_t rx_buffer[18]; //uart通讯缓冲区
extern MotorManagement motors;
extern IMU bmi088;
int b = 0;

void mainloop() {
    HAL_IWDG_Refresh(&hiwdg);
    // HAL_UART_Receive_DMA(&huart1, rx_buffer, 18U);
b++;
    bmi088.receive();
    // motors.control();
}

void motorinit() {
    motors.motorsinit();
}

void imuinit() {
    bmi088.init();
}