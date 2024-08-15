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
            edf_control_signal = 30; // Needs to start at this value, while starting
        }

        void init() {
            delay(1000);  // global delay
            edf_servo.attach(edf_pin);
            edf_servo.write(29);
            delay(3000);  // waiting for indefinite beeping
            edf_servo.write(180);  // should begin double beeping
            delay(1000);  // within 4 double beeps, firing up the fan
            edf_servo.write(edf_control_signal);
        }

        void update(int control_angle) {
            if (Serial.available()) {
                auto user_command = Serial.readString();  // can only update in range 30-180
                edf_control_signal = user_command.toInt();
                edf_servo.write(edf_control_signal);
            }
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

        Bluetooth(int baud_rate = 9600) {
            bluetooth_baud_rate = baud_rate;
            incomingData = "";
        }

        void init() {
            Serial.begin(9600);
            Serial2.begin(9600);
            while (!Serial2) {;}
            message();
            message();
        }

        void update() {
            if (Serial2.available()) {
                char incomingChar = Serial2.read();
                incomingData += incomingChar;

                if (incomingChar == '\n') {
                    Serial.print(incomingData);       
                    incomingData = "";            
                    } }

            if (Serial.available()) {
                String outgoingData = Serial.readStringUntil('\n');
                Serial2.print("Sent from Teensy: " + outgoingData + '\n'); }
        }

        void message(String chat) {
            Serial.print(chat + '\n');
            Serial2.print(chat + '\n');
        }
};

#endif // BLUETOOTH_H