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
Thread thread12vtoggle;
Thread threadmotor1read;
Thread threadmotor2read;
Thread thread12vread;

/*void ledfeedbackFunction()
{
  double_t value;
  LedBatt1 = 0;
  ThisThread::sleep_for(300);
  LedBatt2 = 0;
  ThisThread::sleep_for(300);
  LedBatt3 = 0;
  ThisThread::sleep_for(300);
  LedBatt4 = 0;

  while(true)
  {
    value = Battery_16V.read();
    if(value > 0.93 && value <= 0.96)
    {
      LedBatt1 = 1;
      LedBatt2 = 0;
    }
    else if (value > 0.90 && value <= 0.93)
    {
      LedBatt2 = 1;
      LedBatt3 = 0;
    }
    else if (value <= 0.90)
    {
      LedBatt3 = 1;
    }
    else
    {
      LedBatt1 = 0;
    }
    if(Killswitch == 0)
    {
      LedKillswitch = 0;
    }
    else
    {
      LedKillswitch = 1;
    }
    if(StatusMotor1 == 1)
    {
      LedStatusV1 = 0;
    }
    else
    {
      LedStatusV1 = 1;
    }
    if(StatusMotor2 == 1)
    {
      LedStatusV2 = 0;
    }
    else
    {
      LedStatusV2 = 1;
    }
    ThisThread::sleep_for(2000);
  }
}

void Battery4SVoltage()
{
  uint8_t cmd_array[1] = {CMD_PS_VBatt};
  uint8_t battery_receive[255]= {0};
  uint8_t battery_send[255]= {0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 4;
  double_t voltage_battery;

  while(true)
  {
    RS485::read(cmd_array,nb_command,battery_receive);
    if(battery_receive[0]==1)
    {
      voltage_battery = calcul_tension(Battery_16V.read(),91000,22000);
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
  char voltage12v[16]={0};

  while(true)
  {
    RS485::read(cmd_array,nb_command,voltage12v_receive);
    if(voltage12v_receive[0]==1)
    {
      if(!i2c.read(adress12v,registreBusVoltage,1))
      {
        i2c.read(adress12v,voltage12v,2);
        putCharInArray(voltage12v_send,voltage12v,0.00125);
        RS485::write(PSU_ID,cmd_array[0],nb_byte_send,voltage12v_send);
      }
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
  char voltage12v[16]={0};

  while(true)
  {
    RS485::read(cmd_array,nb_command,voltage12v_receive);
    if(voltage12v_receive[0]==1)
    {
      if(!i2c.read(adress12v,registreCurrent,1))
      {
        i2c.read(adress12v,voltage12v,2);
        putCharInArray(voltage12v_send,voltage12v,0.001);
        RS485::write(PSU_ID,cmd_array[0],nb_byte_send,voltage12v_send);
      }
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
  char motor1[16]={0};

  while(true)
  {
    RS485::read(cmd_array,nb_command,motor1_receive);
    if(motor1_receive[0]==1)
    {
      if(!i2c.read(adress16V1,registreBusVoltage,1))
      {
        i2c.read(adress16V1,motor1,2);
        putCharInArray(motor1_send,motor1,0.00125);
        RS485::write(PSU_ID,cmd_array[0],nb_byte_send,motor1_send);
      }
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
  char motor1[16]={0};

  while(true)
  {
    RS485::read(cmd_array,nb_command,motor1_receive);
    if(motor1_receive[0]==1)
    {
      if(!i2c.read(adress16V1,registreCurrent,1))
      {
        i2c.read(adress16V1,motor1,2);
        putCharInArray(motor1_send,motor1,0.001);
        RS485::write(PSU_ID,cmd_array[0],nb_byte_send,motor1_send);
      }
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
  char motor2[16]={0};

  while(true)
  {
    RS485::read(cmd_array,nb_command,motor2_receive);
    if(motor2_receive[0]==1)
    {
      if(!i2c.read(adress16V2,registreBusVoltage,1))
      {
        i2c.read(adress16V2,motor2,2);
        putCharInArray(motor2_send,motor2,0.00125);
        RS485::write(PSU_ID,cmd_array[0],nb_byte_send,motor2_send);
      }
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
  char motor2[16]={0};

  while(true)
  {
    RS485::read(cmd_array,nb_command,motor2_receive);
    if(motor2_receive[0]==1)
    {
      if(!i2c.read(adress16V2,registreCurrent,1))
      {
        i2c.read(adress16V2,motor2,2);
        putCharInArray(motor2_send,motor2,0.001);
        RS485::write(PSU_ID,cmd_array[0],nb_byte_send,motor2_send);
      }
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

void Run12vToggle()
{
  uint8_t cmd_array[1]={CMD_PS_ACT_12V};
  uint8_t run12v_receive[255]={0};
  uint8_t run12v_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 1;

  while(true)
  {
    RS485::read(cmd_array,nb_command,run12v_receive);
    if(run12v_receive[0]==1)
    {
      Run12v = 1;
      run12v_send[0]=1;
    }
    else
    {
      Run12v = 0;
      run12v_send[0]=0;
    }
    RS485::write(PSU_ID,cmd_array[0],nb_byte_send,run12v_send);
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

void Run12vRead()
{
  uint8_t cmd_array[1]={CMD_PS_CHECK_12V};
  uint8_t run12v_receive[255]={0};
  uint8_t run12v_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 1;

  while(true)
  {
    RS485::read(cmd_array,nb_command,run12v_receive);
    if(run12v_receive[0]==1)
    {
      if(Run12v.read())
      {
        run12v_send[0] = 1;
      }
      else
      {
        run12v_send[0] = 0;
      }
      RS485::write(PSU_ID,cmd_array[0],nb_byte_send,run12v_send);
    }
  }
}

void i2c_initialize()
{
}*/

int main()
{
  char data[2];
  data[0] = 0x02;
  data[1] = 0x34;
  I2C i2c_bus(I2CSDA,I2CSCL);
  INA226 sensorm1(&i2c_bus, (char)adress16V1);
  //RS485::init(PSU_ID);

  //RunMotor1 = 0;
  LedBatt4 = 1;

  sensorm1.setConfig((unsigned short)0100010100100111);
  //sensorm1.setCalibration((unsigned short)10000);
  
  //RunMotor1 = 1;

  wait_us(500000);

  while(1){
    float_t voltage;
    LedBatt4 = 1;
    //voltage = sensorm1.getBusVolt();
    i2c_bus.write(adress16V1,data,2,true);
    wait_us(500000);
    if(voltage > 0)
    {
      LedBatt4 = 0;
    }
  }
  /*feedbackPSU.start(ledfeedbackFunction);
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

  thread12vtoggle.start(Run12vToggle);
  thread12vtoggle.set_priority(osPriorityHigh);

  threadmotor1read.start(Motor1Read);
  threadmotor1read.set_priority(osPriorityHigh);

  threadmotor2read.start(Motor2Read);
  threadmotor2read.set_priority(osPriorityHigh);

  thread12vread.start(Run12vRead);
  thread12vread.set_priority(osPriorityHigh);
  */
}