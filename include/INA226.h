/*
 * Texas Instruments INA226 voltage/ current/ power monitor library
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
 * @brief INA226 I2C
 */
 
#ifndef INA226_H
#define INA226_H

#include "mbed.h"

#define CONF 0x00

#define VSHUNT 0x01
#define VBUS 0x02
#define POWER 0x03
#define CURRENT 0x04
#define CAL 0x05
#define MASK 0x06
#define ALERT 0x07

#define DIE_ID 0xFF

#define SHUNT_LSB 0.0000025
#define BUS_LSB 0.00125



/** INA226 class 
 */
class INA226 
{
    
public:

    /** 
     * @brief INA226 class constructor.
     * 
     * @param i2c pointer to I2C serial interface
     * @param addr sensor I2C address
     */
    INA226 (I2C* i2c, char addr); 

    /** 
     * @brief Set the configuration of the device.
     * 
     * @param reg desired Configure Register bits.
     */
    void setConfig (unsigned short reg);
    
    /** 
     * @brief Get device Configuration register.
     * 
     * @return unsigned short
     */  
    unsigned short getConfig();
    
    /** 
     * @brief Get the voltage over the shunt.
     * 
     * @return float
     */
    float getShuntVolt();

    /** 
     * @brief Get the voltage of the bus rail.
     * 
     * @return float
     */
    float getBusVolt();
    
    /** 
     * @brief Get the current measured through the shunt.
     * 
     * @warning In order to get the current you need to set the shut resistor value first.
     * @return float
     */  
    float getCurrent();
    
    /**
     * @brief Get the Power object
     * 
     * @return float 
     */
    float getPower();
    
    /** 
     * @brief Set the Calibartion register.
     * 
     * @param reg Register value.
     */
    void setCalibration(unsigned short reg);
    
    /** 
     * @brief Get the Calibration register.
     * 
     * @return unsigned short
     */
    unsigned short getCalibration();
    
    /** 
     * @brief Set the mask/enable Register.
     * 
     */
    void setMaskEnable(unsigned short reg);
    
    /** 
     * @brief Get the mask/enable Register.
     * 
     * @return unsigned short
     */
    unsigned short getMaskEnable();
    
    /**
     * @brief Set the Alert Limit object.
     * 
     * @param reg Register value.
     */
    void setAlertLimit(unsigned short reg);
    
    /**
     * @brief Get the Alert Limit object.
     * 
     * @return unsigned short 
     */
    unsigned short getAlertLimit();
    
    /** 
     * @brief Get the unique ID of the device.
     * 
     * @return unsigned short
     */
    unsigned short getID();
    
    /** 
     * @brief Set the shunt resistor.
     * 
     * @param val Value in ohm.
     */
    void setShuntRValue(float val);
    
    /**
     * @brief Get the Shunt R Value object.
     * 
     * @return float 
     */
    float getShuntRValue(void);
    
    /** 
     * @brief Set the current LSB for maximum current in decimal.
     * 
     * @param val the LSB value.
     */
    void setCurrentLSB(float val);

    /**
     * @brief Get the Current LSB object.
     * 
     * @return float 
     */
    float getCurrentLSB();
    
protected:


private:
    char _addr;
    I2C *_i2c;
    float ShuntR;
    float CURR_LSB;
    
    
    
};
    
#endif