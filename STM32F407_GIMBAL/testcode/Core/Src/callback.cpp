//
// Created by zhu_y on 2024/11/13.
//
#include "main.h"
#include "tim.h"
#include "control.h"
#include "can.h"

//定时器中断控制mainloop，周期1000Hz
//程序入口
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if(htim->Instance == htim6.Instance) {
        // HAL_GPIO_TogglePin(LED_B_GPIO_Port,LED_B_Pin); //测试代码
        mainloop();
    }
}

//接收机代码
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    //信息接收
    uint8_t rx_data[8];
    CAN_RxHeaderTypeDef rx_header;
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
    //信息处理
    canRxHandle(hcan, rx_header, rx_data);
}

