//
// Created by zhu_y on 2024/11/2.
//

#ifndef BMI088_H
#define BMI088_H
#include "main.h"
#include "spi.h"
#include "tim.h"

#define ACC_DATA_START_REG 0x12
#define GYRO_DATA_START_REG 0x02

typedef struct {

        // float acc_rate_x;
        // float acc_rate_y;
        // float acc_rate_z;

        float gy_rate_x;
        float gy_rate_y;
        float gy_rate_z;

}BMI088;

void BMI088_ACCEL_NS_L(void);
void BMI088_ACCEL_NS_H(void);
void BMI088_GYRO_NS_L(void);
void BMI088_GYRO_NS_H(void);

void BMI088_Read_Reg_ACCEL(uint8_t reg, uint8_t *return_data, uint8_t length);
void BMI088_Read_Reg_GYRO(uint8_t reg, uint8_t *return_data, uint8_t length);
void BMI088_Write_Reg_ACCEL(uint8_t reg, uint8_t write_data, uint8_t length);
void BMI088_Write_Reg_GYRO(uint8_t reg, uint8_t write_data, uint8_t length);

void BMI088_Init(void);

void GYRO_handle();
void ACCEL_handle();

float linearmapping(int16_t input, float max_f, float min_f, int max_bi, int min_bi);



#endif //BMI088_H
