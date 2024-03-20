/**
 * Majority of code taken from https://github.com/jankammerath/L475VG-IOT01A-Mbed-AWS-IoT
 */

void cloud_init();
void cloud_send(float temperature, float humidity, float pressure);
int cloud_connect();
