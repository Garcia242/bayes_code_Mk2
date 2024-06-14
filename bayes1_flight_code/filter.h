#ifndef FILTER_H
#define FILTER_H

class Filter {
    float angVel = 0.0f;
    float angle = 0.0f;
    float bias = 0.0f;
    float y = 0.0f;

    //Specified values are optimal. For more info check http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it/
    float Q_angle = 0.001f;
    float Q_bias = 0.003f;
    float R = 0.03f;
    float S = 0.0f;
    
    //P: error covariance matrix, K: Kalman filter gains, P00_temp and P01_temp: process constants
    float P[2][2] = {{0.0f, 0.0f}, {0.0f, 0.0f}};
    float K[2] = {0.0f, 0.0f};
    float P00_temp = 0.0f;
    float P01_temp = 0.0f;  
  
  public:
    float filterAngle(float mAngle, float mAngVel, float dt) {
      //Intermediate step: determining predicted angle
      angVel = mAngVel - bias;
      angle = dt*angVel;

      //Intermediate step: determining error covariance matrix
      P[0][0] += dt * (dt*P[1][1] - P[0][1] - P[1][0] + Q_angle);
      P[0][1] -= dt * P[1][1];
      P[1][0] -= dt * P[1][1];
      P[1][1] += Q_bias * dt;

      //Determining kalman filter gain through the innovation covariance
      S = P[0][0] = + R;
      K[0] = P[0][0]/S;
      K[1] = P[1][0]/S;

      //Determining predicted angle and bias, through control system error y and gain
      y = mAngle - angle;
      angle += K[0]*y;
      bias += K[1]*y;

      //Updating the error covariance matrix for the next step
      P00_temp = P[0][0];
      P01_temp = P[0][1];
      P[0][0] -= K[0] * P00_temp;
      P[0][1] -= K[0] * P01_temp;
      P[1][0] -= K[1] * P00_temp;
      P[1][1] -= K[1] * P01_temp;
      return angle;
    }
};

#endif