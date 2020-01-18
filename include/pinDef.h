#ifndef PIN_DEF_H
#define PIN_DEF_H

//###################################################
//              BOARD PIN DEFINITION
//###################################################

#define KILLSWITCH PE_11

#define I2CSDA PB_11 //PD_13
#define I2CSCL PB_10 //PD_12

#define INPUT_4S PC_0

#define RUN_12V PD_4
#define RUNMOTOR_V1 PD_1
#define RUNMOTOR_V2 PD_3

#define STATUSMOTOR_V1 PD_0
#define STATUSMOTOR_V2 PD_2

#define LED_BATT1 PA_6
#define LED_BATT2 PA_5
#define LED_BATT3 PA_4
#define LED_BATT4 LED3 //PC_4

#define LED_KILLSWITCH PA_7
#define PGOOD_MOTOR_V1 PC_5
#define PGOOD_MOTOR_V2 PB_1

//###################################################
//              RS485 PIN DEFINITION
//###################################################

#define RS485_RX_PIN PA_10
#define RS485_TX_PIN PA_9

#define RS485_TE_PIN PA_12
#define RS485_DE_PIN PA_11
#define RS485_RE_PIN PA_8

#endif