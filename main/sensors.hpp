#include <Wire.h>
#include "coordinate.hpp"
#include <DFRobot_BMP280.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>
#include "kalman_filter.hpp"

#ifndef SENSOR_H
#define SENSOR_H

typedef DFRobot_BMP280_IIC BMP;
BMP bmp(&Wire, BMP::eSdoLow);

class Sensor {
    public:
        Adafruit_BNO055 imu = Adafruit_BNO055(55, 0x28, &Wire);
        Angle_KF kalman_filter_pitch = Angle_KF(0.25, 0.0f);
        Angle_KF kalman_filter_roll = Angle_KF(0.25, 0.0f);
        Angle_KF kalman_filter_yaw = Angle_KF(0.25, 0.0f);
        Altitude_KF kalman_filter_accelz = Altitude_KF(0.1f); 
        sensors_event_t orientation;
        sensors_event_t accelerometer;
        sensors_event_t angular_velocity;
        sensors_event_t magnetometer;
        sensors_event_t linear_acceleration;
        float temperature;
        uint32_t pressure;
        float altitude;
        float normalise_alt;


        // FOR BMP280 FAILURE DETECTION
        void printLastOperateStatus(BMP::eStatus_t eStatus) {
            switch(eStatus) {
                case BMP::eStatusOK:    Serial.println("Everything OK"); break;
                case BMP::eStatusErr:   Serial.println("Unknown Error"); break;
                case BMP::eStatusErrDeviceNotDetected:    Serial.println("Device Not Detected"); break;
                case BMP::eStatusErrParameter:    Serial.println("Parameter Error"); break;
                default: Serial.println("Unknown Status"); break;
            }
        }

        // START: CHECK SENSOR CONNECTIONS
        void init() {
            // BNO055: CHECK SENSORS
            if(!imu.begin()) {
                Serial.println("BAYES IMU NOT DETECTED, GO FY!");
            }
            // BMP280: CHECK SENSORS
            bmp.reset();
            while(bmp.begin() != BMP::eStatusOK) {
                Serial.println("bmp begin faild");
                printLastOperateStatus(bmp.lastOperateStatus);
                delay(2000);
            }
            // To remove bias that exists with altitude data
            normalise_alt = 0;
            for (int i=0; i < 600; i++) {
                float pressure_sensor_data = float(Sensor::getPressure());
                float alt_z = 44330 * (1.0 - pow((pressure_sensor_data / SEA_LEVEL_PRESSURE), (1.0 / 5.225)));
                if (i >= 100) {
                    normalise_alt += alt_z;
                }
            }
            normalise_alt = normalise_alt/500;
        }

        Vector3 getOrientation() {
            Quaternion quaternion = Sensor::getQuaternion();
            Vector3 euler_angles = quaternion.toDegrees();
            Vector3 ang_vel = Sensor::getAngularVelocity();
            
            float roll = kalman_filter_roll.filter(ang_vel.x, euler_angles.x-3);
            float pitch = kalman_filter_pitch.filter(ang_vel.y, euler_angles.y+2);
            float yaw = kalman_filter_yaw.filter(ang_vel.z, euler_angles.z);
            return Vector3(roll, pitch, yaw);
        }

        Vector3 getAcceleration() {
            imu.getEvent(&accelerometer, Adafruit_BNO055::VECTOR_ACCELEROMETER);
            return Vector3(accelerometer.acceleration.x, accelerometer.acceleration.y, accelerometer.acceleration.z);
        }

        Vector3 getAngularVelocity() {
            imu.getEvent(&angular_velocity, Adafruit_BNO055::VECTOR_GYROSCOPE);
            return Vector3(angular_velocity.gyro.x, angular_velocity.gyro.y, angular_velocity.gyro.z);
        }

        Vector3 getMagnetometer() {
            imu.getEvent(&magnetometer, Adafruit_BNO055::VECTOR_MAGNETOMETER);
            return Vector3(magnetometer.magnetic.x, magnetometer.magnetic.y, magnetometer.magnetic.z);
        }

        Vector3 getLinearAcceleration() {
            imu.getEvent(&linear_acceleration, Adafruit_BNO055::VECTOR_LINEARACCEL);
            return Vector3(linear_acceleration.acceleration.x, linear_acceleration.acceleration.y, linear_acceleration.acceleration.z);
        }

        Quaternion getQuaternion() {
            imu::Quaternion quat = imu.getQuat();
            return Quaternion(quat.w(), quat.x(), quat.y(), quat.z());
        }

        float getTemperature() {
            temperature = bmp.getTemperature();
            return temperature;
        }

        uint32_t getPressure() {
            pressure = bmp.getPressure();
            return pressure;
        }
        float roundToNearestQuarter(float value) {
            return round(value * 4.0) / 4.0;
        }
        float getAltitude() {
            Vector3 linear_accel = Sensor::getLinearAcceleration();
            float pressure_sensor_data = float(Sensor::getPressure());
            float alt_z = 44330 * (1.0 - pow((pressure_sensor_data / SEA_LEVEL_PRESSURE), (1.0 / 5.225))) - normalise_alt;
            altitude = kalman_filter_accelz.filter(linear_accel.z, alt_z);
            return  roundToNearestQuarter(altitude);
        }
};

#endif