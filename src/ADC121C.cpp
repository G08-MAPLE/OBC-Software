#include "ADC121C.hpp"

ADC121C::ADC121C(uint8_t adr){
  _adr = adr;
}

void ADC121C::begin(TwoWire &wire){
  _wire = &wire;
  _wire->begin();
}

unsigned int ADC121C::readConversion(bool &boo){
  // return value AND check for alert flag
  uint16_t data = readRegister(0x00);
  if(data >= 32768){ //caught condition flag
    bitWrite(data,15,0); //set that bit to zero since it isn't needed
    boo = true; //alert has been raised
  } else {
    boo = false; //alert hasn't been raised yet
  }
  return data;
}

unsigned int ADC121C::readConversion(){
  uint16_t data = readRegister(0x00);
  if(data >= 32768){ //caught condition flag
    bitWrite(data,15,0); //set that bit to zero since it isn't needed
  }
  return data;
}

// get set blah blah blah
// for your convenient sake
// 2 bytes config
unsigned int ADC121C::getLowLimit(){
  return readRegister(0x03);
}

void ADC121C::setLowLimit(uint16_t val){
  if(val > 4095)
    val = 4095;
  writeRegister(0x03,val);
}

unsigned int ADC121C::getHighLimit(){
  return readRegister(0x04);
}

void ADC121C::setHighLimit(uint16_t val){
  if(val > 4095)
    val = 4095;
  writeRegister(0x04,val);
}

unsigned int ADC121C::getAlertHyst(){
  return readRegister(0x05);
}

void ADC121C::setAlertHyst(uint16_t val){
  if(val > 4095)
    val = 4095;
  writeRegister(0x05,val);
}

unsigned int ADC121C::getLowConv(){
  return readRegister(0x06);
}

void ADC121C::setLowConv(uint16_t val){
  if(val > 4095)
    val = 4095;
  writeRegister(0x06,val);
}

unsigned int ADC121C::getHighConv(){
  return readRegister(0x07);
}

void ADC121C::setHighConv(uint16_t val){
  if(val > 4095)
    val = 4095;
  writeRegister(0x07,val);
}

// 1 byte config
// alert status reg
uint8_t ADC121C::getAlertStatus(){
  return readRegister(0x01);
}

void ADC121C::setAlertStatus(uint8_t val){
  if(val <= 3) //dont write the value IF it goes beyond 0x03
    writeRegister(0x01,val);
}

// configurations
// for your convenience sake..
void ADC121C::setPolarity(bool val){
  uint8_t data = readRegister(0x02);
  bitWrite(data,0,val);
  writeRegister(0x02,data);
}

void ADC121C::setAlertPinEnable(bool val){
  uint8_t data = readRegister(0x02);
  bitWrite(data,2,val);
  writeRegister(0x02,data);
}

void ADC121C::setAlertFlagEnable(bool val){
  uint8_t data = readRegister(0x02);
  bitWrite(data,3,val);
  writeRegister(0x02,data);
}

void ADC121C::setAlertHold(bool val){
  uint8_t data = readRegister(0x02);
  bitWrite(data,4,val);
  writeRegister(0x02,data);
}

void ADC121C::setCycleTime(uint8_t val){
  uint8_t data = readRegister(0x02), bitIndex = 0;
  for(uint8_t i = 5; i < 8; i++){
    bitWrite(data,i,bitRead(val,bitIndex));
    bitIndex++;
  }
  writeRegister(0x02,data);
}

// config reg.. oh damn

int8_t ADC121C::checkConfig(uint8_t op){
  uint8_t data = readRegister(0x02);
  
  if(op > 7)
    return -1; //invalid option

  if(op == 1) //dogma
    return -1; 

  if(op > 4){
    return data >> 5; // shift 5 times
  } else {
    return bitRead(data,op);
  }
}

uint16_t ADC121C::readRegister(uint8_t reg){
  uint8_t data[1];
  _wire->beginTransmission(_adr);
  _wire->write(reg);
  _wire->endTransmission(false);
  _wire->requestFrom(_adr,uint8_t(2));
  data[0] = Wire.read();
  data[1] = Wire.read();
  _wire->endTransmission();
  if(reg == 0x01 || reg == 0x02){ //for one byte register
    return data[0];
  } else {
    return (data[0] << 8) | data[1];
  }
}

void ADC121C::writeRegister(uint8_t reg, uint16_t val){
  uint8_t data[1];
  data[0] = val; //this will only read lsb byte
  data[1] = val >> 8; //this will only read msb byte
  _wire->beginTransmission(_adr);
  _wire->write(reg);
  if(reg != 0x01 || reg != 0x02){
    _wire->write(data[1]); //start with msb
  }
  _wire->write(data[0]); //end with lsb or write only single byte
  _wire->endTransmission();
}