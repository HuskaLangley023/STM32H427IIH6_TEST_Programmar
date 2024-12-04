//
// Created by zhu_y on 2024/11/20.
//

#ifndef CAN_COMM_H
#define CAN_COMM_H

#include "main.h"
#include "can.h"




#ifdef __cplusplus
extern "C"
{
#endif
void canInit();
#ifdef __cplusplus
}
#endif

void canMotorsTxMsg();

#endif //CAN_COMM_H
