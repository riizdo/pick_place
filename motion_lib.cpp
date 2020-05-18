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
  _coordinateList_init(&_pointsList);
}

void Motion_lib::_motorList_init(tMotorList *list) {//-----------             motorList_init----------------------------------------------
  free(list->motor);
  list->nMotor = 0;
}

void Motion_lib::_coordinateList_init(tCoordinateList *list) {//-----         coordinateList_init-----------------------------------------
  free(list->pos);
  list->nPos = 0;
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

  if (_motorExists(list, axis) != OK_MOTION) {
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

tErrorMotion Motion_lib::_motorExists(tMotorList *list, int axis) {
  for (int i = 0; i < list->nMotor; i++) {
    if (list->motor[i].axis == axis) {
      return OK_MOTION;
    }
  }

  return ERROR_MOTION_MOTOR_NOT_EXISTS;
}
