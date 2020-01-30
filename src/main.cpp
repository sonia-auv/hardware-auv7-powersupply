/***
 * 
 * Author : Francis Alonzo
 * 
 * Firmware_PSU
 * 
 ***/

#include "main.h"

#define PSU_ID SLAVE_powersupply0

Thread feedbackPSU;
Thread threadbattery4s;
Thread thread12v;
Thread thread12c;
Thread threadmotor1v;
Thread threadmotor1c;
Thread threadmotor2v;
Thread threadmotor2c;
Thread threadmotor1toggle;
Thread threadmotor2toggle;
Thread threadmotor1read;
Thread threadmotor2read;

I2C i2c_bus(I2CSDA,I2CSCL);
INA226 sensorm1(&i2c_bus, adress16V1);
INA226 sensorm2(&i2c_bus, adress16V2);
INA226 sensor12v(&i2c_bus, adress12v);
TC74A5 temperature(&i2c_bus, adressTemp);

void ledfeedbackFunction()      //  Logique des LEDs est inversée 0 pour allumer et 1 pour éteindre
{
  double_t value;               //  Un peu de sauce pour le fun
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

  while(true)
  {
    value = Battery_16V.read();                    // Valeur de la batterie donnée avec un test pratique (voir Excel)
    if(value > 0.462)                               // Full - 16,4V
    {
      LedBatt1 = 0;
      LedBatt2 = 0;
      LedBatt3 = 0;
      LedBatt4 = 0;
    }
    else if (value <= 0.462 && value > 0.445)       // 16,4V - 15,8V 
    {
      LedBatt1 = 1;
      LedBatt2 = 0;
      LedBatt3 = 0;
      LedBatt4 = 0;
    }
    else if (value <= 0.445 && value > 0.433)      // 15,8V - 15,4V
    {
      LedBatt1 = 1;
      LedBatt2 = 1;
      LedBatt3 = 0;
      LedBatt4 = 0;
    }
    else                                           // 15,4V - Sors la batterie caliss
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
    ThisThread::sleep_for(1000);
  }
}

void Battery4SVoltage()
{
  uint8_t cmd_array[1] = {CMD_PS_VBatt};
  uint8_t battery_receive[255]= {0};
  uint8_t battery_send[255]= {0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 4;
  float_t voltage_battery;

  while(true)
  {
    RS485::read(cmd_array,nb_command,battery_receive);
    if(battery_receive[0]==1)
    {
      voltage_battery = calcul_tension(Battery_16V.read());
      putFloatInArray(battery_send,voltage_battery);
      RS485::write(PSU_ID,cmd_array[0],nb_byte_send,battery_send);
    }
  }
}

void Supply12vVoltage()
{
  uint8_t cmd_array[1]={CMD_PS_V12};
  uint8_t voltage12v_receive[255]={0};
  uint8_t voltage12v_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 4;
  float_t voltage12v;

  while(true)
  {
    RS485::read(cmd_array,nb_command,voltage12v_receive);
    if(voltage12v_receive[0]==1)
    {
      voltage12v = sensor12v.getBusVolt();
      putFloatInArray(voltage12v_send,voltage12v);
      RS485::write(PSU_ID,cmd_array[0],nb_byte_send,voltage12v_send);
    }
  }
}

void Supply12vCurrent()
{
  uint8_t cmd_array[1]={CMD_PS_C12};
  uint8_t voltage12v_receive[255]={0};
  uint8_t voltage12v_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 4;
  float_t current12v;

  while(true)
  {
    RS485::read(cmd_array,nb_command,voltage12v_receive);
    if(voltage12v_receive[0]==1)
    {
      current12v = sensor12v.getCurrent();
      putFloatInArray(voltage12v_send,current12v);
      RS485::write(PSU_ID,cmd_array[0],nb_byte_send,voltage12v_send);
    }
  }
}

void Motor1Voltage()
{
  uint8_t cmd_array[1]={CMD_PS_V16_1};
  uint8_t motor1_receive[255]={0};
  uint8_t motor1_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 4;
  float_t m1voltage;

  while(true)
  {
    RS485::read(cmd_array,nb_command,motor1_receive);
    if(motor1_receive[0]==1)
    {
      m1voltage = sensorm1.getBusVolt();
      putFloatInArray(motor1_send,m1voltage);
      RS485::write(PSU_ID,cmd_array[0],nb_byte_send,motor1_send);
    }
  }
}

void Motor1Current()
{
  uint8_t cmd_array[1]={CMD_PS_C16_1};
  uint8_t motor1_receive[255]={0};
  uint8_t motor1_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 4;
  float_t currentm1;

  while(true)
  {
    RS485::read(cmd_array,nb_command,motor1_receive);
    if(motor1_receive[0]==1)
    {
      currentm1 = sensorm1.getCurrent();
      putFloatInArray(motor1_send,currentm1);
      RS485::write(PSU_ID,cmd_array[0],nb_byte_send,motor1_send);
    }
  }
}

void Motor2Voltage()
{
  uint8_t cmd_array[1]={CMD_PS_V16_2};
  uint8_t motor2_receive[255]={0};
  uint8_t motor2_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 4;
  float_t voltagem2;

  while(true)
  {
    RS485::read(cmd_array,nb_command,motor2_receive);
    if(motor2_receive[0]==1)
    {
      voltagem2 = sensorm2.getBusVolt();
      putFloatInArray(motor2_send,voltagem2);
      RS485::write(PSU_ID,cmd_array[0],nb_byte_send,motor2_send);
    }
  }
}

void Motor2Current()
{
  uint8_t cmd_array[1]={CMD_PS_C16_2};
  uint8_t motor2_receive[255]={0};
  uint8_t motor2_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 4;
  float_t currentm2;

  while(true)
  {
    RS485::read(cmd_array,nb_command,motor2_receive);
    if(motor2_receive[0]==1)
    {
      currentm2 = sensorm2.getCurrent();
      putFloatInArray(motor2_send,currentm2);
      RS485::write(PSU_ID,cmd_array[0],nb_byte_send,motor2_send);
    }
  }
}

void Motor1Toggle()
{
  uint8_t cmd_array[1]={CMD_PS_ACT_16V_1};
  uint8_t motor1_receive[255]={0};
  uint8_t motor1_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 1;

  while(true)
  {
    RS485::read(cmd_array,nb_command,motor1_receive);
    if(motor1_receive[0]==1)
    {
      RunMotor1 = 1;
      motor1_send[0]=1;
    }
    else
    {
      RunMotor1 = 0;
      motor1_send[0]=0;
    }
    RS485::write(PSU_ID,cmd_array[0],nb_byte_send,motor1_send);
  }
}

void Motor2Toggle()
{
  uint8_t cmd_array[1]={CMD_PS_ACT_16V_2};
  uint8_t motor2_receive[255]={0};
  uint8_t motor2_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 1;

  while(true)
  {
    RS485::read(cmd_array,nb_command,motor2_receive);
    if(motor2_receive[0]==1)
    {
      RunMotor2 = 1;
      motor2_send[0]=1;
    }
    else
    {
      RunMotor2 = 0;
      motor2_send[0]=0;
    }
    RS485::write(PSU_ID,cmd_array[0],nb_byte_send,motor2_send);
  }
}

void Motor1Read()
{
  uint8_t cmd_array[1]={CMD_PS_CHECK_16V_1};
  uint8_t motor1_receive[255]={0};
  uint8_t motor1_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 1;

  while(true)
  {
    RS485::read(cmd_array,nb_command,motor1_receive);
    if(motor1_receive[0]==1)
    {
      if(StatusMotor1 == 1)
      {
        motor1_send[0] = 1;
      }
      else
      {
        motor1_send[0] = 0;
      }
      RS485::write(PSU_ID,cmd_array[0],nb_byte_send,motor1_send);
    }
  }
}

void Motor2Read()
{
  uint8_t cmd_array[1]={CMD_PS_ACT_16V_2};
  uint8_t motor2_receive[255]={0};
  uint8_t motor2_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 1;

  while(true)
  {
    RS485::read(cmd_array,nb_command,motor2_receive);
    if(motor2_receive[0]==1)
    {
      if(StatusMotor2 == 1)
      {
        motor2_send[0] = 1;
      }
      else
      {
        motor2_send[0] = 0;
      }
      RS485::write(PSU_ID,cmd_array[0],nb_byte_send,motor2_send);
    }
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
  Run12v = 1;               //  Allume le regulateur 12V
  RunMotor1 = 0;            //  On s'assure que les 2 moteurs sont éteints
  RunMotor2 = 0;

  short temp;

  while (1)
  {
    temp = (short) temperature.getTemp();

    if (temp)
    {
      LedBatt3 = 0;
    }

    ThisThread::sleep_for(1000);
  }
  //RS485::init(PSU_ID);

  /*float thing;
  sensor12v.setConfig(CONFIG);
  ThisThread::sleep_for(delay);
  sensor12v.setCalibration(CALIBRATION);
  sensor12v.setCurrentLSB(CURRENTLSB);
  ThisThread::sleep_for(delay);
  thing = sensor12v.getCurrent();*/

  feedbackPSU.start(ledfeedbackFunction);
  feedbackPSU.set_priority(osPriorityAboveNormal1);

  threadbattery4s.start(Battery4SVoltage);
  threadbattery4s.set_priority(osPriorityHigh);

  thread12v.start(Supply12vVoltage);
  thread12v.set_priority(osPriorityAboveNormal3);

  thread12c.start(Supply12vCurrent);
  thread12c.set_priority(osPriorityAboveNormal3);

  threadmotor1v.start(Motor1Voltage);
  threadmotor1v.set_priority(osPriorityAboveNormal3);

  threadmotor1c.start(Motor1Current);
  threadmotor1c.set_priority(osPriorityAboveNormal3);

  threadmotor2v.start(Motor2Voltage);
  threadmotor2v.set_priority(osPriorityAboveNormal3);

  threadmotor2c.start(Motor2Current);
  threadmotor2c.set_priority(osPriorityAboveNormal3);

  threadmotor1toggle.start(Motor1Toggle);
  threadmotor1toggle.set_priority(osPriorityHigh);

  threadmotor2toggle.start(Motor2Toggle);
  threadmotor2toggle.set_priority(osPriorityHigh);

  threadmotor1read.start(Motor1Read);
  threadmotor1read.set_priority(osPriorityHigh);

  threadmotor2read.start(Motor2Read);
  threadmotor2read.set_priority(osPriorityHigh);
}