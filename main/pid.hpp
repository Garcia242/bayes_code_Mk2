#include <math.h>
#include <array>
#include "sensors.hpp"

#ifndef PID_H
#define PID_H
class PID {
    public:
    float Kp;
    float Ki;
    float Kd;
    float error;
    float prev_sensor_data;
    float derivative_error;
    float integral_error;
    float desired_value;
    float dt;

    PID(float Kp_input, float Ki_input, float Kd_input, float dt_input, float desired_value_input) {
        Kp = Kp_input;
        Ki = Ki_input;
        Kd = Kd_input;
        desired_value = desired_value_input;
        dt = dt_input;
        error = 0.0f;
        prev_sensor_data = 0.0f;
        derivative_error = 0.0f;
        integral_error = 0.0f;
    }

    float update(float sensor_data) {
        error = desired_value - sensor_data;
        integral_error += error*dt;
        derivative_error = (sensor_data - prev_sensor_data) / dt;
        prev_sensor_data = sensor_data;
        return Kp * error + Ki * integral_error + Kd * derivative_error;
    }
};
#endif // PID_H

#ifndef CONTROLLER_H
#define CONTROLLER_H
class Controller {
    public:
    PID roll_controller;
    PID pitch_controller;
    PID altitude_pid_controller;
    Sensor sensor;

    Controller(float roll_gain[], float pitch_gain[], float altitude_gain[], float desired_altitude):
        roll_controller(roll_gain[0], roll_gain[1], roll_gain[2], 0.1f, 0.0f),
        pitch_controller(pitch_gain[0], pitch_gain[1], pitch_gain[2], 0.1f, 0.0f),
        altitude_pid_controller(altitude_gain[0], altitude_gain[1], altitude_gain[2], 0.1f, desired_altitude) {}

    void init() {
        sensor.init();
    }

    float bounds_check(float input, float min_val, float max_val) {
        if(input > max_val) {return max_val;}
        else if(input < min_val) {return min_val;}
        else {return input;}
    }

    std::array<int, 2> orientation_controller() {
        Vector3 orientation_data = Vector3(0.0f, 0.0f, 0.0f);
        orientation_data = sensor.getOrientation();
        float roll_control_signal = roll_controller.update(orientation_data.x);
        roll_control_signal = bounds_check(roll_control_signal, -90.0f, 90.0f);
        float pitch_control_signal = pitch_controller.update(orientation_data.y);
        pitch_control_signal =  bounds_check(pitch_control_signal, -90.0f, 90.0f);
        return {int(roll_control_signal+90.0f), int(pitch_control_signal+90.0f)};
    }

    int altitude_controller() {
        float altitude_data = sensor.getAltitude();
        float altitude_control_signal = altitude_pid_controller.update(altitude_data);
        altitude_control_signal = bounds_check(altitude_control_signal, 0.0f, 180.0f);
        return int(altitude_control_signal);
    }
};
#endif // CONTROLLER_H