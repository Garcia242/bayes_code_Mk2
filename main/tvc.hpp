#include <Servo.h>

#ifndef TVC_H
#define TVC_H

class TVC {
    public:
        Servo roll_servo;      // inner servo cloests to EDF
        Servo pitch_servo;      // outer servo furthest from EDF
        int rotation_duration;

        TVC(int roll_pin, int pitch_pin, int duration = 2000) {
            roll_servo.attach(roll_pin);
            pitch_servo.attach(pitch_pin);
            rotation_duration = duration;
        }

        void init() {
            int counter = 0;
            int roll_angle = 0;
            int pitch_angle = 90;
            roll_servo.write(roll_angle);
            pitch_servo.write(pitch_angle);
            while (counter < rotation_duration) {
                roll_angle = 90*(1-cos((2*PI*counter)/rotation_duration));
                pitch_angle = 90*(1+sin((2*PI*counter)/rotation_duration));
                roll_servo.write(roll_angle);
                pitch_servo.write(pitch_angle);
                counter ++;
                delay(1); } 
        }

        void update(int roll_angle, int pitch_angle) {
            roll_servo.write(roll_angle);
            pitch_servo.write(pitch_angle);
        }
};

#endif // TVC_H