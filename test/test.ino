// #include "Wire.h"
// #include "DFRobot_BNO055.h"

// typedef DFRobot_BNO055_IIC BNO;

// BNO bno(&Wire, 0x28);

// void setup()
// {
//     Serial.begin(115200);
//     bno.reset();
//     // while (bno.begin() != BNO::eStatusOK)
//     // {
//     //     Serial.println("BNO begin failed");
//     //     delay(2000);
//     // }
//     Serial.println("BNO connected");
// }

// void loop()
// {
//     BNO::sAxisAnalog_t sAccAnalog;

//     sAccAnalog = bno.getAxis(BNO::eAxisAcc);
//     Serial.print("x: ");
//     Serial.print(sAccAnalog.x);
//     Serial.print("\t y: ");
//     Serial.print(sAccAnalog.y);
//     Serial.print("\t z: ");
//     Serial.println(sAccAnalog.z);
//     delay(1000);
// }




#include <Wire.h>

void setup() {
  Wire.begin();

  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }

  Serial.println("\nI2C Scanner");
}

void loop() {
  byte error, address;
  int deviceCount = 0;

  Serial.println("Scanning...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("Device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("");
      deviceCount++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }

  if (deviceCount == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("Done\n");
  }

  delay(5000);  // Wait 5 seconds before scanning again
}

