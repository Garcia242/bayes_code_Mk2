#include "edf.hpp"
#include "tvc.hpp"
#include "pid.hpp"

int roll_servo_pin = 11;
int pitch_servo_pin = 10;

std::array<float, 2> tvc_angles = {0.0f, 0.0f};
float roll_pid_gains[3] = {-2.0f, 0.0f, 0.5f};
float pitch_pid_gains[3] = {2.0f, 0.0f, 0.5f};

Bluetooth bayes_bluetooth;
TVC bayes_tvc = TVC(roll_servo_pin, pitch_servo_pin);
Controller bayes_controller = Controller(roll_pid_gains, pitch_pid_gains);

void setup() {
    delay(1000);         // delay to indicate start of the code
    bayes_bluetooth.init();
    bayes_bluetooth.message("Bluetooth HC-05 can now do wizardry ;)");
    bayes_bluetooth.message("Bayes is now initialising ...");

    delay(1000);         // delay before next step
    bayes_tvc.init();    // rotate edf 360o and stop at starting position
    bayes_bluetooth.message("Bayes thrust vector control gimbling initialised ;)");
    
    delay(1000);        // please keep the rocket as still as possible, do not shake it now!
    bayes_controller.init();
    bayes_bluetooth.message("Bayes sensors and pid controller's initialised ;)");
}

void loop() {
    tvc_angles = bayes_controller.orientation_controller();
    Serial.print(tvc_angles[0]);
    Serial.print("\t");
    Serial.println(tvc_angles[1]);
    bayes_tvc.update(int(tvc_angles[0]), int(tvc_angles[1]));
    delay(100);
}