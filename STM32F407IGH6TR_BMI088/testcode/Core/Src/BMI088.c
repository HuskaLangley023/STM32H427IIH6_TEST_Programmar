//
// Created by zhu_y on 2024/11/2.
//
#include "BMI088.h"

extern uint8_t rx_data[6];
extern uint8_t tx_reg;
extern uint8_t rx_buffer[8];
uint8_t acc_chipID;
uint8_t gyro_chipID;

BMI088 bmi088 = {
    .acc_rate_x = 0,
    .acc_rate_y = 0,
    .acc_rate_z = 0,
    .gy_rate_x = 0,
    .gy_rate_y = 0,
    .gy_rate_z = 0
    };

//片选
void BMI088_ACCEL_NS_L(void) {
    HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_RESET);
}
void BMI088_ACCEL_NS_H(void) {
    HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_SET);
}
void BMI088_GYRO_NS_L(void) {
    HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_RESET);
}
void BMI088_GYRO_NS_H(void) {
    HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_SET);
}

//GYRO
void BMI088_Write_Reg_GYRO(uint8_t reg, uint8_t write_data) {
    BMI088_GYRO_NS_L();
    // 发送寄存器地址
    tx_reg = 0x00 | reg; // 0000 0000 | reg
    HAL_SPI_Transmit(&hspi1, &tx_reg, 1, HAL_MAX_DELAY); //size是字节数
    //写入
    HAL_SPI_Transmit(&hspi1, &write_data, 1, HAL_MAX_DELAY);
    BMI088_GYRO_NS_H();
}

void BMI088_Read_Reg_GYRO(uint8_t reg, uint8_t *return_data, uint8_t length) {
    //片选
    BMI088_GYRO_NS_L();
    // 发送寄存器地址
    tx_reg = 0x80 | reg; // 1000 0000 | reg
    HAL_SPI_Transmit(&hspi1, &tx_reg, 1, HAL_MAX_DELAY); //size是字节数
    // 读取数据
    HAL_SPI_Receive(&hspi1, return_data, length, HAL_MAX_DELAY);
    //结束片选
    BMI088_GYRO_NS_H();
}

void GYRO_handle() {
    uint8_t rx_data[6];
    BMI088_Read_Reg_GYRO(GYRO_DATA_START_REG, rx_data, 6);
    int16_t RATE_X = rx_data[1] << 8 | rx_data[0];
    int16_t RATE_Y = rx_data[3] << 8 | rx_data[2];
    int16_t RATE_Z = rx_data[5] << 8 | rx_data[4];
    bmi088.gy_rate_x = linearmapping(RATE_X, 2000, -2000, 32767, -32767);
    bmi088.gy_rate_y = linearmapping(RATE_Y, 2000, -2000, 32767, -32767);
    bmi088.gy_rate_z = linearmapping(RATE_Z, 2000, -2000, 32767, -32767);
    BMI088_ACCEL_NS_L();
    BMI088_Read_Reg_ACCEL(0x23, rx_buffer+1,1); // Write 0xB6 to ACC_SOFTRESET(0x7E)
    BMI088_Read_Reg_ACCEL(0x00, rx_buffer+2,1); // Write 0xB6 to ACC_SOFTRESET(0x7E)
    HAL_Delay(1);
    BMI088_ACCEL_NS_H();
}

//ACCEL
 void BMI088_Read_Reg_ACCEL(uint8_t reg, uint8_t *return_data, uint8_t length) { // 读一个地址
     //片选
     BMI088_ACCEL_NS_L();
     // 发送寄存器地址
     tx_reg = 0x80 | reg; // 1000 0000 | reg
     HAL_SPI_Transmit(&hspi1, &tx_reg, 1, HAL_MAX_DELAY);
    uint8_t rx[length+1];
     // 读取数据
     // HAL_SPI_Receive(&hspi1, return_data, 1, HAL_MAX_DELAY);
     HAL_SPI_Receive(&hspi1, rx, length+1, HAL_MAX_DELAY); // 所需数据
    for(int i = 0; i<length ; i++) {
        return_data[i] = rx[i+1];
    }
     //结束片选
     BMI088_ACCEL_NS_H();
 }

 void BMI088_Write_Reg_ACCEL(uint8_t reg, uint8_t write_data) {
     BMI088_ACCEL_NS_L();
     // 发送寄存器地址
     tx_reg = 0x00 | reg; // 0000 0000 | reg
     HAL_SPI_Transmit(&hspi1, &tx_reg, 1, HAL_MAX_DELAY); //size是字节数
     //写入
     HAL_SPI_Transmit(&hspi1, &write_data, 1, HAL_MAX_DELAY);
     BMI088_ACCEL_NS_H();
 }


void ACCEL_handle() {
    // uint8_t rx_data[6];
    BMI088_Read_Reg_ACCEL(ACC_DATA_START_REG, rx_data, 6);
    int16_t RATE_X = rx_data[2] << 8 | rx_data[1];
    int16_t RATE_Y = rx_data[4] << 8 | rx_data[3];
    int16_t RATE_Z = rx_data[5] << 8 | rx_data[5];
    bmi088.acc_rate_x = linearmapping(RATE_X, 2000, -2000, 32767, -32767);
    bmi088.acc_rate_y = linearmapping(RATE_Y, 2000, -2000, 32767, -32767);
    bmi088.acc_rate_z = linearmapping(RATE_Z, 2000, -2000, 32767, -32767);
}

void BMI088_Init(void) {

    // Soft Reset ACCEL

    BMI088_Write_Reg_ACCEL(0x7E, 0xB6); // Write 0xB6 to ACC_SOFTRESET(0x7E)
    HAL_Delay(1);


    // Soft Reset GYRO

    BMI088_Write_Reg_GYRO(0x14, 0xB6); // Write 0xB6 to GYRO_SOFTRESET(0x14)
    HAL_Delay(30);


    // Switch ACCEL to Normal Mode

    HAL_Delay(1);
    BMI088_Write_Reg_ACCEL(0x7D, 0x04); // Write 0x04 to ACC_PWR_CTRL(0x7D)
    HAL_Delay(1);



    BMI088_Read_Reg_ACCEL(0x00, &acc_chipID,1); // Write 0xB6 to ACC_SOFTRESET(0x7E)
    HAL_Delay(1);


}


float linearmapping(int16_t input, float max_f, float min_f, int max_bi, int min_bi) {
    float output;
    float k = (max_f - min_f)/(float)(max_bi - min_bi);
    float b = max_f - k * max_bi;
    output = k * input + b;
    return output;
}




