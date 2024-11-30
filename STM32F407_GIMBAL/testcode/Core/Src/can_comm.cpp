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

    Filterconfig.FilterBank = 14;
    HAL_CAN_ConfigFilter(&hcan2, &Filterconfig);
    HAL_CAN_Start(&hcan2);
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
}


void canTxMonitor(void) {
    // note: 每个通道一次只能发送3个包
    dji_motor_driver.canTxMsg(1, djimotor::ID_1_4);   // can1, id: 0x200
    osDelay(1);
}


void DJIMotorDriver::canTxMsg(uint8_t can_channel,
                              djimotor::CANIDRange_e id_range) {
    Motor** can_motor;
    if (can_channel == 1) {
        hcan_ = &hcan1;
        can_motor = can1_motor_;
    } else if (can_channel == 2) {
        hcan_ = &hcan2;
        can_motor = can2_motor_;
    } else {
        return;
    }

    // Pack transmit message.
    can_tx_header_.IDE = CAN_ID_STD;
    can_tx_header_.RTR = CAN_RTR_DATA;
    can_tx_header_.DLC = 8;
    if (id_range == djimotor::ID_1_4) {
        can_tx_header_.StdId = 0x200;
        motor_id_base_ = 0;
    } else if (id_range == djimotor::ID_5_8) {
        can_tx_header_.StdId = 0x1ff;
        motor_id_base_ = 4;
    } else if (id_range == djimotor::ID_9_11) {
        can_tx_header_.StdId = 0x2ff;
        motor_id_base_ = 8;
    }
    for (int i = 0; i < 4; i++) {
        if (can_motor[i + motor_id_base_] == nullptr) {
            can_tx_data_[2 * i] = 0;
            can_tx_data_[2 * i + 1] = 0;
        } else {
            can_tx_data_[2 * i] =
                (can_motor[i + motor_id_base_]->intensity_ >> 8);
            can_tx_data_[2 * i + 1] =
                (can_motor[i + motor_id_base_]->intensity_);
        }
    }