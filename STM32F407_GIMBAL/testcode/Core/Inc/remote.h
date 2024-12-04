//
// Created by zhu_y on 2024/11/27.
//

#ifndef REMOTE_H
#define REMOTE_H

#include "main.h"
#include "math.h"

//映射前
#define RCChannel_BF_Max (uint16_t)1684
#define RCChannel_BF_Offset (uint16_t)1024
#define RCChannel_BF_Min (uint16_t)364
//映射后
#define RCChannel_AF_Max 1
#define RCChannel_AF_Offset 0
#define RCChannel_AF_Min -1

#define RCBotton_Min (uint8_t)1
#define RCBotton_Mid (uint8_t)2
#define RCBotton_Max (uint8_t)3

#ifdef __cplusplus
class RC {
private:
    void reset() {
        current_data_.r_col = 0;
        current_data_.r_row = 0;
        current_data_.l_col = 0;
        current_data_.l_row = 0;
        current_data_.s1 = Botton_Off;
        current_data_.s2 = Botton_Off;
    }
public:
    enum RCBotton{
        Botton_H = RCBotton_Min,
        Botton_L = RCBotton_Mid,
        Botton_Off = RCBotton_Max,
    };

    struct CurrentData {
        float r_row;
        float r_col;
        float l_row; //Yaw
        float l_col; //Pitch
        RCBotton s1;
        RCBotton s2;
    }current_data_;

    struct RawData{
        uint16_t ch0;
        uint16_t ch1;
        uint16_t ch2;
        uint16_t ch3;
        uint8_t  s1;
        uint8_t  s2;
    }raw_data_;

    float control_data_[6];

    RC();

    float RCChannel_mapping(uint16_t rc_ch){
        return linearmapping<uint16_t,float>(rc_ch, RCChannel_BF_Min, RCChannel_BF_Max, RCChannel_AF_Min, RCChannel_AF_Max);
    }

    void handle(uint8_t rubuffer[18]);
};
#endif
void uartRxHandle(uint8_t rx_buffer[18]);

#endif //REMOTE_H
