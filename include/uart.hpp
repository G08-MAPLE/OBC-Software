#ifndef UART_config_H
#define UART_config_H

static const char *UART_TAG = "UART_Controller";

class UARTController {
// This class will contain all the functions necessary to configure the ESP32 UART and all functions related to 
// sending messages through the XBEE modules.
private:
    // static const int RX_BUF_SIZE = 1024;
    int _sendData(const char* logName, const char* data);
    void _parseData(uint8_t* data);

public:
    UARTController();
    void config();
    void XBEE_tx(const char* dataTx);
    void XBEE_digi_tx();
    void XBEE_rx();
    
};

#endif