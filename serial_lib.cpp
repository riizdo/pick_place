/*  file: serial_lib.cpp
 *  author: riizdo
 *  date: 12/05/2020
 *  version: 0.0
 *  description: cpp file for serial_lib 
 */

#include "Arduino.h"
#include "serial_lib.h"

Serial_lib::Serial_lib(int port, int und = 0){          //the constructor of class///////////////////////////////////////////
  _station = und;
  _empty = true;
  _listInit(&_dataList);
  if (port >= 0 && port <= 4) {
    _port = port;
    _error = OK_SERIAL;
  } else {
    _port = NULL;
    _error = ERROR_SERIAL_PORT_ASSIGNED;
  }
}

void Serial_lib::init() {                               //the init of class
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

void Serial_lib::listener() {                           //the method listener
  if (_empty) {
    _listInit(&_dataList);//initialize the data list of the transmission
  }
  
  switch (_port) {
    case 0:
    _readBuffer(&_dataList, Serial.available());
    break;
    case 1:
    _readBuffer(&_dataList, Serial1.available());
    break;
    case 2:
    _readBuffer(&_dataList, Serial2.available());
    break;
    case 3:
    _readBuffer(&_dataList, Serial3.available());
    break;
  }
}

void Serial_lib::_listInit(tDataList *list) {
  free(list->element);
  list->nElement = 0;
  _empty = true;
}

void Serial_lib::_readBuffer(tDataList *list, int n) {
  if (list->nElement == 0) {
    list->element = (int*) malloc(n * sizeof(int));
  } else {
    list->element = (int*) realloc(list->element, (list->nElement + n) *sizeof(int));
  }
  for (int i = list->nElement; i < n; i++) {
    switch (_port) {
      case 0:
      list->element[i] = Serial.read();
      break;
      case 1:
      list->element[i] = Serial1.read();
      break;
      case 2:
      list->element[i] = Serial2.read();
      break;
      case 3:
      list->element[i] = Serial3.read();
      break;
    }
    list->nElement++;
    _empty = false;
  }
}

void Serial_lib::show() {
  Serial.print("El puerto seleccionado: ");
  Serial.println(_port);
  Serial.print("La estacion seleccionada: ");
  Serial.println(_station);
}

int Serial_lib::port(int port = NULL) {
  if (port == NULL) {
    return _port;
  } else if (port >= 0 && port <= 4) {
    _port = port;
    _error = OK_SERIAL;
    return NULL;
  } else {
    _error = ERROR_SERIAL_PORT_ASSIGNED;
    return NULL;
  }
}

int Serial_lib::station(int station = NULL) {
  if (station = NULL) {
    return _station;
  } else {
    _station = station;
    return NULL;
  }
}
