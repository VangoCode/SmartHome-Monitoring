#ifndef __MQTT_SERVER_SETTING_H__
#define __MQTT_SERVER_SETTING_H__

const char MQTT_SERVER_HOST_NAME[] = "....iot.us-east-2.amazonaws.com";
const char MQTT_CLIENT_ID[] = "STM32L475VGIOT01A";
const char MQTT_USERNAME[] = "STM32L4";
const char MQTT_PASSWORD[] = "";
const char MQTT_TOPIC_PUB[] = "pubtopic";
const char MQTT_TOPIC_SUB[] = "subtopic";


const int MQTT_SERVER_PORT = 8883;

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

#endif /* __MQTT_SERVER_SETTING_H__ */
