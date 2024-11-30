//
// Created by zhu_y on 2024/11/13.
//
#ifndef MOTOR_H
#define MOTOR_H
#include "main.h"
#include "PID.h"

class Motor {
public:
    enum type {
        M3508,
        M2006,
        M6020,
    } type_m;

    float ratio_; //减速比
    struct Infomation {
        uint8_t id;
    }info;

    typedef struct {
        float target_angle_;
        float target_speed_;
        float fdb_angle_;
        float last_fdb_angle_;
    } ControlData;

    ControlData control_data_;

    typedef struct {
        //输出角度
        float motor_angle_;
        float delta_angle_;
        //编码器角度
        float ecd_angle_;
        float last_ecd_angle_;
        float delta_ecd_angle_;
        float motor_speed_; //dps

        float current_;
        float temp_;
    } CurrentData;

    CurrentData current_data_;

    PID pid_;

    Motor(const type &type, const float &ratio, const PID &pid);

    void handle();
};


class MotorManagement {
public:
    Motor *motors[4];
    MotorManagement(Motor *motors_[4]);
    void configId();
    void canMotorsCallback(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef rx_header, uint8_t rx_data[8]);

};

void canRxHandle(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef rx_header, uint8_t rx_data[8]);


#endif //MOTOR_H
