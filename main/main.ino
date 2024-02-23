#include "sensors.hpp"

Sensor sensor;

void setup()
{
    Serial.begin(9600);
    sensor.init();
}

void loop()
{
    // Serial.println(sensor.getAcceleration().x);
    // Serial.println(sensor.getAngularVelocity().x);
    // Serial.println(sensor.getMagnetometer().x);
    Serial.println(sensor.getAmplitude());
    delay(1000);
}
