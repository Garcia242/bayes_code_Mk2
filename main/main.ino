#include "sensors.hpp"
#include "filter.hpp"

    Sensor sensor;
    MovingAverage filter;

void setup()
{
    Serial.begin(9600);
    sensor.init();
    for (int i = 0; i < 10000; i++) {
      float alt = sensor.getAltitude();
      filter.init_buffer(alt);
    }
}

void loop()
{
    // Serial.println(sensor.getAcceleration().x);
    // Serial.println(sensor.getAngularVelocity().x);
    // Serial.println(sensor.getMagnetometer().x);
    // float alt = (1-pow(10, log10(pres/SEA_LEVEL_PRESSURE)/5.25588))/(2.25577e-5);
    float alt = sensor.getAltitude();
    float new_alt = filter.apply_filter(alt);
    Serial.println(new_alt);
    delay(1000);
}
