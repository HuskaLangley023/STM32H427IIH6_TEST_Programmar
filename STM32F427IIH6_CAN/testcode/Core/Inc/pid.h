//
// Created by zhu_y on 2024/10/26.
//

#ifndef PID_H
#define PID_H
#include "main.h"

class pid {
public:
    pid(float kp_, float ki_, float kd_, float i_max, float out_max);
    float calc(float ref, float fdb);
    void limit_err_sum(float err_);

private:
    float kp_,ki_,kd_;
    float i_max_,out_max_;

    float output_;
    float ref_,fdb_;
    float err_,err_sum_,last_err_;
    float pout_,iout_,dout_;
};

#endif //PID_H
