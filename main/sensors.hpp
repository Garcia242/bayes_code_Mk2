#include <Wire.h>
#include "coordinate.hpp"
#include <DFRobot_BMP280.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>

#ifndef SENSOR_H
#define SENSOR_H

typedef DFRobot_BMP280_IIC BMP;
BMP bmp(&Wire, BMP::eSdoLow);

class Sensor {
    Adafruit_BNO055 imu = Adafruit_BNO055(55, 0x28, &Wire);
    sensors_event_t orientation;
    sensors_event_t accelerometer;
    sensors_event_t angular_velocity;
    sensors_event_t magnetometer;
    float temperature;
    uint32_t pressure;
    float altitude;

public:

    // FOR BMP280 FAILURE DETECTION
    void printLastOperateStatus(BMP::eStatus_t eStatus) {
      switch(eStatus) {
      case BMP::eStatusOK:    Serial.println("everything ok"); break;
      case BMP::eStatusErr:   Serial.println("unknow error"); break;
      case BMP::eStatusErrDeviceNotDetected:    Serial.println("device not detected"); break;
      case BMP::eStatusErrParameter:    Serial.println("parameter error"); break;
      default: Serial.println("unknow status"); break;
      }
    }

    // START: CHECK SENSOR CONNECTIONS
    void init() {
        // BNO055: CHECK SENSORS
        if(!imu.begin()) {
          Serial.println("BAYES IMU NOT DETECTED, GO FYU!");
        }

        // BMP280: CHECK SENSORS
        bmp.reset();
        while(bmp.begin() != BMP::eStatusOK) {
          Serial.println("bmp begin faild");
          printLastOperateStatus(bmp.lastOperateStatus);
          delay(2000);
        }
    }

    Vector3 getOrientation() {
        imu.getEvent(&orientation, Adafruit_BNO055::VECTOR_EULER);
        return Vector3(orientation.orientation.x, orientation.orientation.y, orientation.orientation.z);
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

    float getAltitude() {
        // altitude = bmp.calAltitude(SEA_LEVEL_PRESSURE, pressure);
        float a = 2.25577e-5;
        float b = 5.25588;
        float pressure = getPressure();
        float numerator = 1 - pow(10, (log10(pressure / SEA_LEVEL_PRESSURE)) / (b));
        altitude = numerator / a;
        return altitude;
    }
};

#endif