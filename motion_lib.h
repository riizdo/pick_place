/*  file: motion_lib.h
 *  author: riizdo
 *  date: 16/05/2020
 *  version: 0.0
 *  description: data and headers for motion library
 */

#ifndef motion_lib_h
#define motion_lib_h

#include "Arduino.h"
#include "motor_lib.h"

#define MOTOR_INTERFACE_TYPE 1
#define DEFAULT_STEP_PIN 2
#define DEFAULT_DIR_PIN 22
#define MAX_MOTOR 10

/////////////////////////////////////////////////////////////////////TYPES//////////////////////////////////////////////////////////////
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

typedef struct {                    //trayectory type
  int id;
  int nAxis;
  char *axis;
  int *distance;
  int *cadence;
} tTrayectory;

typedef struct {                    //trayectory list type
  int nTrayctory;
  tTrayectory *trayectory;
} tTrayectoryList;

typedef struct {                    //point list type
  int nPoint;
  tPoint *point;
} tPointList;

typedef struct {                    //motor list type
  int nMotor;
  Stepper_lib* motor[MAX_MOTOR];
} tMotorList;


/////////////////////////////////////////////////////////////////CLASS MOTION_LIB////////////////////////////////////////////////////////
class Motion_lib {                  //motion class---------------------------------------------
  public:                           //public
    Motion_lib(int reference);

    tErrorMotion Motion_lib::_motorList_add(char axis);
  private:                          //private
    tMotorList _motorList;
    tPointList _pointList;
    tPoint _actualPoint;

    void _motorList_init();
    bool _existAxis(char axis);
    tErrorMotion _axisAdd(char axis);
};//close of class

#endif
