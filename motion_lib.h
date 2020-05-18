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
  ERROR_MOTION_MOTOR_NOT_EXISTS,
  ERROR_MOTION_MOTOR_INDELIBLE,
  ERROR_MOTION_MEMORY,
  ERROR_MOTION_FULL_MOTORS,
  ERROR_MOTION_POINT_NOT_EXISTS
} tErrorMotion;               //type error of this class

typedef struct {
  int id;
  int pos_x;
  int pos_y;
} tPoint;

typedef struct {
  int nPoint;
  tPoint *point;
} tPointList;

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
    tPointList _pointsList;
    tMotorList _motorList;

    void _motorList_init(tMotorList *list);
    void _pointList_init(tPointList *list);
    tErrorMotion _motorExists(tMotorList list, int axis);
    tErrorMotion _pointExists(tPointList list, int id);
    void _cpyMotor(tMotor *dest, tMotor orgn);
    void _cpyPoint(tPoint *dets, tMotor orgn);

  public:
    Motion_lib();
    tErrorMotion addMotor(tMotorList *list, char axis);
    tErrorMotion removeMotor(tMotorList *list, char axis);
    tErrorMotion addPoint(tPointList *list, tPoint point);
    tErrorMotion removePoint(tPointList *list, int id);
    tErrorMotion stepPin(tMotorList *list, char axis, int pin = NULL);
    tErrorMotion dirPin(tMotorList *list, char axis, int pin = NULL);
    tErrorMotion pos_x(tPointList *list, int id, int pos_x = NULL);
    tErrorMotion pos_y(tPointList *list, int id, int pos_y = NULL);
  
};


#endif
