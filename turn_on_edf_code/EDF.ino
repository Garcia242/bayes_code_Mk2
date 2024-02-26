#include <Servo.h>

/*
 * First time calibration:
 * 0. Turning the EDF without any configuration will cause it to beep indefinitely, approx. every 1 second.
 * 1. Make sure the EDF is off, upload the program first.
 * 2. Send an input of 180 (upper bound) to it the EDF.
 * 2. Turn the EDF on, it will start making double beeps indicating
 *    that it is in the settings phase.
 * 3. During the first 4 double beeps, write a value of 30 or greater which 
 *    will be considered the minimum thrust (no thrust).
 *        - Note: Writing 29 will re-enter the indefinite beeping, after 
 *                which 180 can be written to restart the double beeps.
 * 4. The EDF will beep 12 times (one for each lipo cell), after which it will set
 *    the minimum thrust at 30. 
 *    
 * Regular use:
 * 1. Write a value of 30 to the EDF pin once before turning it on.
 * 2. Writing a thrust value in the following range: 37 <= value <= 180 once to the 
 *    EDF will produce constant thrust.
 *        - Note: Values between 30 and 36 do not produce thrust (off).
 *        - Warning: Values above 100 get extremely loud!
 *        - Note: The EDF still has unity step (1) thrust resolution after 37, which means 38 
 *                will produce a noticeably different thrust than 37.
 */

int pin = 3; // change to pin where the EDF is connected
Servo edf;
int control = 30; // change to 180 according to omkar

void setup() {
  Serial.begin(9600);
  delay(3000);
  edf.attach(pin);
  Serial.println("Pin attached.");
  edf.write(control);
}

void loop() {
  if (Serial.available()) {
    auto user = Serial.readString();
    control = user.toInt();
    edf.write(control);
    Serial.println(control);
  }
} 