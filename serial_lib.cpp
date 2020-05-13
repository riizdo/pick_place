/*  file: serial_lib.cpp
 *  author: riizdo
 *  date: 12/05/2020
 *  version: 0.0
 *  description: cpp file for serial_lib 
 */

#include "Arduino.h"
#include "serial_lib.h"

Serial_lib::Serial_lib(int port){
  _port = port;
}

void Serial_lib::init() {
  switch (_port) {
    case 0:
    Serial.begin(_BAUDRATE);
    break;
    case 1:
    Serial1.begin(_BAUDRATE);
    break;
    case 2:
    Serial2.begin(_BAUDRATE);
    break;
    case 3:
    Serial3.begin(_BAUDRATE);
    break;
    
  }
}

void Serial_lib::listener() {
  int n = 0;
  switch (_port) {
    case 0:
    n = Serial.available();
    _listInit(_dataList);
    
  }
}

void Serial_lib::_listInit(int *list) {
  free(list);
}
