#include "IR_barrier.h"

/**
 * @brief Constructor function for a new sensor structure
 * 
 * @param pin_offset Sets the offset of the first Rx sensor
 * @param barrier_number Sets the number of barriers to read
 * 
 * @param pattern 
 * @param pattern_length 
 * @return Sensor* 
 */
Sensor newSensor( int pin_offset, size_t barrier_number,  int *pattern, size_t pattern_length){
    Sensor sensor;

    sensor.pin_offset = pin_offset;
    sensor.barrier_number = barrier_number;

    sensor.pattern_array = pattern;
    sensor.pattern_length = pattern_length;

    return sensor;
}

/**
 * @brief Trims the data received from the sensors port
 * according to the sensor struct offset and length
 * 
 * @param sensor 
 * @param data_value 
 * @return int 
 */
int trimOffset(Sensor *sensor, int data_value){
    int offset_value = sensor->pin_offset;
    int trimmed_value = (data_value<<offset_value);

    return trimmed_value;
}

int cmpStepValue (Sensor *sensor, int data_value){
    int current_step = sensor->pattern_index;

    if (current_step >= sensor->pattern_length-1){
        return 0;
    }

    int expected_value = sensor-> pattern_array+current_step;    

    if (expected_value == data_value)
    {
        /* code */
    }
    return 1;

}