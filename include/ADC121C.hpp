#ifndef ADC121C_ft
#define ADC121C_ft

#include "Arduino.h"
#include "Wire.h"

class ADC121C{
  public:
    ADC121C(uint8_t adr);
    // Initiate a ADC121XXX
    void begin(TwoWire &wire = Wire);
    // Read the ADC conversion value from the device, with alert condition overload return.
    unsigned int readConversion(bool &boo); //optional reference
    // Read the ADC conversion value from the device.
    unsigned int readConversion(); //overloaded

    // setting stuffs
    // 2 bytes config

    // Get low limit value.
    unsigned int getLowLimit();
    // Set low limit value.
    void setLowLimit(uint16_t val);
    // Get high limit value.
    unsigned int getHighLimit();
    // Set high limit value.
    void setHighLimit(uint16_t val);
    // Get alert hysteresis value.
    unsigned int getAlertHyst();
    // Set alert hysteresis value.
    void setAlertHyst(uint16_t val);
    // Get the lowest conversion result when in automatic conversion mode.
    unsigned int getLowConv();
    // Set the lowest conversion result.
    void setLowConv(uint16_t val);
    // Get the highest conversion result when in automatic conversion mode.
    unsigned int getHighConv();
    // Set the highest conversion result.
    void setHighConv(uint16_t val);
    
    // alert status config

    // Get the alert status register value.
    uint8_t getAlertStatus();
    // Configure the alert status register.
    void setAlertStatus(uint8_t val);

    // .. configuration register

    // Configure the polarity output of ALERT pin.
    void setPolarity(bool val);
    // Enable or disable the ALERT pin.
    void setAlertPinEnable(bool val);
    // Enable or disable the ALERT flag in conversion result.
    void setAlertFlagEnable(bool val);
    // Enable or disable the ALERT hold logic in alert status register.
    void setAlertHold(bool val);
    // Configure automatic conversion mode.
    void setCycleTime(uint8_t val);
    // Check a configure.
    int8_t checkConfig(uint8_t op);
    
  private:
    TwoWire *_wire;
    uint8_t _adr;
    uint16_t readRegister(uint8_t reg); //some register returns two bytes
    void writeRegister(uint8_t reg, uint16_t val); //some register needs only one byte
};

#endif