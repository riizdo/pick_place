/*  file: motor_lib.h
 *  author: riizdo
 *  date: 30/05/2020
 *  version: 0.0
 *  description: data and headers for motor library
 */

 #ifndef motor_lib_h
 #define motor_lib_h

#include "Arduino.h"
#include <AccelStepper.h>


class Motor_lib:AccelStepper {
  public:
    Motor_lib(int type, int stepPin, int dirPin, char axis);

    char getAxis();
    int getPosition();
  private:
    int _type;
    int _stepPin;
    int _dirPin;
    char _axis;
};

 #endif
