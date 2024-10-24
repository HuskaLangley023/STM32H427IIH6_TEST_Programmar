//
// Created by zhu_y on 2024/10/24.
//

#ifndef M3508_H
#define M3508_H

extern float Motor_ratio_;

typedef struct{

    float ratio_;
    float angle_;
    float delta_angle_;
    float ecd_angle_;
    float last_ecd_angle_;
    float delta_ecd_angle_;
    float rotate_speed_;
    float current_;
    float temp_;

    // uint16_t uint16_ecd_angle_;
    // uint16_t uint16_last_ecd_angle_;
    // uint16_t uint16_delta_ecd_angle_;
    // uint16_t uint16_sum_ecd_angle_;
    // uint16_t uint16_abs_sum_delta_ecd_angle_;
    // float abs_sum_delta_angle_;
    // float error_angle;
    // float error_time;

}M3508_Motor;

float linearMapping(uint16_t in, int in_min, int in_max, float out_min, float out_max);
void solve_data(const uint8_t *rx_data, M3508_Motor* motor);
void motor_init(M3508_Motor* motor);

#endif //M3508_H
