#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <math.h>
class PID {
  float Kp;
  float Ki;
  float Kd;
  float error = 0.0f;
  float differential_error = 0.0f;
  float derivative_error = 0.0f;
  float integral_error = 0.0f;
  float output = 0.0f;
  float dt = 0.001;
  float offset;

public:
  PID(float new_Kp, float new_Ki, float new_Kd, float new_offset): Kp(new_Kp), Ki(new_Ki), Kd(new_Kd), offset(new_offset) {}
  ~PID() {}
  
  float controller(float sensor_data) {
    differential_error = -(sensor_data + error);
    derivative_error = differential_error/dt;
    integral_error = integral_error - sensor_data*dt;
    error = -sensor_data;
    
    output = Kp*error + Ki*(integral_error) + Kd*(derivative_error);
    output = bounds_check();
    return output;
  }
  
  float bounds_check() {
    if(abs(output) > offset) {
      if(output < 0) {
        return -offset;
      }
      else {
        return offset;
      }
    }
    else {
      return output;
    }
  }
};

#endif
