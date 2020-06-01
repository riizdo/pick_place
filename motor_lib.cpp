/*  file: motor_lib.cpp
 *  author: riizdo
 *  date: 30/05/2020
 *  version: 0.0
 *  description: code for motor library
 */


#include "Arduino.h"
#include "motor_lib.h"


Stepper_lib::Stepper_lib(int type, int stepPin, int dirPin, char axis):AccelStepper(type, stepPin, dirPin) {
  _type = type;
  _stepPin = stepPin;
  _dirPin = dirPin;
  _axis = axis;
}

char Stepper_lib::getAxis() {
  return _axis;
}
