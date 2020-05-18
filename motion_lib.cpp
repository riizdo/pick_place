/*  file: motion_lib.cpp
 *  author: riizdo
 *  date: 16/05/2020
 *  version: 0.0
 *  description: code for motion library
 */

#include "Arduino.h"
#include "motion_lib.h"


Motion_lib::Motion_lib() {////////////////////////////////////////////CONSTRUCTOR---------------------------------------------------------
  _motorList_init(&_motorList);
  _pointList_init(&_pointsList);
}

void Motion_lib::_motorList_init(tMotorList *list) {//-----------             motorList_init----------------------------------------------
  free(list->motor);
  list->nMotor = 0;
}

void Motion_lib::_pointList_init(tPointList *list) {//-----                   pointList_init-----------------------------------------
  free(list->point);
  list->nPoint = 0;
}

tErrorMotion Motion_lib::addMotor(tMotorList *list, char axis) {//---         addMotor----------------------------------------------------
  if (list->nMotor == 0) {//if list is empty----------------------
    list->motor = (tMotor*) malloc(sizeof(tMotor));
  } else if (list->nMotor < 12) {//if list is not empty and not full
    for (int i = 0; i < list->nMotor; i++) {
      if (list->motor[i].axis == axis) {//if this motor exists
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

  list->motor[list->nMotor].axis = axis;//assign the parameters of the new motor
  if (list->nMotor == 0) {
    list->motor[list->nMotor].stepPin = DEFAULT_STEP_PIN;
    list->motor[list->nMotor].dirPin = DEFAULT_DIR_PIN;
  } else {
    list->motor[list->nMotor].stepPin = list->motor[list->nMotor - 1].stepPin + 1;
    list->motor[list->nMotor].dirPin = list->motor[list->nMotor -1].dirPin + 1;
  }
  list->nMotor++;
  return OK_MOTION;
}

tErrorMotion Motion_lib::removeMotor(tMotorList *list, char axis) {//---      removeMotor----------------------------------------------------------
  bool restore = true;

  if (axis == 'x' || axis == 'y') {
    return ERROR_MOTION_MOTOR_INDELIBLE;
  }

  if (_motorExists(*list, axis) != OK_MOTION) {
    return ERROR_MOTION_MOTOR_NOT_EXISTS;
  }
  
  for (int i = list->nMotor; i == 0; i--) {
    if (restore && list->motor[i].axis != axis) {
      list->motor[i - 1].axis = list->motor[i].axis;
      list->motor[i - 1].stepPin = list->motor[i].stepPin;
      list->motor[i - 1].dirPin = list->motor[i].dirPin;
    } else if (restore && list->motor[i].axis == axis) {
      list->motor[i - 1].axis = list->motor[i].axis;
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

tErrorMotion Motion_lib::_motorExists(tMotorList list, int axis) {//---        motorExists----------------------------------------------------
  for (int i = 0; i < list.nMotor; i++) {
    if (list.motor[i].axis == axis) {
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
  list->point[list->nPoint].pos_x = point.pos_x;
  list->point[list->nPoint].pos_y = point.pos_y;
  list->nPoint++;

  return OK_MOTION;
}

tErrorMotion Motion_lib::removePoint(tPointList *list, int id) {//------        removePoint---------------------------------------------------
  if (_pointExists(*list, id) == ERROR_MOTION_POINT_NOT_EXISTS) {
    return ERROR_MOTION_POINT_NOT_EXISTS;
  }
  
  for (int i = list->nPoint; i == 0; i--) {
    list->point[i].id = list->point[i -1].id;
    list->point[i].pos_x = list->point[i - 1].pos_x;
    list->point[i].pos_y = list->point[i - 1].pos_y;
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
