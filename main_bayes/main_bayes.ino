#include "edf.hpp"
#include "tvc.hpp"
#include "pid.hpp"

int roll_servo_pin = 11;
int pitch_servo_pin = 10;
int edf_on_flag = 0;

std::array<float, 2> tvc_angles = {0.0f, 0.0f};
float roll_pid_gains[3] = {-2.0f, 0.0f, 0.5f};
float pitch_pid_gains[3] = {2.0f, 0.0f, 0.5f};

EDF bayes_edf;
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

    bayes_bluetooth.message("Get ready to turn the EDF switch ON");
    delay(3000);
    bayes_bluetooth.message("In 3 seconds turn EDF switch ON");
    bayes_edf.init(); 
    bayes_bluetooth.message("IMPORTANT: EDF initialising... Fire EDF!");
    while (!edf_on_flag) {
        String message = bayes_bluetooth.update();
        if (message != "None") {
            bayes_edf.update(message);
            edf_on_flag = 1;
            delay(100);
        }
    }
    bayes_bluetooth.message("EDF initialised ;)");

    bayes_bluetooth.message("Bayes lifts off in T-minus 3.0 seconds.");
    delay(1000);
    bayes_bluetooth.message("Bayes lifts off in T-minus 2.0 seconds.");
    delay(1000);
    bayes_bluetooth.message("Bayes lifts off in T-minus 1.0 seconds.");
    delay(1000);
    bayes_bluetooth.message("And we have lift-off!");
    delay(1500);
    bayes_bluetooth.message("The rocket has cleared the tower and is climbing steadily into the sky. All systems are nominal at this time.");
}

void loop() {
    String message = bayes_bluetooth.update();
    bayes_edf.update(message);
    tvc_angles = bayes_controller.orientation_controller();
    bayes_tvc.update(tvc_angles[0], tvc_angles[1]);
    delay(100);
}

// Next Year: Get the EDF PID controller working, so it hover's at one place stabely. For now we controled it manually.
// Next Year: Get the landing code working, so that we don't crash land each time.
// Next Year: Get the PCB printed and test the current with that and then proceed.
// Next Year: Get a live user interface (GUI) where you see all the sensor data visually on some graph.