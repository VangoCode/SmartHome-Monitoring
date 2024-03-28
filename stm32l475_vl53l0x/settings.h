#include <string.h>
#include <string>

#ifndef _385_SETTINGS_H
#define _385_SETTINGS_H


typedef struct threshold_t
{
  float min;
  float max;
} Threshold;

class SettingsInterface
{
  public:
    float (*p[4]) (void);
    int num_sensors;
    std::string* sensors;
    Threshold* thresholds;
  
    SettingsInterface(std::string* sensors_list, int number_of_sensors, Threshold* threshold_settings);
    void get_sensor_data(void);
};

SettingsInterface* initialize_settings_test(std::string* sensors_list, int num_sensors, Threshold* threshold_settings);

#endif