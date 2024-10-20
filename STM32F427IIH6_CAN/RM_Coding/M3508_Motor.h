//
// Created by zhu_y on 24-9-19.
//

#ifndef M3508_MOTOR_H
#define M3508_MOTOR_H
#include<cstdint>
#include<iostream>
#include<cmath>
using namespace std;

class M3508_Motor {
private:
    float ratio_;  // 电机减速比
    float angle_;  // deg 输出端累计转动角度
    float delta_angle_;  // deg 输出端新转动的角度
    float ecd_angle_;  // deg 当前电机编码器角度
    float last_ecd_angle_;  // deg 上次电机编码器角度
    float delta_ecd_angle_; // deg 编码器端新转动的角度
    float rotate_speed_;// dps 反馈转子转速
    float current_;  // A   反馈转矩电流
    float temp_;  // °C  反馈电机温度

    uint16_t uint16_ecd_angle_=0; //Task6新增变量，记录映射前转子角度值
    uint16_t uint16_last_ecd_angle_=0;  //Task6新增变量，记录映射前上次转子角度值
    uint16_t uint16_delta_ecd_angle_=0;  //Task6新增变量，记录映射前转子角度变化值
    uint16_t uint16_sum_ecd_angle_=0; //Task6新增变量，此非映射前转子角状态，而是总转动累积量
    uint16_t uint16_abs_sum_delta_ecd_angle_=0;  //Task6新增变量，记录映射前转子总转角的绝对值求和
    float abs_sum_delta_angle_=0;  //Task6新增变量，记录转子总转角的绝对值求和
    float error_angle=0;  //Task6新增变量，记录总误差值
    float error_time=0;  //Task6新增变量，记录总误差值所需时间
public:
    M3508_Motor();
    void canRxMsgCallback_v1(uint8_t rx_data[8]);
    void canRxMsgCallback_v2(uint8_t rx_data[8]);
    void canRxMsgCallback_v3(uint8_t rx_data[8]);
    void cal_error_time(uint8_t rx_data[8]);
    float estimate_error_time(float rotate_speed_);
    void canRxMsgCallback_v4(uint8_t rx_data[8]);
    float linearMapping(int, int, int, float, float);
};




#endif //M3508_MOTOR_H
