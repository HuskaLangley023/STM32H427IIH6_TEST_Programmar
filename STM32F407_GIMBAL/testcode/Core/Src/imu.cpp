//
// Created by zhu_y on 2024/12/4.
//
#include "imu.h"



uint8_t rx_buffer[8];

IMU bmi088;
int a = 0;

IMU::IMU() {
    acc_rate_x = 0;
    acc_rate_y = 0;
    acc_rate_z = 0;
    gy_rate_x = 0;
    gy_rate_y = 0;
    gy_rate_z = 0;
}

//片选
void IMU::ACCEL_NS_L() {
    HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_RESET);
}
void IMU::ACCEL_NS_H() {
    HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_SET);
}
void IMU::GYRO_NS_L() {
    HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_RESET);
}
void IMU::GYRO_NS_H() {
    HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_SET);
}
//读取函数调用前后无需调用片选函数，写入函数前后需要调用片选函数

//GYRO
void IMU::WriteReg_GYRO(uint8_t reg, uint8_t write_data) {
    uint8_t tx_reg;
    // 发送寄存器地址
    tx_reg = 0x00 | reg; // 0000 0000 | reg
    HAL_SPI_Transmit(&hspi1, &tx_reg, 1, HAL_MAX_DELAY); //size是字节数
    //写入
    HAL_SPI_Transmit(&hspi1, &write_data, 1, HAL_MAX_DELAY);
}

void IMU::ReadReg_GYRO(uint8_t reg, uint8_t *return_data, uint8_t length) {
    uint8_t tx_reg;
    //片选
    GYRO_NS_L();
    // 发送寄存器地址
    tx_reg = 0x80 | reg; // 1000 0000 | reg
    HAL_SPI_Transmit(&hspi1, &tx_reg, 1, HAL_MAX_DELAY); //size是字节数
    // 读取数据
    HAL_SPI_Receive(&hspi1, return_data, length, HAL_MAX_DELAY);
    //结束片选
    GYRO_NS_H();
}

void IMU::GYRO_handle() {
    a++;
    uint8_t rx_data[6];
    ReadReg_GYRO(GYRO_DATA_START_REG, rx_data, 6);
    int16_t RATE_X = (int16_t)((int8_t)rx_data[1] << 8 | rx_data[0]);
    int16_t RATE_Y = (int16_t)((int8_t)rx_data[3] << 8 | rx_data[2]);
    int16_t RATE_Z = (int16_t)((int8_t)rx_data[5] << 8 | rx_data[4]);
    gy_rate_x = gyro_lsb2dps(RATE_X);
    gy_rate_y = gyro_lsb2dps(RATE_Y);
    gy_rate_z = gyro_lsb2dps(RATE_Z);
}

//ACCEL
 void IMU::ReadReg_ACCEL(uint8_t reg, uint8_t *return_data, uint8_t length) { // 读一个地址
    uint8_t tx_reg;
    //片选
    ACCEL_NS_L();
    // 发送寄存器地址
    tx_reg = 0x80 | reg; // 1000 0000 | reg
    HAL_SPI_Transmit(&hspi1, &tx_reg, 1, HAL_MAX_DELAY);
    // 读取数据
    HAL_SPI_Receive(&hspi1, return_data, 1, HAL_MAX_DELAY); // 不确定数据
    HAL_SPI_Receive(&hspi1, return_data, length, HAL_MAX_DELAY); // 所需数据
    // for(int i = 0; i<length; i++) {
    //     return_data[i] = return_data[i+1];
    // }
    //结束片选
    ACCEL_NS_H();
 }

 void IMU::WriteReg_ACCEL(uint8_t reg, uint8_t write_data) {
    uint8_t tx_reg;
    // 发送寄存器地址
    tx_reg = 0x00 | reg; // 0000 0000 | reg
    HAL_SPI_Transmit(&hspi1, &tx_reg, 1, HAL_MAX_DELAY); //size是字节数
    //写入
    HAL_SPI_Transmit(&hspi1, &write_data, 1, HAL_MAX_DELAY);
 }

void IMU::ACCEL_handle() {
    uint8_t rx_data[6];
    ReadReg_ACCEL(ACC_DATA_START_REG, rx_data, 6); //有效数据6位，实际读入7位
    //rx_data[0]为废数据
    int16_t RATE_X = (int16_t)((int8_t)rx_data[1] << 8 | rx_data[0]);
    int16_t RATE_Y = (int16_t)((int8_t)rx_data[3] << 8 | rx_data[2]);
    int16_t RATE_Z = (int16_t)((int8_t)rx_data[5] << 8 | rx_data[4]);
    acc_rate_x = acc_lsb2g(RATE_X);
    acc_rate_y = acc_lsb2g(RATE_Y);
    acc_rate_z = acc_lsb2g(RATE_Z);
}

void IMU::init() {
    // Soft Reset ACCEL
    ACCEL_NS_L();
    WriteReg_ACCEL(0x7E, 0xB6); // Write 0xB6 to ACC_SOFTRESET(0x7E)
    HAL_Delay(1);
    ACCEL_NS_H();

    // Soft Reset GYRO
    GYRO_NS_L();
    WriteReg_GYRO(0x14, 0xB6); // Write 0xB6 to GYRO_SOFTRESET(0x14)
    HAL_Delay(30);
    GYRO_NS_H();

    // Switch ACCEL to Normal Mode
    ACCEL_NS_L();
    HAL_Delay(1);
    WriteReg_ACCEL(0x7D, 0x04); // Write 0x04 to ACC_PWR_CTRL(0x7D)
    HAL_Delay(1);
    ACCEL_NS_H();

    GYRO_NS_L();
    WriteReg_GYRO(0x0F, 0x00); // range -+2000
    GYRO_NS_H();

    ACCEL_NS_L();
    WriteReg_ACCEL(0x41, 0x00); // range -+24g
    ACCEL_NS_H();

    ReadReg_GYRO(0x00, &info.gyro_id, 1); //0x0F
    ReadReg_ACCEL(0x00, &info.accel_id, 1); //0x1E
}

void IMU::receive() {
    ACCEL_handle();
    GYRO_handle();
    data[0] = acc_rate_x;
    data[1] = acc_rate_y;
    data[2] = acc_rate_z;
    data[3] = gy_rate_x;
    data[4] = gy_rate_y;
    data[5] = gy_rate_z;
}




