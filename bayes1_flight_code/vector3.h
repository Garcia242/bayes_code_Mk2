#ifndef VECTOR3_H
#define VECTOR3_H

#include "defines.h"

//Creating a new custom data type Vector3
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
