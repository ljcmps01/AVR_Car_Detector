#ifndef IR_barrier_h
#define IR_barrier_h

#include <stdlib.h>

typedef struct{
  //Set up pin config
  int pin_offset;
  size_t barrier_number;

  //Pattern recognition
  size_t pattern_length;
  int pattern_index;
  int *pattern_array;

}Sensor;

Sensor *newSensor(int pin_offset, size_t barrier_number,  int *pattern, size_t pattern_length);

#endif