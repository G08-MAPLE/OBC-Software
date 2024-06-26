#ifndef UART_config_H
#define UART_config_H

#include <stdint.h>

class UARTController {
// This class will contain all the functions necessary to configure the ESP32 UART and all functions related to 
// sending messages through the XBEE modules.
private:
    // static const int RX_BUF_SIZE = 1024;
    int _sendData(const char* logName, const char* data);
    void _parseData(uint8_t* data);
    int _msgDecision(uint8_t* msgData, uint8_t* expectedMsg, int msgLen);

public:
    UARTController();
    void config();
    void XBEE_tx(uint8_t* dataTx, int len);
    void XBEE_digi_static_tx(int responseType);
    void XBEE_rx();
    
};

#endif