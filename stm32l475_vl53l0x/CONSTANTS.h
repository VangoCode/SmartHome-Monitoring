#ifndef _CONSTANTS_H
#define _CONSTANTS_H

/*
 * SENSOR THRESHOLDS
 */
float HUMIDITY_MINIMUM_THRESHOLD = 0.0;
float HUMIDITY_MAXIMUM_THRESHOLD = 20.0;
float TEMPERATURE_MINIMUM_THRESHOLD = 5.0;
float TEMPERATURE_MAXIMUM_THRESHOLD = 30.0;
float PRESSURE_MINIMUM_THRESHOLD = 30.0;
float PRESSURE_MAXIMUM_THRESHOLD = 5.0;
float DISTANCE_MINIMUM_THRESHOLD = 200.0;
float DISTANCE_MAXIMUM_THRESHOLD = 600.0;


/*
 * WIFI CONFIG
 */
#define MBED_CONF_APP_WIFI_SSID "..."
#define MBED_CONF_APP_WIFI_PASSWORD "..."



/*
 * MQTT CONFIG
 */
const char MQTT_SERVER_HOST_NAME[] = "....iot.us-east-2.amazonaws.com";
const char MQTT_TOPIC_PUB[] = "csc385-demo";
const char MQTT_TOPIC_SUB[] = "csc385-demo-subscription";

/*
 * Root CA certificate in PEM format
 * "-----BEGIN CERTIFICATE-----\n"
 * ...
 * "-----END CERTIFICATE-----\n";
 */
/* AmazonRootCA1.pem */
const char* SSL_CA_PEM = "-----BEGIN CERTIFICATE-----\n"
"..."
"-----END CERTIFICATE-----\n";

/*
 * (optional, set NULL if not use) Client certificate in PEM format
 * "-----BEGIN CERTIFICATE-----\n"
 * ...
 * "-----END CERTIFICATE-----\n";
 */
const char* SSL_CLIENT_CERT_PEM = "-----BEGIN CERTIFICATE-----\n"
"..."
"-----END CERTIFICATE-----\n";


/*
 * (optional, set NULL of not use) Client private key in PEM format
 * "-----BEGIN RSA PRIVATE KEY-----\n"
 * ...
 * "-----END RSA PRIVATE KEY-----\n";
 */
const char* SSL_CLIENT_PRIVATE_KEY_PEM = "-----BEGIN RSA PRIVATE KEY-----\n"
"..."
"-----END RSA PRIVATE KEY-----\n";

#endif