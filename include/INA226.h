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
class INA226 {
public:
    /** init INA226 class
     * @param *i2c pointer to I2C serial interface
     * @param addr sensor I2C address
     */
    INA226 (I2C* i2c, char addr); 

    /** Set the configuration of the device
     * @param reg desired Configure Register bits
     */
    void setConfig (unsigned short reg);
    
    /** Get device Configuration register
     */  
    unsigned short getConfig();
    
    /** Get the voltage over the shunt
    */
    float getShuntVolt();

    /** Get the voltage of the bus rail
     */
    float getBusVolt();
    
    /** Get the current measured through the shunt.
     * in order to get the current you need to set the shut resistor value first
     */  
    float getCurrent();
    
    float getPower();
    
    /** Set the Calibartion register 
    */
    void setCalibration(unsigned short reg);
    
    /** Get the Calibration register
    */
    unsigned short getCalibration();
    
    /** Set the mask/enable Register
    */
    void setMaskEnable(unsigned short reg);
    
    /** Get the mask/enable Register
    */
    unsigned short getMaskEnable();
    
    void setAlertLimit(unsigned short reg);
    
    unsigned short getAlertLimit();
    
    /** Get the unique ID of the device
    */
    unsigned short getID();
    
    /** Set the shunt resistor
    *@param val value in ohm
    */
    void setShuntRValue(float val);
    
    float getShuntRValue(void);
    
    /** Set the current LSB for maximum current in decimal
    */
    void setCurrentLSB(float val);

    float getCurrentLSB(float val);
    
protected:


private:
    char _addr;
    I2C *_i2c;
    float ShuntR;
    float CURR_LSB;
    
    
    
};
    
#endif