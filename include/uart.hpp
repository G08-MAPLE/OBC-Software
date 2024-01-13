#ifndef UART_config_H
#define UART_config_H

static const char *UART_TAG = "UART_Controller";

class UARTController {
// This class will contain all the functions necessary to configure the ESP32 UART and all functions related to 
// sending messages through the XBEE modules.
private:
    // static const int RX_BUF_SIZE = 1024;
    int _sendData(const char* logName, const char* data);

public:
    UARTController();
    void config(void);
    void XBEE_tx();
};

#endif