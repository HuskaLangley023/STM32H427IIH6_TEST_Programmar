//
// Created by zhu_y on 2024/11/13.
//
#include "math.h"

template<class type_in, class type_out>
type_out linearmapping(type_in in, const type_in& imin, const type_in& imax, const type_out& omin, const type_out& omax) {
    type_out k = (omax - omin) / (type_out) (imax - imin);
    type_out b = (type_out) -k * (type_out) imax;
    return k * (type_out) in +b;
}

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

//返回value在min与max间的主值
float loopLimit(float value, const float& min, const float& max) {
    if (min >= max)
        return value;
    if (value > max) {
        while (value > max)
            value -= (max - min);
    } else if (value < min) {
        while (value < min)
            value += (max - min);
    }
    return value;
}

float degNormalize180(const float& angle) {
    return loopLimit(angle, -180.f, 180.f);
}

float rpm2dps(const float& w) {
    return w * 6.f;
}