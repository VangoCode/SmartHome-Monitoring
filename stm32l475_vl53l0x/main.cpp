#include <mbed.h>
#include "stm32l475e_iot01_dsensor.h"

#include "cloud/cloud.h"

#define AWS_IOT_UPDATE_FREQUENCY    60

InterruptIn user_button(BUTTON1);
int subscribe = 0;

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
    if (subscribe < 2)
    {
        subscribe = !subscribe;
        printf("Running subscribe! Will we subscribe?: %d\n", subscribe);
        cloud_read(subscribe);
    }
    if (subscribe == 0)
    {
        subscribe = 2;
    }
    if (subscribe == 2)
    {
        float distance = BSP_DSENSOR_GetReading();
        cloud_send(distance);
    }
}


int main()
{
    printf("IoT device starting\n");

    VL53L0X_Settings settings;
    settings.mode = single;
    BSP_DSENSOR_Init(settings);
    // unsigned int distance = 0;


    /* init cloud (connect wifi) */
    cloud_init();
    
    /* connect to network */
    int connected = connect();

    if(connected == 1){
        EventQueue *queue = mbed_event_queue();

        user_button.fall(queue->event(send_reading_to_aws));
        printf("Attached button handler for message sending\n");
    } else {
        printf("Could not connect to network\n");
    }
}