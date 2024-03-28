#include <mbed.h>
#include "stm32l475e_iot01_dsensor.h"

#include "cloud/cloud.h"
#include "settings.h"
#include <string>

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
    cloud_read(0);
}

int main()
{
    printf("IoT device starting\n");

    EventQueue* queue = mbed_event_queue();

    /* init cloud (connect wifi) */
    int wifi_connected = cloud_init();

    if (wifi_connected != 0)
    {
        return -1;
    }
    
    /* connect to network */
    int connected = connect();

    if(connected == 1){

        // Subscribe to topic
        cloud_read(1);

        user_button.fall(queue->event(send_reading_to_aws));
        printf("Attached button handler for message sending\n");
    } else {
        printf("Could not connect to network\n");
        return -1;
    }
}