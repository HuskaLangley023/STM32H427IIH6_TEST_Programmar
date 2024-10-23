//
// Created by zhu_y on 2024/10/7.
//
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "string.h"


extern uint8_t rxData[1];
extern uint8_t txData[1];

// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//     if(htim->Instance == TIM6) {
//         HAL_GPIO_TogglePin(LED_G_GPIO_Port,LED_G_Pin);
//     }
// }
//

//uart dma收发
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if(huart->Instance == UART7) {
        HAL_UART_Transmit_DMA(&huart7,rxData,1);
    }

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    memset(rxData, 0, sizeof(rxData));
    if(huart->Instance == UART7) {
        HAL_UART_Receive_DMA(&huart7,rxData,1);
    }
}