//
// Created by zhu_y on 2024/11/27.
//
#include "remote.h"

RC rc;

RC::RC() {
    current_data_.s1 = Botton_Off;
    current_data_.s2 = Botton_Off;
    // raw_data_ = {0, 0, 0, 0, 0, 0};
}

void RC::handle(uint8_t rx_buffer[18]) {
    //读取数据
    raw_data_.ch0 = ((int16_t) rx_buffer[0] | ((int16_t) rx_buffer[1] << 8)) & 0x07FF;
    raw_data_.ch1 = (((int16_t) rx_buffer[1] >> 3) | ((int16_t) rx_buffer[2] << 5)) & 0x07FF;
    raw_data_.ch2 = (((int16_t) rx_buffer[2] >> 6) | ((int16_t) rx_buffer[3] << 2) | ((int16_t) rx_buffer[4] << 10)) &
                    0x07FF;
    raw_data_.ch3 = (((int16_t) rx_buffer[4] >> 1) | ((int16_t) rx_buffer[5] << 7)) & 0x07FF;
    raw_data_.s1 = ((rx_buffer[5] >> 4) & 0x000C) >> 2;
    raw_data_.s2 = ((rx_buffer[5] >> 4) & 0x0003);
    //数据处理
    current_data_.r_row = control_data_[0] = RCChannel_mapping(raw_data_.ch0);
    current_data_.r_col = control_data_[1] = RCChannel_mapping(raw_data_.ch1);
    current_data_.l_row = control_data_[2] = RCChannel_mapping(raw_data_.ch2); //Yaw
    current_data_.l_col = control_data_[3] = RCChannel_mapping(raw_data_.ch3); //Pitch
    if (raw_data_.s1 == Botton_H) {
        current_data_.s1 = Botton_H;
        control_data_[4] = Botton_H;
    } else if (raw_data_.s1 == Botton_L) {
        current_data_.s1 = Botton_L;
        control_data_[4] = Botton_L;
    } else if (raw_data_.s1 == Botton_Off) {
        current_data_.s1 = Botton_Off;
        control_data_[4] = Botton_Off;
    }
    if (raw_data_.s2 == Botton_H) {
        current_data_.s2 = Botton_H;
        control_data_[5] = Botton_H;
    } else if (raw_data_.s2 == Botton_L) {
        current_data_.s2 = Botton_L;
        control_data_[5] = Botton_L;
    } else if (raw_data_.s2 == Botton_Off) {
        current_data_.s2 = Botton_Off;
        control_data_[5] = Botton_Off;
    }
}

void uartRxHandle(uint8_t rx_buffer[18]) {
    rc.handle(rx_buffer);
}
