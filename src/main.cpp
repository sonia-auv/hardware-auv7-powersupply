/***
 * 
 * Author : Francis Alonzo
 * 
 * Firmware_PSU
 * 
 ***/

#include "main.h"

void initLedFunction();
void checkMask(INA226 sensor);
void ledFeedbackFunction(double_t batt_voltage);

void ledFeedbackFunction(double_t batt_voltage)      //  Logique des LEDs est inversée 0 pour allumer et 1 pour éteindre
{
  while(true)
  {
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
    if(Killswitch == 0)                       // Double inversion
    {
      LedKillswitch = 1;
    }
    else
    {
      LedKillswitch = 0;
    }
    if(RunMotor1.read())
    {
      LedStatusV1 = 0;
    }
    else
    {
      LedStatusV1 = 1;
    }
    if(RunMotor2.read())
    {
      LedStatusV2 = 0;
    }
    else
    {
      LedStatusV2 = 1;
    }
  }
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

void enableMotorCallback()
{
  uint8_t cmd_array[1]={CMD_ACT_MOTOR};
  uint8_t motor_receive[255]={0};
  uint8_t motor_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 2;

  while(true)
  {
    if(rs.read(cmd_array, nb_command, motor_receive) == 2)
    {
      enableMotor1 = motor_receive[0];
      enableMotor2 = motor_receive[1];
      ThisThread::sleep_for(500);
      motor_send[0] = RunMotor1;
      motor_send[1] = RunMotor2;
    }
    rs.write(PSU_ID, cmd_array[0], nb_byte_send, motor_send);
  }
}

void readMotorCallback()
{
  uint8_t cmd_array[1]={CMD_READ_MOTOR};
  uint8_t motor_send[255]={0};
  uint8_t nb_byte_send = 2;

  while(true)
  {
    motor_send[0] = RunMotor1;
    motor_send[1] = RunMotor2;
    rs.write(PSU_ID, cmd_array[0], nb_byte_send, motor_send);
    ThisThread::sleep_for(2500);
  }  
}

void emergencyStopCallBack()
{
  while(true)
  {
    if(Killswitch == 1)
    {
      RunMotor1 = 0;
      RunMotor2 = 0;
    }
    else if (Killswitch == 0)
    {
      RunMotor1 = enableMotor1;
      RunMotor2 = enableMotor2;
    }
    ThisThread::sleep_for(200);
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
  LedStatusV1 = 0;
  ThisThread::sleep_for(delay);
  LedStatusV2 = 0;
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
  LedStatusV1 = 1;
  LedStatusV2 = 1;
  RunMotor1 = 0;            //  On s'assure que les 2 moteurs sont éteints
  RunMotor2 = 0;

  sensor[0].setConfig(CONFIG);
  sensor[0].setCalibration(CALIBRATION);
  sensor[0].setCurrentLSB(CURRENTLSB);
  sensor[1].setConfig(CONFIG);
  sensor[1].setCalibration(CALIBRATION);
  sensor[1].setCurrentLSB(CURRENTLSB);
  sensor[2].setConfig(CONFIG);
  sensor[2].setCalibration(CALIBRATION);
  sensor[2].setCurrentLSB(CURRENTLSB);

  initLedFunction();

  sensorRead.start(readSensorCallback);
  sensorRead.set_priority(osPriorityHigh);

  activationMotor.start(enableMotorCallback);
  activationMotor.set_priority(osPriorityHigh);

  readMotor.start(readMotorCallback);
  readMotor.set_priority(osPriorityHigh);

  threademergencystop.start(emergencyStopCallBack);
  threademergencystop.set_priority(osPriorityAboveNormal1);

  // threadtemperature.start(TemperatureRead);
  // threadtemperature.set_priority(osPriorityHigh);
}