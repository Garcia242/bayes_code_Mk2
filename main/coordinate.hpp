#include "defines.hpp"
#include <math.h>

#ifndef VECTOR_H
#define VECTOR_H
// Creating a new custom data type Vector3
class Vector3 {
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    ~Vector3() {}

    Vector3 toRadians() {
        return Vector3(x * DEG_TO_RAD, y * DEG_TO_RAD, z * DEG_TO_RAD);
    }

    Vector3 toDegrees() {
        return Vector3(x * RAD_TO_DEG, y * RAD_TO_DEG, z * RAD_TO_DEG);
    }
};
#endif

#ifndef QUATERNION_H
#define QUATERNION_H

class Quaternion
{
public:
    float w = 0.0f;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
    ~Quaternion() {}

    Vector3 toRadians() const {
        float pitch = -1 * PI / 2 + 2 * atan2(sqrt(1 + 2 * (w * y - x * z)), sqrt(1 - 2 * (w * y - x * z)));
        float roll = atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y));
        float yaw = atan2(2 * (w * z + x * y), 1 - 2 * (y * y + z * z));
        return Vector3(pitch, roll, yaw);
    }

    Vector3 toDegrees() const {
        Vector3 angles = toRadians();
        angles.x *= RAD_TO_DEG;
        angles.y *= RAD_TO_DEG;
        angles.z *= RAD_TO_DEG;
        return angles;
    }
};
#endif
