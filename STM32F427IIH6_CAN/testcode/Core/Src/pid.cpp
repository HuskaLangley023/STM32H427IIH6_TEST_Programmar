//
// Created by zhu_y on 2024/10/26.
//
#include "pid.h"


pid::pid(float kp_, float ki_, float kd_, float i_max, float out_max):
kp_(kp_),ki_(ki_),kd_(kd_),i_max_(i_max),out_max_(out_max) {
    output_ = 0;
    ref_ = 0;
    fdb_ = 0;
    err_ = 0;
    err_sum_ = 0;
    last_err_ = 0;
    pout_ = 0;
    iout_ = 0;
    dout_ = 0;
}

float pid::calc(float ref, float fdb) {
    last_err_ = err_;
    err_ = ref - fdb;
    pout_ = kp_ * err_;
    limit_err_sum(err_);
    iout_ = ki_ * err_sum_;
    dout_ = kd_ * (err_ - last_err_);
    return output_ = pout_ + iout_ + dout_;
}

void pid::limit_err_sum(float err_) {
    if(err_sum_<i_max_&&err_sum_>-i_max_) err_sum_ += err_;
    else if(err_sum_ > i_max_) err_sum_ = i_max_;
    else if(err_sum_ < -i_max_) err_sum_ = -i_max_;
}