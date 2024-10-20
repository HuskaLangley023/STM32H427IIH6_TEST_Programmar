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

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define RCChannel_BF_Max (uint16_t)1684
#define RCChannel_BF_Min (uint16_t)364
#define RCChannel_BF_Offset (uint16_t)1024
#define RCChannel_AF_Max 1
#define RCChannel_AF_Min -1

#define RCBotton_Max 3
#define RCBotton_Mid 2
#define RCBotton_Min 1

#define RCMouse_BF_Max 32767
#define RCMouse_BF_Min -32768
#define RCMouse_AF_Max 1
#define RCMouse_AF_Min -1

#define RCSwitch_Max 1
#define RCSwitch_Min 0

#define KEY_PRESSED_OFFSET_W               ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S               ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A               ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D               ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_Q               ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_E               ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_SHIFT           ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_CTRL            ((uint16_t)0x01<<7)

  typedef enum {
    Botton_HL = RCBotton_Max,
    Botton_CL = RCBotton_Mid,
    Botton_Off = RCBotton_Min,
}RCBottonTypeDef;

  typedef enum {
    Switch_On = RCSwitch_Max,
    Switch_Off = RCSwitch_Min,
}RCSwitchTypeDef;


  typedef struct
  {
    struct
    {
      uint16_t ch0;
      uint16_t ch1;
      uint16_t ch2;
      uint16_t ch3;
      uint8_t  s1;
      uint8_t  s2;
    }rc;
    struct
    {
      int16_t x;
      int16_t y;
      int16_t z;
      uint8_t press_l;
      uint8_t press_r;
    }mouse;
    struct
    {
      uint16_t v;
    }key;
  } RC_Ctl_t;

  typedef struct {
    float r_row;
    float r_col;
    float l_row;
    float l_col;

    RCBottonTypeDef s1;
    RCBottonTypeDef s2;

    RCSwitchTypeDef l;
    RCSwitchTypeDef r;

    float mouse_x;
    float mouse_y;
    float mouse_z;
  }RC;
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

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
