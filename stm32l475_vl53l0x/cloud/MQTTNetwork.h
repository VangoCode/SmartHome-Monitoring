/**
 * Majority of code taken from https://github.com/jankammerath/L475VG-IOT01A-Mbed-AWS-IoT
 */

#ifndef _MQTTNETWORK_H_
#define _MQTTNETWORK_H_

#include "NetworkInterface.h"

#include "SocketAddress.h"
#include "TLSSocket.h"
#include "ISM43362Interface.h"

class MQTTNetwork {
public:
    MQTTNetwork(NetworkInterface* aNetwork) : network(aNetwork) {
        socket = new TLSSocket();
    }

    ~MQTTNetwork() {
        delete socket;
    }

    int read(unsigned char* buffer, int len, int timeout) {
        nsapi_size_or_error_t rc = 0;
        socket->set_timeout(timeout);
        rc = socket->recv(buffer, len);
        if (rc == NSAPI_ERROR_WOULD_BLOCK){
            // time out and no data
            // MQTTClient.readPacket() requires 0 on time out and no data.
            return 0;
        }
        return rc;
    }

    int write(unsigned char* buffer, int len, int timeout) {
        // TODO: handle time out
        return socket->send(buffer, len);
    }

    /** Expose the MQTT network socket
     *
     * @author Ron Varshavsky
     */
    TLSSocket* get_socket()
    {
        return socket;
    }

    int connect(SocketAddress socket_addr, const char *ssl_ca_pem = NULL,
            const char *ssl_cli_pem = NULL, const char *ssl_pk_pem = NULL) {  
        socket->set_root_ca_cert(ssl_ca_pem);
        socket->set_client_cert_key(ssl_cli_pem, ssl_pk_pem);
        return socket->connect(socket_addr);
    }

    int disconnect() {
        return socket->close();
    }

private:
    NetworkInterface* network;
    TLSSocket* socket;
};

#endif // _MQTTNETWORK_H_
