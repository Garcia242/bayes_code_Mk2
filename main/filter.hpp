#include <vector>

#ifndef FILTER_H
#define FILTER_H

#define BUFFER_LENGTH 5000

class MovingAverage{

    public:
    float buffer[BUFFER_LENGTH];
    float old_sum = 0;

    void update_buffer(float new_value) {
      float sum1 = sum_array(buffer, BUFFER_LENGTH);
      Serial.print(new_value);
      Serial.print("\t");
      Serial.print(buffer[BUFFER_LENGTH-1]);
      Serial.print("\t");
      Serial.print("Sum Diff: ");
      Serial.print(buffer[BUFFER_LENGTH-1]-new_value);
      Serial.print("\t");
      for (int i = 0; i < BUFFER_LENGTH-1; i++) {
        buffer[i+1] = buffer[i];
      }
      buffer[0] = new_value;
      float sum2 = sum_array(buffer, BUFFER_LENGTH);
      Serial.print("Sum Diff: ");
      Serial.println(float(sum2-sum1));
    }

    void init_buffer(float new_value) {
      update_buffer(new_value);
    }

    float apply_filter(float new_value) {
        float sum = 0;
        update_buffer(new_value);
        sum = sum_array(buffer, BUFFER_LENGTH);
        Serial.print("Sum Diff.: ");
        Serial.println(float(sum-old_sum));
        old_sum = sum;
        return sum / BUFFER_LENGTH;
    }   

    float sum_array(float array[], int size) {
        float sum = 0;
        for (int i = 0; i < size; i++) {
            sum += array[i];
        }
        return sum;
    }
};

#endif