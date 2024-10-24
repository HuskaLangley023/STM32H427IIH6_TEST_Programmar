//
// Created by zhu_y on 2024/10/15.
//
#include "main.h"
#include "M3508.h"


extern uint8_t rx_buffer[8];
extern uint8_t tx_buffer[8];
extern M3508_Motor motor;
CAN_RxHeaderTypeDef rx_header;


int i = 0;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if(hcan == &hcan1) {
        //接受电机数据代码
        HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_buffer);
        solve_data(rx_buffer, &motor);
    }


    //下为can接收测试
    // if(rx_buffer[5] == tx_buffer[5]) {
    //     HAL_GPIO_TogglePin(LED_G_GPIO_Port,LED_G_Pin);
    // }
}


