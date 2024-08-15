#include "edf.hpp"
#include "tvc.hpp"
#include "pid.hpp"

int roll_servo_pin = 11;
int pitch_servo_pin = 10;

float desired_altitude = 1.0f       // this is where the rocket will hover
float roll_pid_gains = {0.1f, 0.1f, 0.1f};
float pitch_pid_gains = {0.1f, 0.1f, 0.1f};
float altitude_pid_gains = {0.1f, 0.1f, 0.1f};

int land_flag = 0;
int edf_thrust = 0.0f;
int tvc_angles[2] = {0.0f, 0.0f};
Bluetooth bayes_bluetooth;
EDF bayes_edf;
TVC bayes_tvc = TVC(roll_servo_pin, pitch_servo_pin);
Controller bayes_controller = Controller(roll_pid_gains, pitch_pid_gains, altitude_pid_gains, desired_altitude)

void setup() {
    delay(1000);         // delay to indicate start of the code
    bayes_bluetooth.init();
    bayes_bluetooth.message("Bluetooth HC-05 can now do wizardry ;)");
    bayes_bluetooth.message("Bayes is now initialising ...");

    delay(1000);         // delay before next step
    bayes_tvc.init();    // rotate edf 360o and stop at starting position
    bayes_bluetooth.message("Bayes thrust vector control gimbling initialised ;)");
    
    delay(10000);        // please keep the rocket as still as possible, do not shake it now!
    bayes_controller.init();
    bayes_bluetooth.message("Bayes sensors and pid controller's initialised ;)");
    bayes_edf.init(); 
    bayes_bluetooth.message("IMPORTANT: EDF initialising... Don't write anything in serial !!!");

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
    if (! land_flag) {
        bayes_bluetooth.update();
        edf_thrust = bayes_controller.altitude_controller();
        tvc_angles = bayes_controller.orientation_controller();
        land_flag = bayes_edf.update(edf_thrust);
        bayes_tvc.update(tvc_angles[0], tvc_angles[1]);
    }
}

// Next Year: Get the EDF PID controller working, so it hover's at one place stabely. For now we controled it manually.
// Next Year: Get the landing code working, so that we don't crash land each time.
// Next Year: Get the PCB printed and test the current with that and then proceed.
// Next Year: Get a live user interface (GUI) where you see all the sensor data visually on some graph.