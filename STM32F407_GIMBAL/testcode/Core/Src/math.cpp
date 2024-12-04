//
// Created by zhu_y on 2024/11/13.
//
#include "math.h"

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

float pid2vol(const float& pid_output, const float& pid_range) {
    return pid_output*50000/pid_range;
}


float ecd2deg(const float& ecd, const float& ecd_range) {
    return ecd * 360.f / ecd_range;
}

float degNormalize180(const float& angle) {
    return loopLimit(angle, -180.f, 180.f);
}

float rpm2dps(const float& w) {
    return w * 6.f;
}