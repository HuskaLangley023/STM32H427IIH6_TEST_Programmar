//
// Created by zhu_y on 2024/11/20.
//
#include "can_comm.h"


extern uint8_t rx_buffer[8];
extern uint8_t tx_buffer[8];
CAN_RxHeaderTypeDef rx_header;


void canInit() {
    CAN_FilterTypeDef Filterconfig = {
        0x0000,
        0x0000,
        0x0000,
        0x0000,
        CAN_FilterFIFO0,
        0,
        CAN_FILTERMODE_IDMASK,
        CAN_FILTERSCALE_32BIT,
        ENABLE,
        0
      };

    HAL_CAN_ConfigFilter(&hcan1, &Filterconfig);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

    // Filterconfig.FilterBank = 14;
    // HAL_CAN_ConfigFilter(&hcan2, &Filterconfig);
    // HAL_CAN_Start(&hcan2);
    // HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
}



