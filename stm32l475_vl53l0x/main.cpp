#include <mbed.h>
#include "stm32l475e_iot01_dsensor.h"

#include "cloud/cloud.h"
#include "settings.h"
#include <string>

#define AWS_IOT_UPDATE_FREQUENCY    60

InterruptIn user_button(BUTTON1);


/* connect services */
int connect(){
    int result = 0;

    int cloud_status = cloud_connect();
    if(cloud_status == 1){
        result = 1;    
    }
    
    return result;
}

void send_reading_to_aws() {
    float distance = BSP_DSENSOR_GetReading();
    cloud_send(distance);
}

void get_data(SettingsInterface* settings)
{
    settings->get_sensor_data();
}

int main()
{
    printf("IoT device starting\n");

    // VL53L0X_Settings settings;
    // settings.mode = single;
    // BSP_DSENSOR_Init(settings);
    // unsigned int distance = 0;

    EventQueue* queue = mbed_event_queue();
    string sensors[] = {"temperature", "humidity"};
    Threshold temp_thres;
    temp_thres.max = 30.0;
    temp_thres.min = 5.0;
    Threshold humid_thres;
    humid_thres.min = 0.0;
    humid_thres.max = 20.0;
    Threshold thresholds[] = {temp_thres, humid_thres};
    SettingsInterface* settings = initialize_settings_test(sensors, 2, thresholds);

    queue->call_every(10s, get_data, settings);

    /* init cloud (connect wifi) */
    cloud_init();
    
    /* connect to network */
    int connected = connect();

    if(connected == 1){
        // EventQueue *queue = mbed_event_queue();

        user_button.fall(queue->event(send_reading_to_aws));
        printf("Attached button handler for message sending\n");
    } else {
        printf("Could not connect to network\n");
    }
}