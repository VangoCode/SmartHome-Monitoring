#include <mbed.h>
#include "stm32l475e_iot01_dsensor.h"

int main()
{
    VL53L0X_Settings settings;
    settings.mode = single;
    BSP_DSENSOR_Init(settings);

    while (1)
    {
        printf("%u\n", BSP_DSENSOR_GetReading());
        HAL_Delay(500);
    }
}
