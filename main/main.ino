/**
* @section DESCRIPTION
*
* Create an instance of the sensor, then prints out mission parameters.
*/

#include "sensors.hpp"

// Create a sensor object for the sensors.
Sensor sensor;

/**
* @brief Set the baud rate and initialise the sensors.
*/
void setup()
{
    Serial.begin(9600);
    sensor.init();
}

/**
 * @brief Retrieve data from the sensors and print it out.
 */
void loop()
{
    // Serial.println(sensor.getAcceleration().x);
    // Serial.println(sensor.getAngularVelocity().x);
    // Serial.println(sensor.getMagnetometer().x);
    // float alt = (1-pow(10, log10(pres/SEA_LEVEL_PRESSURE)/5.25588))/(2.25577e-5);
    float alt = sensor.getAltitude();
    Serial.println(alt);
    delay(1000);
}
