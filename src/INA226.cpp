/*
 * INA226 voltage/temprature monitor library
 *
 *
 * Copyright (c) 2013 Davy Van Belle, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/** @file
 * @brief LTC29990 I2C
 */
 
#include "mbed.h"
#include "INA226.h"

INA226::INA226 (I2C* i2c, char addr)
{
    _i2c = i2c;
    _addr = addr;

    ShuntR = 0;
    CURR_LSB = 0;
    
}

void INA226::setConfig (unsigned short reg)
{
    char cmd[3];
    cmd[0] = CONF;
    cmd[1] = (char) ((reg & 0xFF00) >> 8);
    cmd[2] = (char) (reg & 0x00FF);
    _i2c->write(_addr,cmd,3);
}    

unsigned short INA226::getConfig()
{
    char cmd;
    char buff[2];
    
    cmd = CONF;
    
    _i2c->write(_addr,&cmd,1,true);
    _i2c->read(_addr+1,buff,2);
    return (buff[0] << 8) | buff[1];
}

float INA226::getShuntVolt()
{
    char cmd;
    char buff[2];
    bool sign;
    short tmp;
    
    cmd = VSHUNT;
    
    _i2c->write(_addr,&cmd,1,true);
    _i2c->read(_addr+1,buff,2);
    
    sign = buff[0] & 0x80;
    tmp = ((buff[0] & 0x7F) << 8) | buff[1];
    if(!sign) return (float)tmp*SHUNT_LSB;                   //positive calulation.
    else return ((float)tmp-32767)*SHUNT_LSB;                 //negative calculation.
}

float INA226::getBusVolt()
{
    char cmd;
    char buff[2];
    bool sign;
    short tmp;
    
    cmd = VBUS;
    
    _i2c->write(_addr,&cmd,1,true);
    _i2c->read(_addr+1,buff,2);
    
    sign = buff[0] & 0x80;
    tmp = ((buff[0] & 0x7F) << 8) | buff[1];
    if(!sign) return ((float)tmp)*BUS_LSB;                   //positive calulation.
    else return ((float)tmp-32767)*BUS_LSB;                 //negative calculation.
}

float INA226::getCurrent()
{
    char cmd;
    char buff[2];
    bool sign;
    short tmp;
    
    cmd = CURRENT;
    
    _i2c->write(_addr,&cmd,1,true);
    _i2c->read(_addr+1,buff,2);
    
    sign = buff[0] & 0x80;
    tmp = ((buff[0] & 0x7F) << 8) | buff[1];
    if(!sign) return (float)tmp*CURR_LSB;                   //positive calulation.
    else return ((float)tmp-32767)*CURR_LSB;                 //negative calculation.
}

float INA226::getPower()
{
    char cmd;
    char buff[2];
    bool sign;
    short tmp;
    
    cmd = POWER;
    
     _i2c->write(_addr,&cmd,1,true);
    _i2c->read(_addr+1,buff,2);
    
    sign = buff[0] & 0x80;
    tmp = ((buff[0] & 0x7F) << 8) | buff[1];
    if(!sign) return (float)tmp*25*CURR_LSB;                   //positive calulation.
    else return ((float)tmp-32767)*25*CURR_LSB;                 //negative calculation.
}

void INA226::setCalibration(unsigned short reg)
{
    char cmd[3];
    cmd[0] = CAL;
    cmd[1] = (char) ((reg & 0xFF00) >> 8);
    cmd[2] = (char) (reg & 0x00FF);
    _i2c->write(_addr,cmd,3);
} 

unsigned short INA226::getCalibration()
{
    char cmd;
    char buff[2];
    
    cmd = CAL;
    
     _i2c->write(_addr,&cmd,1,true);
    _i2c->read(_addr+1,buff,2);
    return (buff[0] << 8) | buff[1];
}

void INA226::setMaskEnable(unsigned short reg)
{
    char cmd[3];
    cmd[0] = MASK;
    cmd[1] = (char) ((reg & 0xFF00) >> 8);
    cmd[2] = (char) (reg & 0x00FF);
    _i2c->write(_addr,cmd,3);
} 

unsigned short INA226::getMaskEnable()
{
    char cmd;
    char buff[2];
    
    cmd = MASK;
    
     _i2c->write(_addr,&cmd,1,true);
    _i2c->read(_addr+1,buff,2);
    return (buff[0] << 8) | buff[1];
}

void INA226::setAlertLimit(unsigned short reg)
{
    char cmd[3];
    cmd[0] = ALERT;
    cmd[1] = (char) ((reg & 0xFF00) >> 8);
    cmd[2] = (char) (reg & 0x00FF);
    _i2c->write(_addr,cmd,3);
} 

unsigned short INA226::getAlertLimit()
{
    char cmd;
    char buff[2];
    
    cmd = ALERT;
    
     _i2c->write(_addr,&cmd,1,true);
    _i2c->read(_addr+1,buff,2);
    return (buff[0] << 8) | buff[1];
}

unsigned short INA226::getID()
{
    char cmd;
    char buff[2];
    
    cmd = DIE_ID;
    
     _i2c->write(_addr,&cmd,1,true);
    _i2c->read(_addr+1,buff,2);
    return (buff[0] << 8) | buff[1];
}

void INA226::setShuntRValue(float val)
{
    ShuntR = val;
}

float INA226::getShuntRValue(void)
{
    return ShuntR;
}


void INA226::setCurrentLSB(float val)
{
    CURR_LSB = val;
}

float INA226::getCurrentLSB(float val)
{
    return CURR_LSB;
}
