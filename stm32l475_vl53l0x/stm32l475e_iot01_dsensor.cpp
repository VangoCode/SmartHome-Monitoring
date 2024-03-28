/*
 * Majority of code here taken from Szymon Szantula 
 * https://github.com/byq77
 * https://os.mbed.com/users/byq77/code/vl53l0x-mbed/
 */

/**
  ******************************************************************************
  * @file    stm32l475e_iot01_dsensor.c
  * @author  Ron Varshavsky, Muhan Zhu
  * @brief   This file provides a set of functions needed to manage the distance sensor
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l475e_iot01_dsensor.h"
#include "mbed.h"

I2C i2c(PB_11, PB_10); 
Timer timer; 
VL53L0X sensor(i2c, timer);

DSENSOR_StatusTypeDef BSP_DSENSOR_Init(VL53L0X_Settings settings)
{  
    printf("Start...\n");
    sensor.init();
    printf("Initialisation completed!\n");
    sensor.setTimeout(500);
    // This increases the sensitivity of the sensor and extends its potential range, but increases the 
    // likelihood of getting an inaccurate reading because of reflections from other objects other than 
    // the intended target - It works best in dark conditions
    // lower the return signal rate limit (default is 0.25 MCPS)
    sensor.setSignalRateLimit(0.1);
    // increase laser pulse periods (defaults are 14 and 10 PCLKs)
    sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
    // reduce timing budget to 20 ms (default is about 33 ms)
    // allows us to have higher speed at the cost of lower accuracy
    sensor.setMeasurementTimingBudget(20000);

    return DSENSOR_OK;
}

/**
  * @brief  DeInitialize the Distance Sensor.
  * @retval None.
  */
void BSP_DSENSOR_DeInit(void)
{
  
}

/**
  * @brief  Set/Unset the Distance Sensor in low power mode.
  * @param  status 0 means disable Low Power Mode, otherwise Low Power Mode is enabled
  * @retval None
  */
void BSP_DSENSOR_LowPower(uint16_t status)
{
  
}

/**
  * @brief  Get distance sensor values.
  * @retval the reading from the distance sensor
  */
float BSP_DSENSOR_GetReading(void)
{
    unsigned int reading = sensor.readRangeSingleMillimeters();
    printf("%u\n", reading);
    if (sensor.timeoutOccurred())
    {
        printf("TIMEOUT!\n");
    }
    return (float) reading;
}
