/*  file: pick_place.ino
 *  author: riizdo
 *  date: 11/05/2020
 *  version: 0.0
 *  description: main for pick & place
 */

#include "serial_lib.h"

Serial_lib serial(1);

void setup() {
  serial.init();

}

void loop() {
  serial.listener();

}