/*  file: motion_lib.h
 *  author: riizdo
 *  date: 16/05/2020
 *  version: 0.0
 *  description: data and headers for motion library
 */

#ifndef motion_lib_h
#define motion_lib_h

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
  ERROR_MOTION_POINT_NOT_EXISTS,
  ERROR_MOTION_AXIS_NOT_EXISTS,
  ERROR_MOTION_AXIS_EXISTS
} tErrorMotion;                           //type error of this class

typedef enum {
  STOPPED,
  RUN,
  STANDBY
} tState;

typedef struct {                    //point type
  int id;
  int nAxis;
  char *axis;
  int *axisValue;
} tPoint;

typedef struct {
  int id;
  int nAxis;
  char *axis;
  int *distance;
  int *cadence;
} tTrayectory;

typedef struct {                    //point list type
  int nPoint;
  tPoint *point;
} tPointList;

typedef struct {                    //motor type
  int type;
  int stepPin;
  int dirPin;
  char letter;        //coordinate axis
} tMotor;

typedef struct {                    //motor list type
  int nMotor;
  tMotor *motor;
} tMotorList;

class Motion_lib {                  //motion class---------------------------------------------
   public:                           //public
    Motion_lib(char axis_1 = 'x', char axis_2 = 'y');//constructor

    tPoint actualPosition;//actual position of the util
    
    tErrorMotion addMotor(char letter);
    tErrorMotion removeMotor(char letter);
    tErrorMotion addPoint();
    tErrorMotion removePoint(int id);
    int getStepPin(char letter);
    int getDirPin(char letter);
    tMotorList getMotorList();
    tPointList getPointList();
    int getPosition(char axis);//get de value of axis position
    void listener(tState state);
    tErrorMotion start(tState state);
    void setPointId(int id);
    int getPointId();
     
  private:                          //private
    tState _state;//state of the object
    tPointList _pointList;//point of coordinates list
    tMotorList _motorList;//motor of axis of coordinates list
    tErrorMotion _error;//error de motion
    tTrayectory _trayectory;//trayectory for move to point

    void _motorList_init();
    void _pointList_init();
    tErrorMotion _motorExists(int letter);
    tErrorMotion _pointExists(int id);
    tErrorMotion _axisExists(char axis);
    tErrorMotion _addAxis(char axis);
    tErrorMotion _removeAxis(char axis);
    void _cpyMotor(tMotor *dest, tMotor orgn);
    void _cpyPoint(tPoint *dets, tMotor orgn);
};//close of class

#endif
