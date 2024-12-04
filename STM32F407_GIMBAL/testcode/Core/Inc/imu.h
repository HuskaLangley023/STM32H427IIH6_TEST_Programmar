//
// Created by zhu_y on 2024/12/4.
//

#ifndef IMU_H
#define IMU_H

#include "main.h"
#include "math.h"
#include "spi.h"
#include "tim.h"

#define ACC_DATA_START_REG 0x12
#define GYRO_DATA_START_REG 0x02

class IMU {
public:
    enum Type {
        Bmi088,
    }type_;

    struct Info {
        uint8_t accel_id;
        uint8_t gyro_id;
    }info;

    float acc_rate_x;
    float acc_rate_y;
    float acc_rate_z;

    float gy_rate_x;
    float gy_rate_y;
    float gy_rate_z;

    float data[6];

    IMU();
    void ACCEL_NS_L();
    void ACCEL_NS_H();
    void GYRO_NS_L();
    void GYRO_NS_H();

    void ReadReg_ACCEL(uint8_t reg, uint8_t *return_data, uint8_t length);
    void ReadReg_GYRO(uint8_t reg, uint8_t *return_data, uint8_t length);
    void WriteReg_ACCEL(uint8_t reg, uint8_t write_data);
    void WriteReg_GYRO(uint8_t reg, uint8_t write_data);

    void init();

    void GYRO_handle();
    void ACCEL_handle();

    float linearmapping(int16_t input, float max_f, float min_f, int max_bi, int min_bi);
    void receive();

};





#endif //IMU_H
