#ifndef EDF_H
#define EDF_H

#include <Servo.h>

class EDF {
    Servo edf;
    int control = 30;
    float current_time = 0.0f;
    float timer = 0.0f;
    float elapsed_time = 0.0f;
    float control_timer = 1000.0f;
    bool edf_stop = true;

    auto user_input() {
        if (Serial.available()) {
            auto command = Serial.readString();
            command.trim();
            if(command == "start" && edf_stop) {
                edf_stop = false;
                return control;
            }
            else if(command == "stop" && !edf_stop) {
                edf_stop = true;
                return control;
            }
            else {
                return command;
            }
        }
        timer = millis();
    }

    void update(auto input) {
        if (elapsed_time > control_timer) {
            if(edf_stop) {
                control = user.toInt();
                edf.write(control);
                Serial.print("Changed speed (0 - 180): ");
                Serial.println(control);
            }                
        }
    }

    public:
        void init() {
            edf.attach(3);
            edf.write(control);
        }

        void loop() {
            float current_time = millis();
            auto input = user_input();
            elapsed_time = current_time - timer;
            update(input);
        }
};

#endif