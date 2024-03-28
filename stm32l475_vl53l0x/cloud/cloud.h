/**
 * Majority of code taken from https://github.com/jankammerath/L475VG-IOT01A-Mbed-AWS-IoT
 */

void cloud_init();
void cloud_send(float distance);
int cloud_connect();


/*
    Listen for MQTT messages from the cloud
*/
void cloud_read(short subscribe);