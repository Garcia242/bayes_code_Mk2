/* This file was not originally in the main branch; it is a non-working moving average filter,
implemented in the branch filter_time. I have placed it here just so that I can comment
on it. */

#include <vector>

#ifndef FILTER_H
#define FILTER_H

#define BUFFER_LENGTH 5000 // Number of data points to be averaged out (filtered).

/**
 * @brief Moving average filter class used to smooth out input data.
 */
class MovingAverage{

    public:
    float buffer[BUFFER_LENGTH]; // Array to store the data points to be filtered.
    float old_sum = 0; // Store the previous sum of the buffer array.

    /**
     * @brief Update the buffer with the newest value.
     * @param new_value: Latest data measurement.
     */
    void update_buffer(float new_value) {
      float sum1 = sum_array(buffer, BUFFER_LENGTH); // Current sum of the buffer array
      Serial.print(new_value);
      Serial.print("\t");
      Serial.print(buffer[BUFFER_LENGTH-1]);
      Serial.print("\t");
      Serial.print("Sum Diff: ");
      Serial.print(buffer[BUFFER_LENGTH-1]-new_value);
      Serial.print("\t");
      for (int i = 0; i < BUFFER_LENGTH-1; i++) {
        buffer[i+1] = buffer[i]; // Shift all elements in the buffer to the right
      }
      buffer[0] = new_value; // Insert latest data measurement at the start of the buffer
      float sum2 = sum_array(buffer, BUFFER_LENGTH); // Updated sum of the buffer array
      Serial.print("Sum Diff: ");
      Serial.println(float(sum2-sum1));
    }

    /**
     * @brief Initialise the buffer.
     * @param new_value: Latest/first data measurement to be filtered.
     */
    void init_buffer(float new_value) {
      update_buffer(new_value);
    }


    /**
     * @brief Filter the input data.
     * @param new_value: Latest data measurement.
     * @return The average value of the last BUFFER_LENGTH measurements.
     */
    float apply_filter(float new_value) {
        float sum = 0;
        update_buffer(new_value); // Update buffer with newest value
        sum = sum_array(buffer, BUFFER_LENGTH); // Calculate sum of buffer array
        Serial.print("Sum Diff.: ");
        Serial.println(float(sum-old_sum));
        old_sum = sum;
        return sum / BUFFER_LENGTH; 
    }   

    /**
     * @brief Compute the sum of elements in an array.
     * @param array[]: The array to be summed up.
     * @param size: The size of the array.
     * @return The sum of all elements in the array.
     */
    float sum_array(float array[], int size) {
        float sum = 0;
        for (int i = 0; i < size; i++) {
            sum += array[i];
        }
        return sum;
    }
};

#endif
