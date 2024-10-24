//
// Created by zhu_y on 2024/10/15.
//
#include "main.h"


extern uint8_t rx_buffer[8];
extern uint8_t tx_buffer[8];
CAN_RxHeaderTypeDef rx_header;

CAN_TxHeaderTypeDef tx_header = {
    .StdId = 0x200,
    .ExtId = 0x01,
    .IDE = CAN_ID_STD,
    .RTR = CAN_RTR_DATA,
    .DLC = 8,
    .TransmitGlobalTime = DISABLE
    };
uint32_t tx_mailbox;
extern uint8_t tx_buffer[];


//发送机按键中断触发代码
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if(GPIO_Pin == Key_Pin) {
        HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_buffer, &tx_mailbox);
    }
}

//接收机代码
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if(hcan == &hcan1) {
        HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_buffer);
        //下为can接收测试
        uint8_t cnt = 0;
        for(uint8_t i = 0; i<8; i++) {
            if(rx_buffer[i] == tx_buffer[i]) cnt++;
        }
        if(cnt == 8) HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
    }
}


