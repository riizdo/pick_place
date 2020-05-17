/*  file: motion_lib.h
 *  author: riizdo
 *  date: 16/05/2020
 *  version: 0.0
 *  description: data and headers for motion library
 */

#ifndef motion_h
#define motion_h

#include "Arduino.h"

#define MOTOR_INTERFACE_TYPE 1
#define DEFAULT_STEP_PIN 2
#define DEFAULT_DIR_PIN 22

typedef enum {
  OK_MOTION,
  ERROR_MOTION_MOTOR_EXISTS,
  ERROR_MOTION_MEMORY,
  ERROR_MOTION_FULL_MOTORS
} tErrorMotion;               //type error of this class

typedef struct {
  int pos_x;
  int pos_y;
} tCoordinate;

typedef struct {
  int nPos;
  tCoordinate *pos;
} tCoordinateList;

typedef struct {
  int type;
  int stepPin;
  int dirPin;
  char axis;        //coordinate axis
} tMotor;

typedef struct {
  int nMotor;
  tMotor *motor;
} tMotorList;

class Motion_lib {
  private:
    tCoordinateList _pointsList;
    tMotorList _motorList;

    void _motorList_init(tMotorList *list);
    void _coordinateList_init(tCoordinateList *list);

  public:
    Motion_lib();
    tErrorMotion addMotor(tMotorList *list, char axis);
    tErrorMotion removeMotor(tMotorList *list, char axis);
    tErrorMotion addPoint(tCoordinateList *list, int x, int y);
    tErrorMotion removePoint(tCoordinateList *list, int x, int y);
    void stepPin(tMotorList *list, char axis, int pin = NULL);
    void dirPin(tMotorList *list, char axis, int pin = NULL);
  
};


#endif
