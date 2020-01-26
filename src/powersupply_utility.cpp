/***
 * 
 * Author : Francis Alonzo
 * 
 * Utilities for power supply
 * 
 ***/

#include "powersupply_utility.h"

float_t calcul_tension(double_t value)
{
    return (3.3*value*(R1+R2))/R2;
}

void putFloatInArray(uint8_t * array, float_t value)
{
    union{
        uint8_t sending[4];
        float_t data;
    } temp;

    temp.data = value;

    for(uint8_t i{};i<4;++i)
    {
        array[i] = temp.sending[i];
    }
}

void putCharInArray(uint8_t * array, char * data, float_t multiplicator)
{
    float_t value = putCharInFloat(data,multiplicator);
    putFloatInArray(array,value);
}

float_t putCharInFloat(char * data, float_t multiplicator)
{
    float_t value =0;

    for(uint8_t i=15;i>0;--i)
    {
        value += pow(data[i],15-i);
    }

    return value*multiplicator;
}