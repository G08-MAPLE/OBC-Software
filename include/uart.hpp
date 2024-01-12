#ifndef UART_config_H
#define UART_config_H

class UARTController {
// This class will contain all the functions necessary to configure the ESP32 UART and all functions related to 
// sending messages through the XBEE modules.
private:
    int _sendData(const char* logName, const char* data);

public:
    static const char *TAG;
    static const int RX_BUF_SIZE;

    UARTController();
    void config(void);
};

#endif