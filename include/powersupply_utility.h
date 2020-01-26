#ifndef POWERSUPPLY_UTILITY_H
#define POWERSUPPLY_UTILITY_H

#define R1 100000
#define R2 10000

#include "mbed.h"
#include "rtos.h"
#include "pinDef.h"

// Function to calculate voltage
float_t calcul_tension(double_t value);

// Put data in array
void putFloatInArray(uint8_t * array, float_t value);

// Put I2C data in array
// Possibility to multiply the value from I2C with a multiplicator (use for BusVoltage and Current)
void putCharInArray(uint8_t * array, char * data, float_t multiplicator = 1);

// Transform I2C data in a float
// Possibility to multiply the value from I2C with a multiplicator
float_t putCharInFloat(char * data, float_t multiplicator = 1);

#endif 