#ifndef MAIN_H
#define MAIN_H

#include "mbed.h"
#include "rtos.h"
#include "pinDef.h"
#include "Utility/Utility.h"
#include "INA226.h"
#include "TC74A5/TC74A5.h"
#include "RS485/RS485.h"
#include "RS485/RS485_definition.h"

#define delay 300
#define battery_delay 20

#define nb_sensor 3
#define nb_alimentation 4
#define nb_samples 10

// Power Supply Slave à définir ici (0 à 3)
#define PSU_ID SLAVE_PSU0

//###################################################
//              I2C SLAVE DEFINITION
//###################################################

#define adress12v (0x44 << 1)
#define adress16V1 (0x41 << 1)
#define adress16V2 (0x40 << 1)
#define adressTemp (0x4D << 1)

#define CONFIG      0x4527
#define CALIBRATION 0x15C3
#define CURRENTLSB  0.0004577636

//###################################################
//             PINOUT FONCTION DEFINITIONS
//###################################################

DigitalOut LedBatt1(LED_BATT1);
DigitalOut LedBatt2(LED_BATT2);
DigitalOut LedBatt3(LED_BATT3);
DigitalOut LedBatt4(LED_BATT4);
DigitalOut LedKillswitch(LED_KILLSWITCH);
DigitalOut LedStatusV1(PGOOD_MOTOR_V1);
DigitalOut LedStatusV2(PGOOD_MOTOR_V2);

DigitalOut Run12v(RUN_12V);
DigitalOut RunMotor1(RUNMOTOR_V1);
DigitalOut RunMotor2(RUNMOTOR_V2);

DigitalIn StatusMotor1(STATUSMOTOR_V1);
DigitalIn StatusMotor2(STATUSMOTOR_V2);
DigitalIn Killswitch(KILLSWITCH);

AnalogIn Battery_16V(INPUT_4S);

//###################################################
//             OBJECT DEFINITIONS
//###################################################

RS485 rs(PSU_ID);
I2C i2c_bus(I2CSDA,I2CSCL);
// INA226 sensorm1(&i2c_bus, adress16V1);
// INA226 sensorm2(&i2c_bus, adress16V2);
// INA226 sensor12v(&i2c_bus, adress12v);
INA226 sensor[3] = {INA226(&i2c_bus, adress16V1), INA226(&i2c_bus, adress16V2), INA226(&i2c_bus, adress12v)};
TC74A5 temperature(&i2c_bus, adressTemp);

//###################################################
//             THREAD DEFINITIONS
//###################################################

Thread sensorRead;
Thread activationMotor;
Thread readMotor;
Thread threademergencystop;
Thread thread_isAlive;

//###################################################
//             GLOBAL VARIABLE DEFINITIONS
//###################################################

uint8_t enableMotor1;
uint8_t enableMotor2;

#endif