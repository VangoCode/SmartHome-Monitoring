/*
 *
 * VL53L0X I2C Driver
 * Authors: Ron Varshavsky, Muhan Zhu
 * Acknowledgement: much of the setup was taken from https://www.youtube.com/watch?v=_JQAve05o_0
 */

#include "VL53L0X.hpp"
#include <stdio.h>

uint8_t VL53L0X_Init(VL53L0X *device, I2C_HandleTypeDef *i2cHandle) {
    // initialize device struct
    device->i2cHandle = i2cHandle;
    device->distance_cm = 0.0f;

    /* error count */
    uint8_t error_count = 0;
    HAL_StatusTypeDef status;

    /*
     * Check initial i2c device values
     */
    uint8_t regData;

    printf("above status\n");

    status = VL53L0X_ReadRegister(device, register_0xC0, &regData);
    printf("under status\n");
    error_count += ( status != HAL_OK );


    if (regData != initial_register_addr_0xC0) {
        printf("250: reg data was: %d; status was %d \n",regData, status);
        // return 250;
    }
    //----
    status = VL53L0X_ReadRegister(device, register_0xC1, &regData);
    error_count += ( status != HAL_OK );

    if (regData != initial_register_addr_0xC1) {
        printf("251: reg data was: %d; status was %d \n",regData, status);
        // return 251;
    }
    //----
    status = VL53L0X_ReadRegister(device, register_0xC2, &regData);
    error_count += ( status != HAL_OK );

    if (regData != initial_register_addr_0xC2) {
        printf("252: reg data was: %d; status was %d \n",regData, status);
        // return 252;
    }
    //----
    status = VL53L0X_ReadRegister(device, register_0x51, &regData);
    error_count += ( status != HAL_OK );

    if (regData != initial_register_addr_0x51) {
        printf("253: reg data was: %d; status was %d \n",regData, status);
        // return 253;
    }
    //----
    status = VL53L0X_ReadRegister(device, register_0x61, &regData);
    error_count += ( status != HAL_OK );

    if (regData != initial_register_addr_0x61) {
        printf("255: reg data was: %d; status was %d \n",regData, status);
        // return 255;
    }

    printf("error count: %d \n", error_count);



    // return no error, for now
    return 0;
}

/*
 * LOW LEVEL REGISTER FUNCTIONS
 */
HAL_StatusTypeDef VL53L0X_ReadRegister(VL53L0X *device, uint8_t reg, uint8_t *data) {
    // device->i2cHandle, I2C_MEMADD_SIZE_8BIT, HAL_MAX_DELAY are all part of HAL
    return HAL_I2C_Mem_Read(device->i2cHandle, I2C_ADDRESS_READ, reg, I2C_MEMADD_SIZE_16BIT, data, 1, HAL_MAX_DELAY);
}