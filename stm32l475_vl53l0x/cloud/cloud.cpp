/**
 * Majority of code taken from https://github.com/jankammerath/L475VG-IOT01A-Mbed-AWS-IoT
 */

#include "SocketAddress.h"
#define MQTTCLIENT_QOS1 0
#define MQTTCLIENT_QOS2 0

#include "mbed.h"
#include "ISM43362Interface.h"
#include "NTPClient.h"
#include "MQTTNetwork.h"
#include "MQTTmbed.h"
#include "MQTT_server_setting.h"
#include "MQTTClient.h"
#include "mbed-trace/mbed_trace.h"
#include "mbed_events.h"
#include "mbedtls/error.h"
#include <string>
#include "settings.h"

/*
    Private classes for MQTT handling
*/

#define MBED_CONF_APP_WIFI_SSID "SM-G930W88116"
#define MBED_CONF_APP_WIFI_PASSWORD "zggl7155"

#ifndef WIFI_INTERFACE_DEF
#define WIFI_INTERFACE_DEF
ISM43362Interface wifi;
#endif

MQTTNetwork* mqttNetwork = NULL;
MQTT::Client<MQTTNetwork, Countdown>* mqttClient = NULL;

static unsigned short mqtt_message_id = 0;
int call_id = -1;

/* forward declaration */
int cloud_connect(void);

/*
    Sync clock with NTP server
*/
void cloud_sync_clock(NetworkInterface* network){
    NTPClient ntp(network);
    
    /* use PTB server in Braunschweig */
    ntp.set_server("ptbtime1.ptb.de", 123);
    
    time_t now = ntp.get_timestamp();
    set_time(now);    
}

/*
    initialise cloud enviroment
*/
int cloud_init(){
    /* connect to the wifi network */
    int w_ret = wifi.connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if(w_ret == 0) {
        printf("WIFI is now connected\r\n");
    }else{
        printf("Failed to connect WIFI, %d\r\n", w_ret);
    }
    
    /* sync the clock */
    cloud_sync_clock(&wifi);    
    return w_ret;
}

/*
    Sends an MQTT message to the cloud
*/
void cloud_send(float value, string sensor, bool max)
{
    MQTT::Message message;
    message.retained = false;
    message.dup = false;
    
    const size_t buf_size = 512;
    char *buf = new char[buf_size];
    message.payload = (void*)buf;
    
    message.qos = MQTT::QOS0;
    message.id = mqtt_message_id++;
    int ret;
    if (max == true)
    {
        ret = snprintf(buf, buf_size, 
                "Warning! High value detected for sensor %s!\nSensor reading: %f\n", 
                sensor.c_str(), value);
    }
    else {
        ret = snprintf(buf, buf_size, 
                "Warning! Low value detected for sensor %s!\nSensor reading: %f\n", 
                sensor.c_str(), value);
    }
    if(ret < 0) {
        printf("ERROR: snprintf() returns %d.", ret);
    }
    message.payloadlen = ret;
    
    int rc = mqttClient->publish(MQTT_TOPIC_PUB, message);
    if(rc != MQTT::SUCCESS) {
        printf("ERROR: rc from MQTT publish is %d\r\n", rc);
        if (mqttClient->isConnected() == false)
        {
            printf("Connection status: %d\n", wifi.get_connection_status());
            printf("Reconnecting to MQTT\n");

            cloud_connect();
        }
    } 
    else 
    {
        printf("Message published to topic %s: %s\r\n", MQTT_TOPIC_PUB,buf);
    }
    delete[] buf; 
}

void get_data(SettingsInterface* settings)
{
    settings->get_sensor_data();
}


void test_handler(MQTT::MessageData& data)
{
    printf("Message was just received. Reading data:\n");
    MQTT::Message message = data.message;
    string* message_payload = static_cast<std::string *>(message.payload);
    printf("%s\n", message_payload->c_str());

    // const char* threshold_settings_message = message_payload->c_str();

    // create array of std::strings for input
    const char* token_string = message_payload->c_str();

    vector<string> settings_list;
    vector<Threshold> threshold_list;
 
    /* Packet: [humidity][temp][pressure][distance] */
    if (token_string[0] == '1')
    {
        settings_list.push_back("humidity");
        Threshold humid_thres;
        humid_thres.min = 0.0;
        humid_thres.max = 20.0;
        threshold_list.push_back(humid_thres);
    }
    if (token_string[1] == '1')
    {
        settings_list.push_back("temp");
        Threshold temp_thres;
        temp_thres.max = 30.0;
        temp_thres.min = 5.0;
        threshold_list.push_back(temp_thres);
    }
    if (token_string[2] == '1')
    {
        settings_list.push_back("pressure");
        Threshold pres_thres;
        pres_thres.max = 30.0;
        pres_thres.min = 5.0;
        threshold_list.push_back(pres_thres);
    }
    if (token_string[3] == '1')
    {
        settings_list.push_back("distance");
        Threshold dist_thres;
        dist_thres.min = 0.0;
        dist_thres.max = 200.0;
        threshold_list.push_back(dist_thres);    
    }

    string* settings = new string[settings_list.size()];
    std::copy(settings_list.begin(), settings_list.end(), settings);

    // printf("List we will push:\n");
    // for (int i = 0; i < settings_list.size(); i++)
    // {
    //     printf("%s\n",settings[i].c_str());
    // }

    Threshold* thresholds = new Threshold[threshold_list.size()];
    std::copy(threshold_list.begin(), threshold_list.end(), thresholds);
    SettingsInterface* display_settings = initialize_settings_test(settings, settings_list.size(), thresholds);

    EventQueue *queue = mbed_event_queue();
    if (call_id != -1)
    {
        queue->cancel(call_id);
    }
    call_id = queue->call_every(10s, get_data, display_settings);

    printf("\nCompleted reading payload data.\n");
}

/*
    Listen for MQTT messages from the cloud
*/
void cloud_read(short subscribe)
{
    if (subscribe)
    {
        int rc = mqttClient->subscribe(MQTT_TOPIC_SUB, MQTT::QOS0, test_handler);
        if (rc)
        {
            printf("Error subscribing, rc %d. Try restarting the device\n", rc);
            subscribe = 0;
        } 
        else 
        {
            printf("Subscribed successfully\n");
        }
    }
    else {
        int rc = mqttClient->subscribe(MQTT_TOPIC_SUB, MQTT::QOS0, test_handler);
    }
}

/*
    Connects to cloud using TLS socket
*/
int cloud_connect()
{
    int result = 0;
      
    /* create mqtt network instance */
    printf("Initialising MQTT network...\n");
    mqttNetwork = new MQTTNetwork(&wifi);

    SocketAddress addr;

    wifi.gethostbyname(MQTT_SERVER_HOST_NAME, &addr);
    addr.set_port(MQTT_SERVER_PORT);

    // get the network specific socket so we can open a connection with it
    mqttNetwork->get_socket()->open(&wifi);

    int rc = mqttNetwork->connect(addr, SSL_CA_PEM,
                SSL_CLIENT_CERT_PEM, SSL_CLIENT_PRIVATE_KEY_PEM);
    if (rc != MQTT::SUCCESS){
        printf("MQTT network not successful: %d\r\n", rc);
        return 0;
    }      
    
    /* create mqtt data packet */
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = (char *)MQTT_CLIENT_ID;
    data.username.cstring = (char *)MQTT_USERNAME;
    data.password.cstring = (char *)MQTT_PASSWORD;
    data.cleansession = true;
    // extra added to keep connection alive past 60 seconds
    // maximum short value
    data.keepAliveInterval = 65535;

    /* execute client and send data */
    mqttClient = new MQTT::Client<MQTTNetwork, Countdown>(*mqttNetwork);
    rc = mqttClient->connect(data);
    if (rc != MQTT::SUCCESS) {
        printf("MQTT client failed\r\n");
        return 0;
    }else{
        printf("MQTT connect successful\r\n");
        result = 1;
    }
    
    return result;
}
