/*
 * Majority of code here taken from Szymon Szantula 
 * https://github.com/byq77
 */

/*
 * This example shows how to get single-shot range
 * measurements from the VL53L0X. The sensor can optionally be
 * configured with different ranging profiles, as described in
 * the VL53L0X API user manual, to get better performance for
 * a certain application.
 */
#include <mbed.h>
#include <VL53L0X.h>

// DigitalOut hsens_power(SENS_POWER_ON,1); // power on hSens terminals on CORE2
// VL53L0X sensor(PB_8, PB_9);
I2C i2c(PB_11, PB_10); 
Timer timer; 
VL53L0X sensor(i2c, timer);

// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

//#define LONG_RANGE


// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED
#define HIGH_ACCURACY

int main()
{
    printf("Start...\n");
    sensor.init();
    printf("Initialisation completed!\n");
    sensor.setTimeout(500);
#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  sensor.setMeasurementTimingBudget(200000);
#endif
    while (1)
    {
        printf("%u\n", sensor.readRangeSingleMillimeters());
        if (sensor.timeoutOccurred())
        {
            printf("TIMEOUT!\n");
        }
    }
}