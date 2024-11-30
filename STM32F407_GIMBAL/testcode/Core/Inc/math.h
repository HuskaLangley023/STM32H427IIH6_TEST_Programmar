//
// Created by zhu_y on 2024/11/13.
//

#ifndef MATH_H
#define MATH_H

template<class type_in, class type_out>
type_out linearmapping(type_in in, const type_in& imin, const type_in& imax, const type_out& omin, const type_out& omax);

template<class type>
type limit(type value, const type& min, const type& max);

float loopLimit(float value, const float& min, const float& max);

float degNormalize180(const float& angle);
float rpm2dps(const float& w);
#endif //MATH_H
