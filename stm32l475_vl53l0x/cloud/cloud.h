/**
 * Majority of code taken from https://github.com/jankammerath/L475VG-IOT01A-Mbed-AWS-IoT
 */
#include <string>
#include "mbed.h"

int cloud_init();
void cloud_send(float value, std::string sensor, bool max);
int cloud_connect();


/*
    Listen for MQTT messages from the cloud
*/
void cloud_read(short subscribe);


int cloud_keep_connection_alive();