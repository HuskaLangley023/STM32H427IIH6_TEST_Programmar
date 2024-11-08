//
// Created by zhu_y on 2024/11/2.
//
#include "BMI088.h"

extern uint8_t rx_data;
extern uint8_t tx_reg;
extern uint8_t rx_buffer[8];
uint8_t acc_chipID;
uint8_t gyro_chipID;

BMI088 bmi088 = {
    // .acc_rate_x = 0,
    // .acc_rate_y = 0,
    // .acc_rate_z = 0,
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
void BMI088_Write_Reg_GYRO(uint8_t reg, uint8_t write_data, uint8_t length) {
    BMI088_GYRO_NS_L();
    // 发送寄存器地址
    tx_reg = 0x00 | reg; // 0000 0000 | reg
    HAL_SPI_Transmit(&hspi1, &tx_reg, 1, HAL_MAX_DELAY); //size是字节数
    //写入
    HAL_SPI_Transmit(&hspi1, &write_data, length, HAL_MAX_DELAY);
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
}

//ACCEL
 void BMI088_Read_Reg_ACCEL(uint8_t reg, uint8_t *return_data, uint8_t length) { // 读一个地址
     //片选
     BMI088_ACCEL_NS_L();
     // 发送寄存器地址
     tx_reg = 0x80 | reg; // 1000 0000 | reg
     HAL_SPI_Transmit(&hspi1, &tx_reg, 1, HAL_MAX_DELAY);
     // 读取数据
     HAL_SPI_Receive(&hspi1, return_data, 1, HAL_MAX_DELAY); // 不确定数据
     HAL_SPI_Receive(&hspi1, return_data, length, HAL_MAX_DELAY); // 所需数据

     //结束片选
     BMI088_ACCEL_NS_H();
 }

 void BMI088_Write_Reg_ACCEL(uint8_t reg, uint8_t write_data, uint8_t length) {
     BMI088_ACCEL_NS_L();
     // 发送寄存器地址
     tx_reg = 0x00 | reg; // 0000 0000 | reg
     HAL_SPI_Transmit(&hspi1, &tx_reg, 1, HAL_MAX_DELAY); //size是字节数
     //写入
     HAL_SPI_Transmit(&hspi1, &write_data, length, HAL_MAX_DELAY);
     BMI088_ACCEL_NS_H();
 }

void BMI088ReadMultipleByte(GPIO_TypeDef* ssPort, uint8_t ssPin, uint8_t reg, uint8_t* rxData, uint8_t length)
{
    HAL_GPIO_WritePin(ssPort, ssPin, GPIO_PIN_RESET);
  uint8_t txData = reg | 0x80;
        HAL_SPI_Transmit(&hspi1, &txData, 1, HAL_MAX_DELAY);
        // while(HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX);

    HAL_SPI_Receive(&hspi1, rxData, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, rxData, length, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(ssPort, ssPin, GPIO_PIN_SET);
}

// void ACCEL_handle() {
//     uint8_t rx_data[6];
//     BMI088_Read_Reg_ACCEL(ACC_DATA_START_REG, rx_data, 6);
//     int16_t RATE_X = rx_data[1] << 8 | rx_data[0];
//     int16_t RATE_Y = rx_data[3] << 8 | rx_data[2];
//     int16_t RATE_Z = rx_data[5] << 8 | rx_data[4];
//     bmi088.acc_rate_x = linearmapping(RATE_X, 2000, -2000, 32767, -32767);
//     bmi088.acc_rate_y = linearmapping(RATE_Y, 2000, -2000, 32767, -32767);
//     bmi088.acc_rate_z = linearmapping(RATE_Z, 2000, -2000, 32767, -32767);
// }

void BMI088_Init(void) {
    BMI088_Read_Reg_GYRO(0x00, &gyro_chipID, 1);
    BMI088_Write_Reg_GYRO(0x7E, 0xB6, 1);


    BMI088ReadMultipleByte(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, 0X00, &acc_chipID, 1);
    BMI088_Read_Reg_GYRO(0x02, rx_buffer, 6);
    // BMI088_Write_Reg_ACCEL(0x27, 0x05, 1);
    // BMI088_Read_Reg_GYRO(0x02, &rx_data, 1);
}

// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//     if(htim == &htim6) {
//         GYRO_handle();
//         ACCEL_handle();
//     }
// }

float linearmapping(int16_t input, float max_f, float min_f, int max_bi, int min_bi) {
    float output;
    float k = (max_f - min_f)/(float)(max_bi - min_bi);
    float b = max_f - k * max_bi;
    output = k * input + b;
    return output;
}




