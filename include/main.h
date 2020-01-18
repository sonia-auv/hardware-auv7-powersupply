#ifndef MAIN_H
#define MAIN_H

#include "mbed.h"
#include "rtos.h"
#include "pinDef.h"
#include "powersupply_utility.h"
#include "INA226.h"
#include "RS485/RS485.h"
#include "RS485/RS485_definition.h"

int adress12v = 10000100;
#define adress16V1 0x41
int adress16V2 = 10000000;

//int registreConfig = 00000000;
//int registreShuntVoltage = 00000001;
char registreBusVoltage[8] = {0,0,0,0,0,0,1,0};
//int registrePower = 00000011;
char registreCurrent[8] = {0,0,0,0,0,1,0,0};
char registreCalibration[8] = {0,0,0,0,0,1,0,1};

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

#endif