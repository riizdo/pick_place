/*  file: motion_lib.cpp
 *  author: riizdo
 *  date: 16/05/2020
 *  version: 0.0
 *  description: code for motion library
 */

#include "Arduino.h"
#include "motion_lib.h"


Motion_lib::Motion_lib() {////////////////////////////////////////////////////CONSTRUCTOR////////////////////////////////////////////////
  _motorList_init(&_motorList);
  _pointList_init(&_pointList);

  _error = addPoint(&_pointList, point);
  _error = addMotor(&_motorList, 'x');
  _error = addMotor(&_motorList, 'y');
}

void Motion_lib::_motorList_init(tMotorList *list) {//-----------             motorList_init----------------------------------------------
  free(list->motor);
  list->nMotor = 0;
}

void Motion_lib::_pointList_init(tPointList *list) {//-----                   pointList_init-----------------------------------------
  free(list->point);
  list->nPoint = 0;
}

tErrorMotion Motion_lib::addMotor(tMotorList *list, char letter) {//---         addMotor----------------------------------------------------
  if (list->nMotor == 0) {//if list is empty----------------------
    list->motor = (tMotor*) malloc(sizeof(tMotor));
  } else if (list->nMotor < 12) {//if list is not empty and not full
    for (int i = 0; i < list->nMotor; i++) {
      if (list->motor[i].letter == letter) {//if this motor exists
        return ERROR_MOTION_MOTOR_EXISTS;
      }
    }
    list->motor = (tMotor*) realloc(list->motor, (list->nMotor + 1) * sizeof(tMotor));
  } else {//if list is full
    return ERROR_MOTION_FULL_MOTORS;
  }

  if (list->motor == NULL) {//if not memory reserve
    return ERROR_MOTION_MEMORY;
  }

  list->motor[list->nMotor].letter = letter;//assign the parameters of the new motor
  if (list->nMotor == 0) {
    list->motor[list->nMotor].type = MOTOR_INTERFACE_TYPE;
    list->motor[list->nMotor].stepPin = DEFAULT_STEP_PIN;
    list->motor[list->nMotor].dirPin = DEFAULT_DIR_PIN;
  } else {
    list->motor[list->nMotor].type = MOTOR_INTERFACE_TYPE;
    list->motor[list->nMotor].stepPin = list->motor[list->nMotor - 1].stepPin + 1;
    list->motor[list->nMotor].dirPin = list->motor[list->nMotor -1].dirPin + 1;
  }
  list->nMotor++;
  return OK_MOTION;
}

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

tErrorMotion Motion_lib::addPoint(tPointList *list, tPoint point) {// ----      addPoint-------------------------------------------------------
  if (list->nPoint == 0) {
    list->point = (tPoint*) malloc(sizeof(tPoint));
  } else {
    list->point = (tPoint*) realloc(list->point, (list->nPoint + 1) * sizeof(tPoint));
  }

  if (list->point == NULL) {
    return ERROR_MOTION_MEMORY;
  }

  list->point[list->nPoint].id = point.id;
  list->point[list->nPoint].axis[0] = point.axis[0];
  list->point[list->nPoint].axis[1] = point.axis[1];
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

int Motion_lib::pointaxis_x(tPointList *list, int id, int axis_x = NULL) {//------  pointaxis_x------------------------------------------------------
  if (_pointExists(*list, id) == ERROR_MOTION_POINT_NOT_EXISTS) {
    return NULL;
  }

  for (int i = 0; i < list->nPoint; i++) {
    if (list->point[i].id == id) {
      if (axis_x == NULL) {
        return list->point[i].axis[0];
      } else {
        list->point[i].axis[0] = axis_x;
        return NULL;
      }//close if else
    }//close if
  }//close for
}//close method

int Motion_lib::pointaxis_y(tPointList *list, int id, int axis_y = NULL) {//-------   pointaxis_y----------------------------------------------
  if (_pointExists(*list, id) == ERROR_MOTION_POINT_NOT_EXISTS) {
    return NULL;
  }

  for (int i = 0; i < list->nPoint; i++) {
    if (list->point[i].id == id) {
      if (axis_y == NULL) {
        return list->point[i].axis[1];
      } else {
        list->point[i].axis[1] = axis_y;
        return NULL;
      }//close if else
    }//close if
  }//close for
}//close method

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
