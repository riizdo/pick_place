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
  if (list->nMotor == 0) {
    list->motor = (tMotor*) malloc(sizeof(tMotor));
  } else if (list->nMotor < 12) {
    for (int i = 0; i < list->nMotor; i++) {
      if (list->motor[i].axis == axis) {
        return ERROR_MOTION_MOTOR_EXISTS;
      }
    }
    list->motor = (tMotor*) realloc(list->motor, (list->nMotor + 1) * sizeof(tMotor));
  } else {
    return ERROR_MOTION_FULL_MOTORS;
  }

  if (list->motor == NULL) {
    return ERROR_MOTION_MEMORY;
  }

  list->motor[list->nMotor].axis = axis;
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
