//
// Created by zhu_y on 2024/11/13.
//

#ifndef PID_H
#define PID_H

#ifdef __cplusplus
// extern "C"{
// #endif
class PID {
public:
    float kp_, ki_, kd_;
    float i_max_, out_max_;
    float output_;

    float ref_, fdb_;
    float err_, err_sum_, last_err_;
    float pout_, iout_, dout_;

    float calc(float ref, float fdb);
    PID(float kp, float ki, float kd, float i_max, float out_max);
    PID();
};
// #ifdef __cplusplus
// }
#endif

#endif //PID_H
