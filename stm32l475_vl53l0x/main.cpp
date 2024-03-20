#include <mbed.h>
// #include "stm32l475e_iot01_dsensor.h"

// int main()
// {
//     VL53L0X_Settings settings;
//     settings.mode = single;
//     BSP_DSENSOR_Init(settings);

//     while (1)
//     {
//         printf("%u\n", BSP_DSENSOR_GetReading());
//         HAL_Delay(500);
//     }
// }

#define AT_HOME
#ifdef AT_HOME
char* WIFI_USERNAME = "";
char* WIFI_PASSWORD = "";
#else
char* WIFI_USERNAME = "";
char* WIFI_PASSWORD = "";
#endif


#include "ISM43362Interface.h"
ISM43362Interface wifi;

const char *sec2str(nsapi_security_t sec)
{
    switch (sec) {
        case NSAPI_SECURITY_NONE:
            return "None";
        case NSAPI_SECURITY_WEP:
            return "WEP";
        case NSAPI_SECURITY_WPA:
            return "WPA";
        case NSAPI_SECURITY_WPA2:
            return "WPA2";
        case NSAPI_SECURITY_WPA_WPA2:
            return "WPA/WPA2";
        case NSAPI_SECURITY_UNKNOWN:
        default:
            return "Unknown";
    }
}

int scan_demo(WiFiInterface *wifi)
{
    WiFiAccessPoint *ap;

    printf("Scan:\n");

    int count = wifi->scan(NULL,0);

    if (count <= 0) {
        printf("scan() failed with return value: %d\n", count);
        return 0;
    }

    /* Limit number of network arbitrary to 15 */
    count = count < 15 ? count : 15;

    ap = new WiFiAccessPoint[count];
    count = wifi->scan(ap, count);

    if (count <= 0) {
        printf("scan() failed with return value: %d\n", count);
        return 0;
    }

    for (int i = 0; i < count; i++) {
        printf("Network: %s secured: %s BSSID: %hhX:%hhX:%hhX:%hhx:%hhx:%hhx RSSI: %hhd Ch: %hhd\n", ap[i].get_ssid(),
               sec2str(ap[i].get_security()), ap[i].get_bssid()[0], ap[i].get_bssid()[1], ap[i].get_bssid()[2],
               ap[i].get_bssid()[3], ap[i].get_bssid()[4], ap[i].get_bssid()[5], ap[i].get_rssi(), ap[i].get_channel());
    }
    printf("%d networks available.\n", count);

    delete[] ap;
    return count;
}

int main()
{
    SocketAddress a;
    printf("WiFi example\n");

#ifdef MBED_MAJOR_VERSION
    printf("Mbed OS version %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif

    int count = scan_demo(&wifi);
    if (count == 0) {
        printf("No WIFI APs found - can't continue further.\n");
        return -1;
    }

    printf("\nConnecting to %s...\n", WIFI_USERNAME);
    int ret = wifi.connect(WIFI_USERNAME, WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
        printf("\nConnection error: %d\n", ret);
        return -1;
    }

    printf("Success\n\n");
    printf("MAC: %s\n", wifi.get_mac_address());
    wifi.get_ip_address(&a);
    printf("IP: %s\n", a.get_ip_address());
    wifi.get_netmask(&a);
    printf("Netmask: %s\n", a.get_ip_address());
    wifi.get_gateway(&a);
    printf("Gateway: %s\n", a.get_ip_address());
    printf("RSSI: %d\n\n", wifi.get_rssi());

    wifi.disconnect();

    printf("\nDone\n");
}