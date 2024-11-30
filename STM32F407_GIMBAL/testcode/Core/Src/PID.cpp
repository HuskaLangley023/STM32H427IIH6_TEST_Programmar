//
// Created by zhu_y on 2024/11/13.
//
#include "PID.h"

#include "math.h"

PID::PID(float kp = 0, float ki = 0, float kd = 0,
         float i_max = 0, float out_max = 0)
    :kp_(kp),
     ki_(ki),
     kd_(kd),
     i_max_(i_max),
     out_max_(out_max) {
    // err_ = 0;
    // err_sum_ = 0;
    // last_err_ = 0;
}

float PID::calc(float ref, float fdb) {
    //数据更新
    ref_ = ref;
    fdb_ = fdb;
    last_err_ = err_;
    err_ = ref_ - fdb_;
    err_sum_ = limit(err_sum_+err_, -i_max_, i_max_);

    pout_ = kp_ * err_;
    iout_ = ki_ * err_sum_;
    dout_ = kd_ * (err_ - last_err_);
    output_ = limit(pout_ + iout_ + dout_, -out_max_, out_max_);

    return output_;
}


void PID::deal_error(float ref, float fdb) {
    float pid_current = 10000 * calc(ref, fdb) / out_max_;
    control_current(pid_current);
}



