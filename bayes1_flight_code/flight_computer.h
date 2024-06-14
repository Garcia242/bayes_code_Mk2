#ifndef FLIGHTCOMPUTER_H
#define FLIGHTCOMPUTER_H

#include "gimble.h"
#include "sensor.h"
#include "filter.h"
#include "controller.h"

class FlightComputer {
    Gimble gimble;    
    IMU bayesIMU = IMU(false);
    PID pitchPID = PID(0.1, 0.01, 0.01, 7);
    PID rollPID = PID(0.1, 0.01, 0.01, 7);
    Filter pitchKFilter;
    Filter rollKFilter;
    Quaternion orientation = Quaternion(0,0,0,0);
    Vector3 angle = Vector3(0,0,0);
    Vector3 ang_vel = Vector3(0,0,0);
    float roll, pitch;
    float roll_servo, pitch_servo;
    unsigned long millisNew, millisOld;
    float dt;
    
    void print(bool condition) {
        if(condition) {
            Serial.print("Filtered Sensor Data: roll = ");
            Serial.print(roll);
            Serial.print(", pitch = ");
            Serial.print(pitch);
            Serial.println(".");
            Serial.print("PID Output: roll = ");
            Serial.print(roll_servo);
            Serial.print(", pitch = ");
            Serial.println(pitch_servo);
            Serial.println(".");
        }
        else {
        }
    }

    public:
      void init() {
          Serial.begin(115200); 
          bayesIMU.init();
          bayesIMU.calibrate();
          gimble.testROM();
          millisNew = millis();
      }

      void loop() {
          millisOld = millisNew;
          millisNew = millis();
          dt = (millisNew - millisOld)/1000;
          
          orientation = bayesIMU.getQuaternion();
          angle = orientation.toDegrees();
          ang_vel = bayesIMU.getAngularVelocity();
          
          roll = rollKFilter.filterAngle(angle.x, ang_vel.x, dt);
          pitch = pitchKFilter.filterAngle(angle.y, ang_vel.y, dt);
          roll_servo = 90 + rollPID.controller(roll);
          pitch_servo = 90 + pitchPID.controller(pitch);
          innerServo.write(roll_servo);
          outerServo.write(pitch_servo);
          print(false);
          delay(20);
      }
};

#endif
