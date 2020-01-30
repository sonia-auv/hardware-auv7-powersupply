#ifndef TC74A5_H
#define TC74A5_H

#include "mbed.h"

#define CONFIGT 0x01
#define TEMP 0x00

class TC74A5 
{
private :
    char adrr; 
    I2C * _i2c;

public :
    
    TC74A5(I2C * i2c, char address);

    char getTemp();

};

#endif