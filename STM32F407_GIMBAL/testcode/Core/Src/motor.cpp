//
// Created by zhu_y on 2024/11/13.
//
#include "motor.h"
#include "math.h"
#include "string.h"

PID ppid_ = {

}


Motor::Motor(const type &type, const float &ratio, const PID &pid)
    : type_m(type),
      ratio_(ratio),
      pid_(pid) {
}

Motor GMY(Motor::M6020, 1, PID(25, 0.1, 10, 10, 1800)); //Yaw轴电机
Motor GMP(Motor::M6020, 1, PID(25, 0.1, 10, 10, 1800)); //Pitch轴电机
Motor GMFL(Motor::M3508, 3591.f / 87.f, PID(20, 0.1, 10, 10, 2500)); //左摩擦轮电机
Motor GMFR(Motor::M3508, 3591.f / 87.f, PID(20, 0.1, 10, 10, 2500)); //右摩擦轮电机

// Motor Testmotor(Motor::M6020, 1, PID(1,1,1,1,1));//test


void Motor::handle() {
    // control_data_.target_angle_ = //设置目标角度，或许与前馈有关？
}


//MotorMangement

Motor *motors_in_use[4] = {
    &GMY,
    &GMP,
    &GMFL,
    &GMFR
};

MotorManagement::MotorManagement(Motor *motors_[4]) {
    memcpy(motors, motors_, 4 * sizeof(Motor *));
    configId();
}

//电机组各电机信息配置
void MotorManagement::configId() {
    for (int i = 0; i < 4; i++) {
        if (motors[i] != nullptr) {
            motors[i]->info.id = i + 1;
        }
    }
}


MotorManagement motors(motors_in_use);

//MotorManagement成员函数，在canRxHandle()中调用，用于处理电机组数据。
void MotorManagement::canMotorsCallback(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef rx_header, uint8_t rx_data[8]) {
    if (rx_header.StdId < 0x201 || rx_header.StdId > 0x208) return;
    int16_t ecd;
    int16_t speed;
    int16_t current;
    int8_t temp;
    ecd = (uint16_t)(rx_data[0] << 8 | rx_data[1]);
    speed = (int16_t)(rx_data[2] << 8 | rx_data[3]); //rpm
    current = (int16_t)(rx_data[4] << 8 | rx_data[5]);
    temp = rx_data[6];

    //选中需要处理数据的电机
    uint8_t id = rx_header.StdId - 0x200;
    if(motors[id-1] == nullptr) return;
    Motor* motor = motors[id-1];


    //读到的是未处理的编码器角度，值域0-8192，对于编码器角度0-360。编码器角度之差除以减速比，为输出角度变化量
    motor->current_data_.ecd_angle_ = linearmapping<uint16_t, float>(ecd, 0,8191, 0, 360);
    motor->current_data_.delta_ecd_angle_ = motor->current_data_.ecd_angle_ - motor->current_data_.last_ecd_angle_;

    motor->current_data_.delta_ecd_angle_ = degNormalize180(motor->current_data_.delta_ecd_angle_) / motor->ratio_;
    motor->current_data_.motor_angle_ += motor->current_data_.delta_ecd_angle_; //deg


    motor->current_data_.motor_speed_ = rpm2dps(speed)/ motor->ratio_; //dps
    motor->current_data_.current_=linearmapping<uint16_t, float>(current,-16384,16384,-20,20);
    motor->current_data_.temp_=temp;

    motor->current_data_.last_ecd_angle_ = motor->current_data_.motor_angle_;
    //数据更新完成

    motor->deal_error(&PID, target_rotate_speed, actual_rotate_speed);
    motor->control_data_.fdb_angle_ = motor->current_data_.motor_angle_;
    motor->control_data_.last_fdb_angle_ = last_angle_;

    motor->handle();

    intensity_float_ = pid_.calc(control_data_.target_angle_, control_data_.fdb_angle_) + control_data_.
                       feedforward_intensity;


}

//在can接收中断回调函数中调用，用于间接调用MotorManagement中canMotorsCallback()
void canRxHandle(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef rx_header, uint8_t rx_data[8]) {
    motors.canMotorsCallback(hcan, rx_header, rx_data);
}
