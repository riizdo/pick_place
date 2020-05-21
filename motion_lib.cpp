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

  _error = addPoint();
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

  if (_axisExists(letter) == ERROR_MOTION_AXIS_EXISTS) {
    return ERROR_MOTION_AXIS_EXISTS;
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
  tErrorMotion error = _addAxis(letter);
  return OK_MOTION;
}//close of method

tErrorMotion Motion_lib::removeMotor(char letter) {//---      removeMotor----------------------------------------------------------
  bool restore = true;

  if (letter == 'x' || letter == 'y') {
    return ERROR_MOTION_MOTOR_INDELIBLE;
  }

  if (_motorExists(letter) != OK_MOTION) {
    return ERROR_MOTION_MOTOR_NOT_EXISTS;
  }
  
  for (int i = _motorList.nMotor; i == 0; i--) {
    if (restore && _motorList.motor[i].letter != letter) {
      _motorList.motor[i - 1].letter = _motorList.motor[i].letter;
      _motorList.motor[i - 1].stepPin = _motorList.motor[i].stepPin;
      _motorList.motor[i - 1].dirPin = _motorList.motor[i].dirPin;
    } else if (restore && _motorList.motor[i].letter == letter) {
      _motorList.motor[i - 1].letter = _motorList.motor[i].letter;
      _motorList.motor[i - 1].stepPin = _motorList.motor[i].stepPin;
      _motorList.motor[i - 1].dirPin = _motorList.motor[i].dirPin;
      restore = false;
    } 
  }
  _motorList.nMotor--;
  _motorList.motor = (tMotor*) realloc(_motorList.motor, _motorList.nMotor * sizeof(tMotor));

  if (_motorList.motor == NULL) {
    return ERROR_MOTION_MEMORY;
  }

  return OK_MOTION;
}

tErrorMotion Motion_lib::_motorExists(int letter) {//---        motorExists----------------------------------------------------
  for (int i = 0; i < _motorList.nMotor; i++) {
    if (_motorList.motor[i].letter == letter) {
      return OK_MOTION;
    }
  }

  return ERROR_MOTION_MOTOR_NOT_EXISTS;
}

tErrorMotion Motion_lib::addPoint() {// ----                    addPoint-------------------------------------------------------
  if (_pointList.nPoint == 0) {
    _pointList.point = (tPoint*) malloc(sizeof(tPoint));
  } else {
    _pointList.point = (tPoint*) realloc(_pointList.point, (_pointList.nPoint + 1) * sizeof(tPoint));
  }

  if (_pointList.point == NULL) {
    return ERROR_MOTION_MEMORY;
  }

  _pointList.point[_pointList.nPoint].id = _pointList.point[_pointList.nPoint - 1].id + 1;
  _pointList.nPoint++;

  return OK_MOTION;
}

tErrorMotion Motion_lib::removePoint(int id) {//------        removePoint---------------------------------------------------
  if (_pointExists(id) == ERROR_MOTION_POINT_NOT_EXISTS) {
    return ERROR_MOTION_POINT_NOT_EXISTS;
  }
  
  for (int i = _pointList.nPoint; i == 0; i--) {
    _pointList.point[i].id = _pointList.point[i -1].id;
    _pointList.point[i].axis[0] = _pointList.point[i - 1].axis[0];
    _pointList.point[i].axis[1] = _pointList.point[i - 1].axis[1];
    if (_pointList.point[i].id == id) {
      break;
    }
  }

  _pointList.nPoint--;
  _pointList.point = (tPoint*) realloc(_pointList.point, _pointList.nPoint * sizeof(tPoint));

  if (_pointList.point == NULL) {
    return ERROR_MOTION_MEMORY;
  }

  return OK_MOTION;
}

tErrorMotion Motion_lib::_pointExists(int id) {//--------       pointExists---------------------------------------------------
  for (int i = 0; i < _pointList.nPoint; i++) {
    if (_pointList.point->id == id) {
      return OK_MOTION;
    }
  }

  return ERROR_MOTION_POINT_NOT_EXISTS;
}

int Motion_lib::getStepPin(char letter) {//--------------         getStepPin---------------------------------------------------------
  if (_motorExists(letter) == ERROR_MOTION_MOTOR_NOT_EXISTS) {
    return NULL;
  }

  for (int i = 0; i < _motorList.nMotor; i++) {
    if (_motorList.motor[i].letter == letter) {
      return _motorList.motor[i].stepPin;
    }
  }
}

int Motion_lib::getDirPin(char letter) {//--------------           getDirPin----------------------------------------------------
  if (_motorExists(letter) == ERROR_MOTION_MOTOR_NOT_EXISTS) {
    return NULL;
  }

  for (int i = 0; i < _motorList.nMotor; i++) {
    if (_motorList.motor[i].letter == letter) {
      return _motorList.motor[i].dirPin;
    }
  }
}

tMotorList Motion_lib::getMotorList() {//-----------------------------              getMotorList-------------------------------------------
  return _motorList;
}

tPointList Motion_lib::getPointList() {//-----------------------------              getPointList-------------------------------------------
  return _pointList;
}

tErrorMotion Motion_lib::_axisExists(char axis) {//-------             axisExists---------------------------------------------
  for (int i = 0; i < _pointList.point[0].nAxis; i++) {
    if (_pointList.point[0].axis[i] == axis) {
      return OK_MOTION;
    }
  }

  return ERROR_MOTION_AXIS_NOT_EXISTS;
}

tErrorMotion Motion_lib::_addAxis(char axis) {//----          addAxis------------------------------------------------
  tErrorMotion error = OK_MOTION;
  
  if (_axisExists(axis) == ERROR_MOTION_AXIS_EXISTS) {
    return ERROR_MOTION_AXIS_EXISTS;
  }
  
  if (actualPosition.nAxis == 0) {
    actualPosition.axis = (char*) malloc(sizeof(char));
  } else {
    actualPosition.axis = (char*) realloc(actualPosition.axis, (actualPosition.nAxis + 1) * sizeof(char));
  }

  if (actualPosition.axis == NULL) {
    return ERROR_MOTION_MEMORY;
  }
  
  for (int i = 0; i < _pointList.nPoint; i++) {
    if (_pointList.point[i].nAxis == 0) {
      _pointList.point[i].axis = (char*) malloc(sizeof(char));
    } else {
      _pointList.point[i].axis = (char*) realloc(_pointList.point[i].axis, (_pointList.point[i].nAxis + 1) * sizeof (char));
    }

    if (_pointList.point[i].axis == NULL) {
      return ERROR_MOTION_MEMORY;
    }

    _pointList.point[i].axis[_pointList.point[i].nAxis] = axis;
    _pointList.point[i].nAxis++;
  }

  actualPosition.axis[actualPosition.nAxis] = axis;
  actualPosition.nAxis++;

  return OK_MOTION;
}

tErrorMotion Motion_lib::_removeAxis(char axis) {//--------   removeAxis----------------------------------------------------
  if (_axisExists(axis) == ERROR_MOTION_AXIS_NOT_EXISTS) {//check if the motor exists
    return ERROR_MOTION_AXIS_NOT_EXISTS;
  }

  for (int i = 0; i < _pointList.nPoint; i++) {
    if (_pointList.point[i].nAxis == 1) {
      free(_pointList.point[i].axis);
    } else {
      for (int e = _pointList.point[i].nAxis; e > 0; i++) {
        _pointList.point[i].axis[e - 1] = _pointList.point[i].axis[e];
        if (_pointList.point[i].axis[e - 1] == axis) {
          break;
        }//close if
      }//close for
      _pointList.point[i].axis = (char*) realloc(_pointList.point[i].axis, (_pointList.point[i].nAxis - 1) * sizeof(char));

      if (_pointList.point[i].axis == NULL) {
        return ERROR_MOTION_MEMORY;
      }
    }//close if else
  }//close for

  return OK_MOTION;
}//close method
