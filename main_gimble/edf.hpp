#include <Servo.h>

#ifndef EDF_H
#define EDF_H

class EDF {
    public:
        int edf_pin;
        int edf_control_signal;
        Servo edf_servo;

        EDF(int pin = 3) {
            edf_pin = pin;
            edf_control_signal = 180; // Needs to start at this value, while starting
        }

        void init() {
            delay(1000);  // global delay
            edf_servo.attach(edf_pin);
            edf_servo.write(edf_control_signal);    // should begin double beeping
            delay(5000);  
            edf_servo.write(30);  // should do a long beep 
        }

        void update(String control_angle) {
            if (control_angle != "None") {
                int unmapped_angle = control_angle.toInt();
                if (unmapped_angle == 0) {
                    edf_control_signal = 30;
                    edf_servo.write(edf_control_signal);
                    Serial.println("The EDF fan has stopped!");
                } else {
                    edf_control_signal = map(unmapped_angle, 0, 100, 36, 180);
                    edf_servo.write(edf_control_signal);
                    Serial.println("The EDF fan is at " + String(edf_control_signal) + " (" + String(unmapped_angle) + "%).");
                } }
        }
};

#endif // EDF_H

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

// default is set at 9600
class Bluetooth {
    public:
        int bluetooth_baud_rate;
        String incomingData;
        String prev_message;

        Bluetooth(int baud_rate = 9600) {
            bluetooth_baud_rate = baud_rate;
            incomingData = "";
        }

        void init() {
            Serial.begin(9600);
            Serial2.begin(9600);
            while (!Serial2) {;}
        }

        String update() {
            if (Serial2.available()) {
                char incomingChar = Serial2.read();
                incomingData += incomingChar;

                if (incomingChar == '\n') {
                    Serial.print(incomingData);
                    prev_message = incomingData;       
                    incomingData = "";            
                    return prev_message; } 
            }

            if (Serial.available()) {
                String outgoingData = Serial.readStringUntil('\n');
                Serial2.print("Sent from Teensy: " + outgoingData + '\n'); }
            return "None";
        }

        void message(String chat) {
            Serial.print(chat + '\n');
            Serial2.print(chat + '\n');
        }
};

#endif // BLUETOOTH_H