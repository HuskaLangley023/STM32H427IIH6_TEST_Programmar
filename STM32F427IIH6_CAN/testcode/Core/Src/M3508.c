//
// Created by zhu_y on 2024/10/24.
//
#include "main.h"
#include "M3508.h"


float linearMapping(uint16_t in, int in_min, int in_max, float out_min, float out_max) {
    float out,k,b;
    k=(out_max-out_min)/(float)(in_max-in_min);
    b=out_max-k*(float)in_max;
    out=k*(float)in+b;
    return out;
}

void solve_data(const uint8_t *rx_data, M3508_Motor* motor) {
    motor->ecd_angle_=linearMapping((uint16_t)rx_data[0]<<8|rx_data[1],0,8191,0,360);
    motor->delta_ecd_angle_=motor->ecd_angle_-motor->last_ecd_angle_;
    if(motor->delta_ecd_angle_>180) motor->delta_ecd_angle_-=360;
    if(motor->delta_ecd_angle_<-180) motor->delta_ecd_angle_+=360;
    motor->last_ecd_angle_=motor->ecd_angle_;

    // motor->delta_angle_=motor->delta_ecd_angle_/motor->ratio_;

    motor->rotate_speed_=(float)((uint16_t)rx_data[2]<<8|rx_data[3]);
    motor->current_=linearMapping((uint16_t)rx_data[4]<<8|rx_data[5],-16384,16384,-20,20);
    motor->temp_=(float)rx_data[6];

}

void motor_init(M3508_Motor* motor) {
  motor->angle_ = 0;
  motor->current_ = 0;
  motor->delta_angle_ = 0;
  motor->delta_ecd_angle_ = 0;
  motor->ecd_angle_ = 0;
  motor->last_ecd_angle_ = 0;
  motor->ratio_ = Motor_ratio_;
  motor->rotate_speed_ = 0;
  motor->temp_ = 0;
}

