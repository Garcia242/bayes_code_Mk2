#include "flight_computer.h"

FlightComputer fc;

void setup() {
  fc.init();
}

void  loop() {
  fc.loop();
}