/***
 * 
 * Author : Francis Alonzo
 * 
 * Firmware_PSU
 * 
 ***/

#define USE_KILL_SIGNAL_HIGH
#include "main.h"

void initLedFunction();
void checkMask(INA226 sensor);
void ledFeedbackFunction(double_t batt_voltage);
bool isKillSwitchActivated();

void ledFeedbackFunction(double_t batt_voltage)      //  Logique des LEDs est inversée 0 pour allumer et 1 pour éteindre
{
  motor_state_t motor_state_cpy[NB_MOTORS];

  motor_state.mutex.lock();
  for(int i=0; i<NB_MOTORS; i++) {motor_state_cpy[i] = motor_state.state[i];}
  motor_state.mutex.unlock();

  if(batt_voltage > 0.462)                               // Full - 16,4V
  {
    LedBatt1 = 0;
    LedBatt2 = 0;
    LedBatt3 = 0;
    LedBatt4 = 1;
  }
  else if (batt_voltage <= 0.462 && batt_voltage > 0.445)       // 16,4V - 15,8V 
  {
    LedBatt1 = 1;
    LedBatt2 = 0;
    LedBatt3 = 0;
    LedBatt4 = 1;
  }
  else if (batt_voltage <= 0.445 && batt_voltage > 0.433)      // 15,8V - 15,4V
  {
    LedBatt1 = 1;
    LedBatt2 = 1;
    LedBatt3 = 0;
    LedBatt4 = 1;
  }
  else                                           // 15,4V - 0V
  {
    LedBatt1 = 1;
    LedBatt2 = 1;
    LedBatt3 = 1;
    LedBatt4 = 0;
  }

  LedKillswitch = (isKillSwitchActivated())? 0: 1;// Double inversion

  for(uint8_t i = 0; i < NB_MOTORS; ++i)
  {
    if(motor_state_cpy[i] == MOTOR_ON)
    {
      LedMotor[i] = 0x01;
    }
    else
    {
      LedMotor[i] = 0x00;
    }
  }
}

bool isKillSwitchActivated()
{
  return kill_input == KILL_ACTIVATION_STATUS;
}

void readSensorCallback()
{
  uint8_t cmd_array[2]={CMD_VOLTAGE, CMD_CURRENT};
  uint8_t voltage_send[255]={0};
  uint8_t current_send[255]={0};
  uint8_t nb_bytes_voltage = nb_alimentation*4;
  uint8_t nb_bytes_current = nb_sensor*4;
  double_t voltage, current, batt;

  while(true)
  {
    for(uint8_t i = 0; i < nb_sensor; ++i)
    {
      checkMask(sensor[i]);
      voltage = sensor[i].getBusVolt();
      current = sensor[i].getCurrent();

      putFloatInArray(voltage_send, voltage, i*4);
      putFloatInArray(current_send, current, i*4);
    }

    batt = 0.0;
    for(uint8_t i = 0; i < nb_samples; ++i)
    {
      batt += Battery_16V.read();                  // Valeur de la batterie donnée avec un test pratique (voir Excel)
      ThisThread::sleep_for(battery_delay);
    }
    batt = batt / (double_t)nb_samples;
    putFloatInArray(voltage_send, calcul_tension(batt, 3.3, 100000, 10000), 3*4); // Offset pour le tableau de la tension

    ledFeedbackFunction(batt);
    rs.write(PSU_ID, cmd_array[0], nb_bytes_voltage, voltage_send);
    rs.write(PSU_ID, cmd_array[1], nb_bytes_current, current_send);
    ThisThread::sleep_for(500);
  }  
}

void receiveMotorEnableRequestCallback()
{
  uint8_t cmd_array[1] = {CMD_ACT_MOTOR};
  uint8_t receive[255];
  uint8_t nb_cmd = 1;
  uint8_t size_cmd = NB_MOTORS;

  while(true)
  {
    if(rs.read(cmd_array, nb_cmd, receive) == size_cmd)
    {
      enable_motor_request.mutex.lock();
      for(uint8_t i = 0; i < NB_MOTORS; ++i)
      {
          enable_motor_request.request[i] = receive[i] & 0x01;
      }
      enable_motor_request.mutex.unlock();
    }
  }
}

void readMotorStatusCallback()
{
  uint8_t cmd_array[1] = {CMD_READ_MOTOR};
  uint8_t send[255] = {0};
  uint8_t nb_bytes = NB_MOTORS;
  uint8_t motor_failure_state_cpy[NB_MOTORS] = {0};
  uint8_t enable_motor_resquest_cpy[NB_MOTORS] = {0};

  while(true)
  {
    // get the motor request from the user for all motor
    enable_motor_request.mutex.lock();
    for(uint8_t i = 0; i < NB_MOTORS; i++) { enable_motor_resquest_cpy[i] = enable_motor_request.request[i];}
    enable_motor_request.mutex.unlock();

    //get motor faillure state and set message
    motor_failure_state.mutex.lock();
    for(uint8_t i = 0; i < NB_MOTORS; ++i)
    {
      motor_failure_state.state[i] = ~(status_motor[i]) & 0x01;
      motor_failure_state_cpy[i] = motor_failure_state.state[i]; 
    }
    motor_failure_state.mutex.unlock();

    //Set the send message
    for(uint8_t i = 0; i < NB_MOTORS; ++i)
    {
      if(motor_failure_state_cpy[i] == 1) send[i] = 0x02;
      else send[i] = enable_motor_resquest_cpy[i];
    }

    rs.write(PSU_ID, cmd_array[0], nb_bytes, send);
    ThisThread::sleep_for(1000);
  }
}

void motorControllerCallback()
{
    uint8_t failure_status_motor[NB_MOTORS] = {0};
    uint8_t enable_motor_request_copy[NB_MOTORS]= {0};
    motor_state_t motor_state_copy[NB_MOTORS];

    while(true)
    {
      //get failure status of all motor
      motor_failure_state.mutex.lock();
      for(uint8_t i = 0; i < NB_MOTORS; i++){ failure_status_motor[i] = motor_failure_state.state[i];}
      motor_failure_state.mutex.unlock();

      //get user request of all motor
      enable_motor_request.mutex.lock();
      for(uint8_t i = 0; i < NB_MOTORS; i++){ enable_motor_request_copy[i] = enable_motor_request.request[i];}
      enable_motor_request.mutex.unlock();
        
      //set enable status depending on the state of the kill stwitch and on the motor error code
      for(uint8_t i = 0; i < NB_MOTORS; ++i)
      {
        if(isKillSwitchActivated())
        {
          enable_motor[i] = 0;
          motor_state_copy[i] = (enable_motor_request_copy[i]) ? MOTOR_ON : MOTOR_OFF;
        }
        else if(failure_status_motor[i] == 1)
        {
          motor_state_copy[i] = MOTOR_FAILURE;
          enable_motor[i] = 0;//(enable_motor_request_copy[i]) ? MOTOR_ON : MOTOR_OFF; (to test)
        }
        else
        {
          enable_motor[i] = enable_motor_request_copy[i];
          motor_state_copy[i] = (enable_motor_request_copy[i]) ? MOTOR_ON : MOTOR_OFF;
        }
      }

      motor_state.mutex.lock();
      for(int i=0; i<NB_MOTORS; i++) { motor_state.state[i] = motor_state_copy[i];}
      motor_state.mutex.unlock();

      ThisThread::sleep_for(500);
    }
}

// void TemperatureRead()
// {
//   uint8_t cmd_array[1]={CMD_PS_temperature};
//   uint8_t temp_receive[255]={0};
//   uint8_t temp_send[255]={0};
//   uint8_t nb_command = 1;
//   uint8_t nb_byte_send = 4;
//   float_t temp;

//   while(true)
//   {
//     rs.read(cmd_array,nb_command,temp_receive);
//     temp = temperature.getTemp();
//     putFloatInArray(temp_send,temp);
//     rs.write(PSU_ID,cmd_array[0],nb_byte_send,temp_send);
//   }
// }

void initLedFunction()
{
  LedBatt4 = 0;
  ThisThread::sleep_for(delay);
  LedBatt3 = 0;
  ThisThread::sleep_for(delay);
  LedBatt2 = 0;
  ThisThread::sleep_for(delay);
  LedBatt1 = 0;
  ThisThread::sleep_for(delay);
  LedKillswitch = 0;
  ThisThread::sleep_for(delay);
  LedMotor[0] = 0;
  ThisThread::sleep_for(delay);
  LedMotor[1] = 0;
  ThisThread::sleep_for(delay);
}

void checkMask(INA226 sensor)
{
  uint8_t dataready = 0;

  while(dataready == 0)
  {
    dataready = ((sensor.getMaskEnable() >> 3) & 0x01);
    ThisThread::sleep_for(20);
  }
}

int main()
{
  LedBatt1 = 1;
  LedBatt2 = 1;
  LedBatt3 = 1;
  LedBatt4 = 1;
  LedKillswitch = 1;
  LedMotor[0] = 1;
  LedMotor[1] = 1;

  //  On s'assure que les 2 moteurs sont éteints
  for(uint8_t i = 0; i < NB_MOTORS; ++i)
  {
    enable_motor[i] = 0;
  }

  sensor[0].setConfig(CONFIG);
  sensor[0].setCalibration(CALIBRATION);
  sensor[0].setCurrentLSB(CURRENTLSB);
  sensor[1].setConfig(CONFIG);
  sensor[1].setCalibration(CALIBRATION);
  sensor[1].setCurrentLSB(CURRENTLSB);
  sensor[2].setConfig(CONFIG);
  sensor[2].setCalibration(CALIBRATION);
  sensor[2].setCurrentLSB(CURRENTLSB);

  ThisThread::sleep_for(rand() % 100); // Petit Délai pour éviter des collisions avec RS485

  initLedFunction();

  sensorRead.start(readSensorCallback);
  sensorRead.set_priority(osPriorityHigh2);

  readMotorStatus.start(readMotorStatusCallback);
  readMotorStatus.set_priority(osPriorityHigh);

  motorController.start(motorControllerCallback);
  motorController.set_priority(osPriorityHigh1);

  motorEnableRqst.start(receiveMotorEnableRequestCallback);
  motorEnableRqst.set_priority(osPriorityHigh);

  thread_isAlive.start(callback(isAliveThread, &rs));
  thread_isAlive.set_priority(osPriorityNormal1);

  // threadtemperature.start(TemperatureRead);
  // threadtemperature.set_priority(osPriorityHigh);
}