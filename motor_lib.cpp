/*  file: motor_lib.cpp
 *  author: riizdo
 *  date: 30/05/2020
 *  version: 0.0
 *  description: code for motor library
 */


#include "Arduino.h"
#include "motor_lib.h"


Motor_lib::Motor_lib(int type, int stepPin, int dirPin, char axis):AccelStepper(type, stepPin, dirPin) {
  _type = type;
  _stepPin = stepPin;
  _dirPin = dirPin;
  _axis = axis;
}

char Motor_lib::getAxis() {
  return _axis;
}
