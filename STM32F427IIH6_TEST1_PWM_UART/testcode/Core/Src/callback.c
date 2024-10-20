//
// Created by zhu_y on 2024/10/7.
//
#include "main.h"
#include "tim.h"
#include "usart.h"


extern uint8_t rxData[1];
extern uint8_t txData[1];

// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//     if(htim->Instance == TIM6) {
//         HAL_GPIO_TogglePin(LED_G_GPIO_Port,LED_G_Pin);
//     }
// }
//
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//     if(huart->Instance == UART7) {
//         HAL_GPIO_TogglePin(LED_G_GPIO_Port,LED_G_Pin);
//         HAL_UART_Transmit_IT(&huart7,rxData,1);
//     }
// }
//
// void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
//     if(huart->Instance == UART7) {
//         HAL_GPIO_TogglePin(LED_R_GPIO_Port,LED_R_Pin);
//         HAL_UART_Receive_IT(&huart7,rxData,1);
//     }
// }