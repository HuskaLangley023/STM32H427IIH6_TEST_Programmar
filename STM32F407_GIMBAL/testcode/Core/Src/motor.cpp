//
// Created by zhu_y on 2024/11/13.
//
#include "motor.h"
#include "math.h"
#include "string.h"
#include "remote.h"
#include "can.h"

extern RC rc;
float Amplitude_p = 25;
float Offset_p = 227;
float Amplitude_y = 60;
float Offset_y = 100;
uint32_t b = 0;
uint32_t c = 0;

uint8_t TxData[8] = { 0 };

Motor::Motor(const type &type, const float &ratio, const PID &ppid,const PID &spid, ControlMethod method)
    : type_m(type),
      ratio_(ratio),
      ppid_(ppid),
      spid_(spid),
      method_(method) {
    control_data_.target_angle_ = 0;
}

Motor GMY(Motor::M6020, 1,
        PID(10, 0.1, 10, 10, 1800),
        PID(10, 2, 0, 500, 30000),
        Motor::POSITION_SPEED); //Yaw轴电机
Motor GMP(Motor::M6020, 1,
        PID(10, 0.1, 10, 10, 1800),
        PID(400, 2, 0, 500, 30000),
        Motor::POSITION_SPEED); //Pitch轴电机
Motor GMFL(Motor::M3508, 3591.f / 87.f,
        PID(20, 0.1, 10, 10, 2500),
        PID(400, 2, 0, 500, 30000),
        Motor::POSITION_SPEED); //左摩擦轮电机
Motor GMFR(Motor::M3508, 3591.f / 87.f,
        PID(20, 0.1, 10, 10, 2500),
        PID(400, 2, 0, 500, 30000),
        Motor::POSITION_SPEED); //右摩擦轮电机

void Motor::setAngle(float angle) {
    control_data_.target_angle_ = angle;
}


float pid_output_;
void Motor::handle() {
    // control_data_.target_angle_ = //设置目标角度，或许与前馈有关？
    if(method_ == POSITION) {
        pid_output_ = ppid_.calc(control_data_.target_angle_, control_data_.fdb_angle_);
    }else if(method_ ==  POSITION_SPEED) {
        control_data_.target_speed_ = ppid_.calc(control_data_.target_angle_,control_data_.fdb_angle_);
        pid_output_ = spid_.calc(control_data_.target_speed_,
                                      control_data_.fdb_speed_);
    }

    setvoltage(pid_output_);
}

void Motor::setvoltage(float output) { //可能要调整为setvoltage
    control_data_.control_voltage_f = pid2vol(pid_output_,60000); //-25000~25000, 对应-3~3A
    control_data_.control_voltage_16 = (int16_t)control_data_.control_voltage_f;
}

void Motor::sendMsg() {
    CAN_TxHeaderTypeDef TxHeader;
    // uint8_t TxData[8] = { 0 };
    uint32_t TxMailbox;

    TxHeader.StdId = 0x1FF;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 8;

    TxData[2*info.id-2] = (control_data_.control_voltage_16 >> 8) & 0xFF; // 高8位
    TxData[2*info.id-1] = control_data_.control_voltage_16 & 0xFF; // 低8位

    if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
        Error_Handler();
    }
}


//MotorMangement

Motor *motors_in_use[4] = {
    &GMP, //ID:1
    &GMY,//ID:2
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
    if (rx_header.StdId < 0x205 || rx_header.StdId > 0x208) return;
    int16_t ecd;
    int16_t speed;
    int16_t current;
    int8_t temp;
    ecd = (uint16_t)(rx_data[0] << 8 | rx_data[1]);
    speed = (int16_t)(rx_data[2] << 8 | rx_data[3]); //rpm
    current = (int16_t)(rx_data[4] << 8 | rx_data[5]);
    temp = rx_data[6];

    //选中需要处理数据的电机
    uint8_t id = rx_header.StdId - 0x204;
    if(motors[id-1] == nullptr) return;
    Motor* motor = motors[id-1];


    //读到的是未处理的编码器角度，值域0-8192，对于编码器角度0-360。编码器角度之差除以减速比，为输出角度变化量
    motor->current_data_.ecd_angle_ = ecd2deg(ecd,8192);
    motor->current_data_.delta_ecd_angle_ = motor->current_data_.ecd_angle_ - motor->current_data_.last_ecd_angle_;

    motor->current_data_.delta_angle_ = degNormalize180(motor->current_data_.delta_ecd_angle_) / motor->ratio_;
    motor->current_data_.motor_angle_ += motor->current_data_.delta_angle_; //deg
    motor->control_data_.last_fdb_angle_ = motor->control_data_.fdb_angle_;
    motor->control_data_.fdb_angle_ = motor->current_data_.motor_angle_;

    motor->current_data_.motor_speed_ = rpm2dps(speed)/ motor->ratio_; //dps
    motor->control_data_.fdb_speed_ = motor->current_data_.motor_speed_;

    motor->current_data_.current_ = current;
    motor->current_data_.temp_=temp;

    motor->current_data_.last_ecd_angle_ = motor->current_data_.ecd_angle_;
    //数据更新完成

}

float test_angle = 180;
void MotorManagement::control() {
    //处理发送数据
    // float remote_input_p = rc.control_data_[3];
    // float target_angle_p = Amplitude_p * remote_input_p + Offset_p;
    // GMP.setAngle(target_angle_p);
    // GMP.handle();
    float remote_input_y = rc.control_data_[2];
    float target_angle_y = Amplitude_y * remote_input_y + Offset_y;
    // GMY.setAngle(target_angle_y);
    motors[1]->setAngle(test_angle); //Yaw
    motors[1]->handle();

    motors[1]->sendMsg();
    // motors[]->sendMsg();
}

//在can接收中断回调函数中调用，用于间接调用MotorManagement中canMotorsCallback()
void canRxHandle(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef rx_header, uint8_t rx_data[8]) {
    motors.canMotorsCallback(hcan, rx_header, rx_data);
}


