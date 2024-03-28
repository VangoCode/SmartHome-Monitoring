
#ifndef __STM32L475E_IOT01_DSENSOR_H
#define __STM32L475E_IOT01_DSENSOR_H

/* Includes ------------------------------------------------------------------*/
#include "VL53L0X.h"
#include "mbed.h"

typedef enum 
{
  continuous = 0,
  single = 1
} 
DSENSOR_ModeTypeDef;

typedef struct vl53l0x_settings_t
{
  DSENSOR_ModeTypeDef mode;
} VL53L0X_Settings;

typedef enum 
{
  DSENSOR_OK = 0,
  DSENSOR_ERROR = 1,
  DSENSOR_TIMEOUT = 2
} 
DSENSOR_StatusTypeDef;

DSENSOR_StatusTypeDef BSP_DSENSOR_Init(VL53L0X_Settings settings);
void BSP_DSENSOR_DeInit(void);
void BSP_DSENSOR_LowPower(uint16_t status); /* 0 Means Disable Low Power Mode, otherwise Low Power Mode is enabled */
float BSP_DSENSOR_GetReading(void);

#endif /* __STM32L475E_IOT01_DSENSOR_H */

