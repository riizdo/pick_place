
/*  file: motion_lib.cpp
 *  author: riizdo
 *  date: 16/05/2020
 *  version: 0.0
 *  description: code for motion library
 */

#include "Arduino.h"
#include "motion_lib.h"


Motion_lib::Motion_lib(int reference) {
  _motorList_init();

  _actualPoint.id = 0;
}

void Motion_lib::_motorList_init() {
  _motorList.nMotor = 0;
}

tErrorMotion Motion_lib::_motorList_add(char axis) {
  if (_existAxis(axis) == OK_MOTION && _motorList.nMotor < MAX_MOTOR) {
    Stepper_lib stepper(MOTOR_INTERFACE_TYPE, DEFAULT_STEP_PIN + _motorList.nMotor, DEFAULT_DIR_PIN + _motorList.nMotor, axis);
    _motorList.motor[_motorList.nMotor] = &stepper;
    _motorList.nMotor++;
    return OK_MOTION;
  } else if (_motorList.nMotor >= MAX_MOTOR) {
    return ERROR_MOTION_FULL_MOTORS;
  } else {
    return ERROR_MOTION_MOTOR_EXISTS;
  }
}

bool Motion_lib::_existAxis(char axis) {
  for (int i = 0; i < _motorList.nMotor; i++) {
    if (axis == _motorList.motor[i]->getAxis()) {
      return true;
    }
  }

  return false;
}

tErrorMotion Motion_lib::_axisAdd(char axis) {
  if (_actualPoint.nAxis == 0) {
    _actualPoint.axis = (char*) malloc(sizeof(char));
    for (int i = 0; i < _pointList.nPoint; i++) {
      _pointList.point[i].axis = (char*) malloc(sizeof(char));
    }
  } else {
    _actualPoint.axis = (char*) realloc(_actualPoint.axis, (_actualPoint.nAxis + 1) * sizeof(char));
    for (int i = 0; i < _pointList.nPoint; i++) {
      _pointList.point[i].axis = (char*) realloc(_pointList.point[i].axis, (_pointList.point[i].nAxis + 1) * sizeof(char));
    }
  }

  
}
