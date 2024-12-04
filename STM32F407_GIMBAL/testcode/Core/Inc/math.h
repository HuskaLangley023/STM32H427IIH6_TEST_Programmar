//
// Created by zhu_y on 2024/11/13.
//

#ifndef MATH_H
#define MATH_H

#include "main.h"

#ifdef __cplusplus
//模板映射存在精度问题，暂时废除
template<class type_in, class type_out>
type_out linearmapping(type_in in, const type_in& imin, const type_in& imax, const type_out& omin, const type_out& omax) {
    type_out k = (omax - omin) / (type_out) (imax - imin);
    type_out b = (type_out)omax -k * (type_out) imax;
    return k * (type_out) in +b;
}

float ecd2deg(const float& ecd, const float& ecd_range);

float pid2vol(const float& pid_output, const float& pid_range);


//将value钳制在一个区间
template<class type>
type limit(type value, const type& min, const type& max) {
    if (min >= max) return value;
    else {
        if (value < min) value = min;
        else if (value > max) value = max;
        else return value;
    }
    return value;
}

float loopLimit(float value, const float& min, const float& max);

float degNormalize180(const float& angle);
float rpm2dps(const float& w);
#endif
#endif //MATH_H
