//
// Created by zhu_y on 2024/10/15.
//
#include <main.h>


extern uint8_t rx_buffer[8];
extern uint8_t tx_buffer[8];
extern M3508_Motor motor;
CAN_RxHeaderTypeDef rx_header;




void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if(hcan == &hcan1) {

        HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_buffer);
        // motor.last_ecd_angle_=motor.ecd_angle_;
        // motor.ecd_angle_=linearMapping((uint16_t)rx_buffer[0]<<8|rx_buffer[1],0,8191,0,360);
        // motor.rotate_speed_=(float)((uint16_t)rx_buffer[2]<<8|rx_buffer[3]);
        // motor.current_=linearMapping((uint16_t)rx_buffer[4]<<8|rx_buffer[5],-16384,16384,-20,20);
        // motor.temp_=(float)rx_buffer[6];
        // motor.delta_ecd_angle_=(float)motor.ecd_angle_-motor.last_ecd_angle_;
        // if(motor.delta_ecd_angle_>180) motor.delta_ecd_angle_-=360;
        // else if(motor.delta_ecd_angle_<-180) motor.delta_ecd_angle_+=360;
        //
        //
        // motor.delta_angle_=motor.delta_ecd_angle_/motor.ratio_;
        // motor.angle_+=motor.delta_angle_;
    }


    //下为can接收测试
    // if(rx_buffer[5] == tx_buffer[5]) {
    //     HAL_GPIO_TogglePin(LED_G_GPIO_Port,LED_G_Pin);
    // }
}


float linearMapping(uint16_t in, int in_min, int in_max, float out_min, float out_max) {
    float out,k,b;
    k=(out_max-out_min)/(float)(in_max-in_min);
    b=out_max-(float)k*in_max;
    out=(float)k*in+b;
    return out;
}

void solve_data(uint8_t *rx_data, M3508_Motor motor) {

    motor.ecd_angle_=linearMapping((uint16_t)rx_data[0]<<8|rx_data[1],0,8191,0,360);
    motor.rotate_speed_=(float)((uint16_t)rx_data[2]<<8|rx_data[3]);
    motor.current_=linearMapping((uint16_t)rx_data[4]<<8|rx_data[5],-16384,16384,-20,20);
    motor.temp_=(float)rx_data[6];
    motor.delta_ecd_angle_=(float)motor.ecd_angle_-motor.last_ecd_angle_;
    if(motor.delta_ecd_angle_>180) motor.delta_ecd_angle_-=360;
    else if(motor.delta_ecd_angle_<-180) motor.delta_ecd_angle_+=360;
    motor.last_ecd_angle_=motor.ecd_angle_;

    motor.delta_angle_=motor.delta_ecd_angle_/motor.ratio_;
    motor.angle_++;
    // =motor.delta_angle_;
}