/***
 * 
 * Author : Francis Alonzo
 * 
 * Firmware_PSU
 * 
 ***/

#include "main.h"

// Board Address (0 to 3)
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
Thread threadtemperature;

I2C i2c_bus(I2CSDA,I2CSCL);
INA226 sensorm1(&i2c_bus, adress16V1);
INA226 sensorm2(&i2c_bus, adress16V2);
INA226 sensor12v(&i2c_bus, adress12v);
TC74A5 temperature(&i2c_bus, adressTemp);

/**
 * @brief Logique des LEDs est inversée 0 pour allumer et 1 pour éteindre
 * 
 */
void ledfeedbackFunction()
{
  double_t value;
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
    // Valeur de la batterie donnée avec un test pratique (voir Excel)
    value = Battery_16V.read();

    // Full - 16,4V
    if(value > 0.462)                               
    {
      LedBatt1 = 0;
      LedBatt2 = 0;
      LedBatt3 = 0;
      LedBatt4 = 1;
    }
    // 16,4V - 15,8V 
    else if (value <= 0.462 && value > 0.445)
    {
      LedBatt1 = 1;
      LedBatt2 = 0;
      LedBatt3 = 0;
      LedBatt4 = 1;
    }
    // 15,8V - 15,4V
    else if (value <= 0.445 && value > 0.433)
    {
      LedBatt1 = 1;
      LedBatt2 = 1;
      LedBatt3 = 0;
      LedBatt4 = 1;
    }
    // 15,4V - 0V
    else                                           
    {
      LedBatt1 = 1;
      LedBatt2 = 1;
      LedBatt3 = 1;
      LedBatt4 = 0;
    }

    // Double inversion
    if(Killswitch == 0)                       
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

/**
 * @brief Fonction pour l'envoi du voltage de la batterie sur RS485
 * 
 */
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

/**
 * @brief Fonction pour l'envoi du voltage du channel 12V sur RS485
 * 
 */
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

/**
 * @brief Fonction pour l'envoi du courant du channel 12V sur RS485
 * 
 */
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

/**
 * @brief Fonction pour l'envoi du voltage du moteur 1 sur RS485
 * 
 */
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

/**
 * @brief Fonction pour l'envoi du courant du moteur 1 sur RS485
 * 
 */
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

/**
 * @brief Fonction pour l'envoi du voltage du moteur 2 sur RS485
 * 
 */
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

/**
 * @brief Fonction pour l'envoi du courant du moteur 2 sur RS485
 * 
 */
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

/**
 * @brief Fonction pour allumer le moteur 1 avec RS485
 * 
 */
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

/**
 * @brief Fonction pour allumer le moteur 2 avec RS485
 * 
 */
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

/**
 * @brief Fonction pour vérifier de l'état du moteur 1 avec RS485
 * 
 */
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

/**
 * @brief Fonction pour vérifier de l'état du moteur 2 avec RS485
 * 
 */
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

/**
 * @brief Fonction pour la température du power supply avec RS485
 * 
 */
void TemperatureRead()
{
  uint8_t cmd_array[1]={CMD_PS_temperature};
  uint8_t temp_receive[255]={0};
  uint8_t temp_send[255]={0};
  uint8_t nb_command = 1;
  uint8_t nb_byte_send = 1;
  float_t temp;

  while(true)
  {
    RS485::read(cmd_array,nb_command,temp_receive);
    if(temp_receive[0]==1)
    {
      temp = temperature.getTemp();
      putFloatInArray(temp_send,temp);
      RS485::write(PSU_ID,cmd_array[0],nb_byte_send,temp_send);
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

  //  On s'assure que les 2 moteurs sont éteints
  RunMotor1 = 0;            
  RunMotor2 = 0;

  RS485::init(PSU_ID);

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

  threadtemperature.start(TemperatureRead);
  threadtemperature.set_priority(osPriorityAboveNormal3);
}