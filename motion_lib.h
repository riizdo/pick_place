/*  file: motion_lib.h
 *  author: riizdo
 *  date: 16/05/2020
 *  version: 0.0
 *  description: data and headers for motion library
 */

#ifndef motion_h
#define motion_h

#include "Arduino.h"
#include <AccelStepper.h>

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
  ERROR_MOTION_POINT_NOT_EXISTS,
  ERROR_MOTION_AXIS_NOT_EXISTS
} tErrorMotion;               //type error of this class

typedef struct {
  int id;
  int nAxis;
  char *axis;
} tPoint;

typedef struct {
  int nPoint;
  tPoint *point;
} tPointList;

typedef struct {
  int type;
  int stepPin;
  int dirPin;
  char letter;        //coordinate axis
} tMotor;

typedef struct {
  int nMotor;
  tMotor *motor;
} tMotorList;

class Motion_lib {
  private:
    tPointList _pointList;
    tMotorList _motorList;
    tErrorMotion _error;

    void _motorList_init(tMotorList *list);
    void _pointList_init(tPointList *list);
    tErrorMotion _motorExists(tMotorList list, int letter);
    tErrorMotion _pointExists(tPointList list, int id);
    tErrorMotion _axisExists(tPoint list, char axis);
    tErrorMotion _addAxis(tPointList *list, char axis);
    tErrorMotion _removeAxis(tPointList *list, char axis);
    void _cpyMotor(tMotor *dest, tMotor orgn);
    void _cpyPoint(tPoint *dets, tMotor orgn);

  public:
    Motion_lib();
    tErrorMotion addMotor(tMotorList *list, char letter);
    tErrorMotion removeMotor(tMotorList *list, char letter);
    tErrorMotion addPoint(tPointList *list, tPoint point);
    tErrorMotion removePoint(tPointList *list, int id);
    int getStepPin(tMotorList list, char letter);
    int getDirPin(tMotorList list, char letter);
    int pointaxis_x(tPointList *list, int id, int axis_x = NULL);
    int pointaxis_y(tPointList *list, int id, int axis_y = NULL);
    tMotorList getMotorList();
    tPointList getPointList();
  
};


#endif
