#include <BasicLinearAlgebra.h>

using namespace BLA;

#ifndef ALTITUDE_KF_H
#define ALTITUDE_KF_H
class Altitude_KF {
  public:
    // declare kalman filter predictions and measurments
    Matrix<2, 1> x;
    Matrix<2, 1> y;
    // declare all kalman filter constant matrices
    Matrix<2, 2> A;
    Matrix<2, 1> G;
    Matrix<1, 2> H;
    Matrix<2, 2> Q;
    Matrix<1, 1> R;
    // declare all kalman filter dynamic matrices
    Matrix<2, 2> P;
    Matrix<2, 2> P_pred;
    Matrix<2, 1> K;

    // Constructor to intialise matrices. Ensure dt is small around 0.004
    Altitude_KF(float dt) {
        // initialising kalman filter predictions and measurments
        x.Fill(0.0f);
        y.Fill(0.0f);
        // initialising all kalman filter constant matrices
        A = {1.0f, dt, 0.0f, 1.0f};
        G = {0.5f*dt*dt, dt};
        H = {1.0f, 0.0f};
        Q = {0.016f*pow(dt, 4), 0.032f*pow(dt, 3), 0.032f*pow(dt, 3), 0.064f*pow(dt, 2)};
        R = {0.49f};
        // initialising all kalman filter dynamic matrices
        P.Fill(0.0f);
        P_pred.Fill(0.0f);
        K.Fill(0.0f);
    }
    
    float filter(float acc_z, float alti_z) {
      // make a prediction
      Matrix<2, 1> w = G*acc_z;
      x = A*y + w;
      // update error covariance
      P_pred = A*P*~A + Q;
      // compute Kalman gain
      K = P_pred*~H*Inverse(H*P_pred*~H + R);
      // compute estimate
      Matrix<1, 1> prediction = H*x;
      Matrix<1, 1> error = {alti_z - prediction(0, 0)};
      y = x + K*error;
      // compute error covariance
      P = P_pred - K*H*P_pred;
      return y(0, 0);
    };
};
#endif // ALTITUDE_KF_H


#ifndef ANGLE_KF_H
#define ANGLE_KF_H
class Angle_KF {
  public:
    // declare kalman filter predictions and measurments
    Matrix<2, 1> x;
    Matrix<2, 1> y;
    // declare all kalman filter constant matrices
    Matrix<2, 2> A;
    Matrix<2, 1> G;
    Matrix<1, 2> H;
    Matrix<2, 2> Q;
    Matrix<1, 1> R;
    // declare all kalman filter dynamic matrices
    Matrix<2, 2> P;
    Matrix<2, 2> P_pred;
    Matrix<2, 1> K;

    // Constructor to intialise matrices. Ensure dt is small around 0.004
    Angle_KF(float dt, float angvel_drift) {
        // initialising kalman filter predictions and measurments
        x.Fill(0.0f);
        y = {0.0f, angvel_drift};
        // initialising all kalman filter constant matrices
        A = {1.0f, -dt, 0.0f, 1.0f};
        G = {dt, 0};
        H = {1.0f, 0.0f};
        Q = {0.003*dt, 0, 0, 0.03*dt};
        R = {0.2f};
        // initialising all kalman filter dynamic matrices
        P.Fill(0.0f);
        P_pred.Fill(0.0f);
        K.Fill(0.0f);
    }
    
    float filter(float angvel, float angle) {
        // make a prediction
        Matrix<2, 1> w = G*angvel;
        x = A*y + w;
        // update error covariance
        P_pred = A*P*~A + Q;
        // compute Kalman gain
        K = P_pred*~H*Inverse(H*P_pred*~H + R);
        // compute estimate
        Matrix<1, 1> prediction = H*x;
        Matrix<1, 1> error = {angle - prediction(0, 0)};
        y = x + K*error;
        // compute error covariance
        P = P_pred - K*H*P_pred;
        return y(0,0);
    };
};
#endif // ANGLE_KF_H