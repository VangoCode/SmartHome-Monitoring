#include "settings.h"
#include <string>
#include "mbed.h"
#include "stm32l475e_iot01_accelero.h"
#include "stm32l475e_iot01_dsensor.h"
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_psensor.h"
#include "stm32l475e_iot01_gyro.h"
#include "stm32l475e_iot01_magneto.h"
#include "VL53L0X.h"


SettingsInterface::SettingsInterface(std::string* sensors_list, int number_of_sensors, Threshold* threshold_settings)
{
    // renamed function param to avoid same name issue
    num_sensors = number_of_sensors;
    sensors = sensors_list;
    thresholds = threshold_settings;
    for (int i = 0; i < number_of_sensors; i++)
    {
        if (sensors[i] == "humidity") {
            BSP_HSENSOR_Init();

            p[i] = BSP_HSENSOR_ReadHumidity;
        } else if (sensors[i] == "temperature") {
            BSP_TSENSOR_Init();

            p[i] = BSP_TSENSOR_ReadTemp;
        } else if (sensors[i] == "pressure") {
            BSP_PSENSOR_Init();

            p[i] = BSP_PSENSOR_ReadPressure;
        } else if (sensors[i] == "distance") {
            VL53L0X_Settings settings;
            settings.mode = single;
            BSP_DSENSOR_Init(settings);

            p[i] = (BSP_DSENSOR_CAST) BSP_DSENSOR_GetReading;
        }
    }
}

void SettingsInterface::get_sensor_data(void)
{   
    for (int i = 0; i < num_sensors; i++)
    {
      float sensor = p[i]();
      printf("sensor: %f\n", sensor);
      if (sensor < thresholds[i].min)
      {
        // SEND MQTT MESSAGE THAT BELOW THRESHOLD
        // TODO
      }
      if (sensor > thresholds[i].max)
      {
        // SEND MQTT MESSAGE THAT ABOVE THRESHOLD
        // TODO
      }
    //   return sensor;
    }
};

/* example sensors: ['humidity', 'temperature', ...] */
SettingsInterface* initialize_settings_test(std::string* sensors_list, int num_sensors, Threshold* threshold_settings)
{
  SettingsInterface* settings = new SettingsInterface(sensors_list, num_sensors, threshold_settings);
  return settings;
}
