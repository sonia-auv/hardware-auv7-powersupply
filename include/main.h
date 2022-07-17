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

#define NB_MOTORS 2

#if defined(USE_KILL_SIGNAL_HIGH)
    #define KILL_ACTIVATION_STATUS (1)
#elif defined(KILL_SWITCH_ACTIVE_LOW)
    #define USE_KILL_SIGNAL_LOW (0)
#else
    #error "Error: kill activation state not defined, plese define USE_KILL_SIGNAL_HIGH or USE_KILL_SIGNAL_LOW before including power_management.h"
#endif

typedef enum{
    MOTOR_ON,
    MOTOR_OFF,
    MOTOR_FAILURE,
    MOTOR_ERROR
} motor_state_t;

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
DigitalOut Run12v(RUN_12V);

DigitalOut LedMotor[NB_MOTORS] = {DigitalOut(PGOOD_MOTOR_V1), DigitalOut(PGOOD_MOTOR_V2)};
DigitalOut enable_motor[NB_MOTORS] = {DigitalOut(RUNMOTOR_V1), DigitalOut(RUNMOTOR_V2)};
DigitalIn status_motor[NB_MOTORS] = {DigitalIn(STATUSMOTOR_V1), DigitalIn(STATUSMOTOR_V2)}; // 0 = fault, 1 = normal 

DigitalIn kill_input(KILLSWITCH);
AnalogIn Battery_16V(INPUT_4S);

//###################################################
//             OBJECT DEFINITIONS
//###################################################

RS485 rs(PSU_ID);
I2C i2c_bus(I2CSDA,I2CSCL);
INA226 sensor[3] = {INA226(&i2c_bus, adress16V1), INA226(&i2c_bus, adress16V2), INA226(&i2c_bus, adress12v)};
TC74A5 temperature(&i2c_bus, adressTemp);

//###################################################
//             THREAD DEFINITIONS
//###################################################

Thread sensorRead;
Thread motorEnableRqst;
Thread readMotorStatus;
Thread motorController;
Thread thread_isAlive;

//###################################################
//             GLOBAL VARIABLE DEFINITIONS
//###################################################

typedef struct {
    uint8_t request[NB_MOTORS];
    Mutex mutex;
}enable_motor_request_t;

enable_motor_request_t enable_motor_request;

typedef struct {
    uint8_t state[NB_MOTORS];
    Mutex mutex;
}motor_failure_state_t;

motor_failure_state_t motor_failure_state;

typedef struct {
    motor_state_t state[NB_MOTORS];
    Mutex mutex;
}current_state_motors_t;
current_state_motors_t motor_state;

// Mutex mutexPWM;
// Mutex mutexStatusMotor;
// Mutex mutexEnableMotorRequest;
// Mutex mutexMotorState;

uint8_t enableMotor1;
uint8_t enableMotor2;

#endif