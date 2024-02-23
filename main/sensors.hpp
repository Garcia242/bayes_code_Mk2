#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <DFRobot_BMP280.h>
#include <utility/imumaths.h>
#include "coordinate.hpp"

#ifndef SENSOR_H
#define SENSOR_H

class Sensor
{
    Adafruit_BNO055 imu = Adafruit_BNO055(55);
    DFRobot_BMP280* bmp;

public:

    // Sensor(bool calibrate_status)
    // {
    //     // this->calibrate = calibrate_status;
    // }

    void init()
    {
                // START BMP
        unsigned status_bmp = bmp->begin();
        delay(1000);
        if (status_bmp)
        {
            Serial.println("connected to BMP280");
        }

        // START IMU
        // unsigned status_imu = imu.begin();
        // delay(1000);
        // imu.setExtCrystalUse(true);
        // if (status_imu)
        // {
        //     Serial.println("connected to BNO055");
        // }
        

    }

    Vector3 getOrientation()
    {
        imu::Vector<3> ori = imu.getVector(Adafruit_BNO055::VECTOR_EULER);
        return Vector3(ori.x(), ori.y(), ori.z());
    }

    Vector3 getAcceleration()
    {
        imu::Vector<3> acc = imu.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
        return Vector3(acc.x(), acc.y(), acc.z());
    }

    Vector3 getAngularVelocity()
    {
        imu::Vector<3> gyro = imu.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
        return Vector3(gyro.x(), gyro.y(), gyro.z());
    }

    Vector3 getMagnetometer()
    {
        imu::Vector<3> mag = imu.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
        return Vector3(mag.x(), mag.y(), mag.z());
    }

    Quaternion getQuaternion()
    {
        imu::Quaternion quat = imu.getQuat();
        return Quaternion(quat.w(), quat.x(), quat.y(), quat.z());
    }

    uint32_t getPressure()
    {
        return bmp->getPressure();
    }

    float getAmplitude()
    {
        auto pressure = getPressure();
        return bmp->calAltitude(1013.25, pressure);
    }



    float getTemperature()
    {
        return bmp->getTemperature();
    }
};

#endif