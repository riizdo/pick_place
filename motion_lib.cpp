/*  file: motion_lib.cpp
 *  author: riizdo
 *  date: 16/05/2020
 *  version: 0.0
 *  description: code for motion library
 */

#include "Arduino.h"
#include "motion_lib.h"


Motion_lib::Motion_lib() {////////////////////////////////////////////////////CONSTRUCTOR////////////////////////////////////////////////
  _motorList_init();
  _pointList_init();

  _error = addPoint(&_pointList);
  _error = addMotor('x');
  _error = addMotor('y');
}

void Motion_lib::_motorList_init() {//-----------             motorList_init----------------------------------------------
  free(_motorList.motor);
  _motorList.nMotor = 0;
}

void Motion_lib::_pointList_init() {//-----                   pointList_init-----------------------------------------
  free(_pointList.point);
  _pointList.nPoint = 0;
}

tErrorMotion Motion_lib::addMotor(char letter) {//---         addMotor----------------------------------------------------
  if (_motorList.nMotor == 0) {//if list is empty----------------------
    _motorList.motor = (tMotor*) malloc(sizeof(tMotor));
  } else if (_motorList.nMotor < 12) {//if list is not empty and not full
    for (int i = 0; i < _motorList.nMotor; i++) {
      if (_motorList.motor[i].letter == letter) {//if this motor exists
        return ERROR_MOTION_MOTOR_EXISTS;
      }
    }
    _motorList.motor = (tMotor*) realloc(_motorList.motor, (_motorList.nMotor + 1) * sizeof(tMotor));
  } else {//if list is full
    return ERROR_MOTION_FULL_MOTORS;
  }

  if (_motorList.motor == NULL) {//if not memory reserve
    return ERROR_MOTION_MEMORY;
  }

  _motorList.motor[_motorList.nMotor].letter = letter;//assign the parameters of the new motor
  if (_motorList.nMotor == 0) {
    _motorList.motor[_motorList.nMotor].type = MOTOR_INTERFACE_TYPE;
    _motorList.motor[_motorList.nMotor].stepPin = DEFAULT_STEP_PIN;
    _motorList.motor[_motorList.nMotor].dirPin = DEFAULT_DIR_PIN;
  } else {
    _motorList.motor[_motorList.nMotor].type = MOTOR_INTERFACE_TYPE;
    _motorList.motor[_motorList.nMotor].stepPin = _motorList.motor[_motorList.nMotor - 1].stepPin + 1;
    _motorList.motor[_motorList.nMotor].dirPin = _motorList.motor[_motorList.nMotor -1].dirPin + 1;
  }
  _motorList.nMotor++;
  tErrorMotion error = _addAxis(&_pointList, letter);
  return OK_MOTION;
}//close of method

tErrorMotion Motion_lib::removeMotor(tMotorList *list, char letter) {//---      removeMotor----------------------------------------------------------
  bool restore = true;

  if (letter == 'x' || letter == 'y') {
    return ERROR_MOTION_MOTOR_INDELIBLE;
  }

  if (_motorExists(*list, letter) != OK_MOTION) {
    return ERROR_MOTION_MOTOR_NOT_EXISTS;
  }
  
  for (int i = list->nMotor; i == 0; i--) {
    if (restore && list->motor[i].letter != letter) {
      list->motor[i - 1].letter = list->motor[i].letter;
      list->motor[i - 1].stepPin = list->motor[i].stepPin;
      list->motor[i - 1].dirPin = list->motor[i].dirPin;
    } else if (restore && list->motor[i].letter == letter) {
      list->motor[i - 1].letter = list->motor[i].letter;
      list->motor[i - 1].stepPin = list->motor[i].stepPin;
      list->motor[i - 1].dirPin = list->motor[i].dirPin;
      restore = false;
    } 
  }
  list->nMotor--;
  list->motor = (tMotor*) realloc(list->motor, list->nMotor * sizeof(tMotor));

  if (list->motor == NULL) {
    return ERROR_MOTION_MEMORY;
  }

  return OK_MOTION;
}

tErrorMotion Motion_lib::_motorExists(tMotorList list, int letter) {//---        motorExists----------------------------------------------------
  for (int i = 0; i < list.nMotor; i++) {
    if (list.motor[i].letter == letter) {
      return OK_MOTION;
    }
  }

  return ERROR_MOTION_MOTOR_NOT_EXISTS;
}

tErrorMotion Motion_lib::addPoint(tPointList *list) {// ----                    addPoint-------------------------------------------------------
  if (list->nPoint == 0) {
    list->point = (tPoint*) malloc(sizeof(tPoint));
  } else {
    list->point = (tPoint*) realloc(list->point, (list->nPoint + 1) * sizeof(tPoint));
  }

  if (list->point == NULL) {
    return ERROR_MOTION_MEMORY;
  }

  list->point[list->nPoint].id = list->point[list->nPoint - 1].id + 1;
  list->nPoint++;

  return OK_MOTION;
}

tErrorMotion Motion_lib::removePoint(tPointList *list, int id) {//------        removePoint---------------------------------------------------
  if (_pointExists(*list, id) == ERROR_MOTION_POINT_NOT_EXISTS) {
    return ERROR_MOTION_POINT_NOT_EXISTS;
  }
  
  for (int i = list->nPoint; i == 0; i--) {
    list->point[i].id = list->point[i -1].id;
    list->point[i].axis[0] = list->point[i - 1].axis[0];
    list->point[i].axis[1] = list->point[i - 1].axis[1];
    if (list->point[i].id == id) {
      break;
    }
  }

  list->nPoint--;
  list->point = (tPoint*) realloc(list->point, list->nPoint * sizeof(tPoint));

  if (list->point == NULL) {
    return ERROR_MOTION_MEMORY;
  }

  return OK_MOTION;
}

tErrorMotion Motion_lib::_pointExists(tPointList list, int id) {//--------       pointExists---------------------------------------------------
  for (int i = 0; i < list.nPoint; i++) {
    if (list.point->id == id) {
      return OK_MOTION;
    }
  }

  return ERROR_MOTION_POINT_NOT_EXISTS;
}

int Motion_lib::getStepPin(tMotorList list, char letter) {//--------------         getStepPin---------------------------------------------------------
  if (_motorExists(list, letter) == ERROR_MOTION_MOTOR_NOT_EXISTS) {
    return NULL;
  }

  for (int i = 0; i < list.nMotor; i++) {
    if (list.motor[i].letter == letter) {
      return list.motor[i].stepPin;
    }
  }
}

int Motion_lib::getDirPin(tMotorList list, char letter) {//--------------           getDirPin----------------------------------------------------
  if (_motorExists(list, letter) == ERROR_MOTION_MOTOR_NOT_EXISTS) {
    return NULL;
  }

  for (int i = 0; i < list.nMotor; i++) {
    if (list.motor[i].letter == letter) {
      return list.motor[i].dirPin;
    }
  }
}

tMotorList Motion_lib::getMotorList() {//-----------------------------              getMotorList-------------------------------------------
  return _motorList;
}

tPointList Motion_lib::getPointList() {//-----------------------------              getPointList-------------------------------------------
  return _pointList;
}

tErrorMotion Motion_lib::_axisExists(tPoint list, char axis) {//-------             axisExists---------------------------------------------
  for (int i = 0; i < list.nAxis; i++) {
    if (list.axis[i] == axis) {
      return OK_MOTION;
    }
  }

  return ERROR_MOTION_AXIS_NOT_EXISTS;
}

tErrorMotion Motion_lib::_addAxis(tPointList *list, char axis) {//----          addAxis------------------------------------------------
  if (_axisExists(list->point[0], axis) == ERROR_MOTION_AXIS_NOT_EXISTS) {
    return ERROR_MOTION_AXIS_NOT_EXISTS;
  }

  for (int i = 0; i < list->nPoint; i++) {
    if (list->point[i].nAxis == 0) {
      list->point[i].axis = (char*) malloc(sizeof(char));
    } else {
      list->point[i].axis = (char*) realloc(list->point[i].axis, (list->point[i].nAxis + 1) * sizeof (char));
    }

    if (list->point[i].axis == NULL) {
      return ERROR_MOTION_MEMORY;
    }

    list->point[i].axis[list->point[i].nAxis] = axis;
    list->point[i].nAxis++;
  }
}

int Motion_lib::getPosition(char axis) {
  
}
