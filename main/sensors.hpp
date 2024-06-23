/**
* @section DESCRIPTION
*
* Initialises, then retrieves data from the IMU and barometer.
*/

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

/**
 * @brief Create event structures and variables to store data for orientation, 
 acceleration, angular velocity, magnetic field strength, temperature, pressure, and altitude.
 */
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

    /**
     * @brief Check the status of the BMP280 and print its current state.
     * @param BMP::eStatus_t eStatus: Current status of the BMP280
     */
    void printLastOperateStatus(BMP::eStatus_t eStatus) {
      switch(eStatus) {
      case BMP::eStatusOK:    Serial.println("everything ok"); break;
      case BMP::eStatusErr:   Serial.println("unknow error"); break;
      case BMP::eStatusErrDeviceNotDetected:    Serial.println("device not detected"); break;
      case BMP::eStatusErrParameter:    Serial.println("parameter error"); break;
      default: Serial.println("unknow status"); break;
      }
    }

    /**
     * @brief Initialise the BNO055 and BMP280. If unable, print an error message.
     */
    void init() {
        // Check if BNO055 detected.
        if(!imu.begin()) {
          Serial.println("BAYES IMU NOT DETECTED, GO FYU!");
        }

        // Check if BMP280 detected.
        bmp.reset();
        while(bmp.begin() != BMP::eStatusOK) {
          Serial.println("bmp begin faild");
          printLastOperateStatus(bmp.lastOperateStatus);
          delay(2000);
        }
    }

    /**
     * @brief Get the orientation data from the BNO055 IMU in Euler angles.
     * @return Vector3 object with pitch, yaw, and roll values in Euler angles (degrees).
     */
    Vector3 getOrientation() {
        imu.getEvent(&orientation, Adafruit_BNO055::VECTOR_EULER);
        return Vector3(orientation.orientation.x, orientation.orientation.y, orientation.orientation.z);
    }
    
    /**
     * @brief Get the acceleration measurements from the BNO055 IMU.
     * @return Vector3 object with acceleration measured in 3 axes (m/s^2).
     */
    Vector3 getAcceleration() {
        imu.getEvent(&accelerometer, Adafruit_BNO055::VECTOR_ACCELEROMETER);
        return Vector3(accelerometer.acceleration.x, accelerometer.acceleration.y, accelerometer.acceleration.z);
    }

    /**
     * @brief Get the angular velocity measurements from the BNO055 IMU.
     * @return Vector3 object with angular velocity measured in 3 axes (rad s^-1).
     */
    Vector3 getAngularVelocity() {
        imu.getEvent(&angular_velocity, Adafruit_BNO055::VECTOR_GYROSCOPE);
        return Vector3(angular_velocity.gyro.x, angular_velocity.gyro.y, angular_velocity.gyro.z);
    }

    /**
     * @brief Get the magnetic flux density measurements from the BNO055 IMU.
     * @return Vector3 object with magnetic flux density measured in 3 axes (uT).
     */
    Vector3 getMagnetometer() {
        imu.getEvent(&magnetometer, Adafruit_BNO055::VECTOR_MAGNETOMETER);
        return Vector3(magnetometer.magnetic.x, magnetometer.magnetic.y, magnetometer.magnetic.z);
    }

    /**
     * @brief Get the orientation data from the BNO055 IMU in a quaternion.
     * @return Quaternion object with 4 components.
     */
    Quaternion getQuaternion() {
        imu::Quaternion quat = imu.getQuat();
        return Quaternion(quat.w(), quat.x(), quat.y(), quat.z());
    }

    /**
     * @brief Get the temperature data from the BMP280.
     * @return Temperature in Celsius.
     */
    float getTemperature() {
        temperature = bmp.getTemperature();
        return temperature;
    }

    /**
     * @brief Get the pressure data from the BMP280.
     * @return Pressure in Pa.
     */
    uint32_t getPressure() {
        pressure = bmp.getPressure();
        return pressure;
    }

    /**
     * @brief Get the pressure data from the BMP280 and use it to calculate the altitude.
     * @return Altitude in m.
     */
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
