/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include<can.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
  typedef struct{

    float ratio_;
    float angle_;
    float delta_angle_;
    float ecd_angle_;
    float last_ecd_angle_;
    float delta_ecd_angle_;
    float rotate_speed_;
    float current_;
    float temp_;

    // uint16_t uint16_ecd_angle_;
    // uint16_t uint16_last_ecd_angle_;
    // uint16_t uint16_delta_ecd_angle_;
    // uint16_t uint16_sum_ecd_angle_;
    // uint16_t uint16_abs_sum_delta_ecd_angle_;
    // float abs_sum_delta_angle_;
    // float error_angle;
    // float error_time;



  }M3508_Motor;

  float linearMapping(uint16_t in, int in_min, int in_max, float out_min, float out_max);

  void solve_data(uint8_t &rx_data, M3508_Motor motor) ;
  // void motor_init(M3508_Motor motor) {
  //   motor.angle_ = 0;
  //   motor.current_ = 0;
  //   motor.delta_angle_ = 0;
  //   motor.delta_ecd_angle_ = 0;
  //   motor.ecd_angle_ = 0;
  //   motor.last_ecd_angle_ = 0;
  //   motor.ratio_ = 19;
  //   motor.rotate_speed_ = 0;
  //   motor.temp_ = 0;
  // }

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_G_Pin GPIO_PIN_14
#define LED_G_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
