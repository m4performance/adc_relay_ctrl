#include "global.h"


#define HYSTERESIS_100_MV         100
#define HYSTERESIS_200_MV         200
#define HYSTERESIS_300_MV         300
#define HYSTERESIS_400_MV         400
#define HYSTERESIS_500_MV         500
#define HYSTERESIS_VALUE          HYSTERESIS_100_MV

#define MAX_VOLTAGE_PR_THRESHOLD  3300
#define MAX_VOLTAGE_PR_VALUE      2500


AnalogIn pressureThresholdIn(PA_6);
AnalogIn pressureIn(PA_7);
DigitalOut relay1Coil(PA_11);//RELAY_1_COIL_UC_3V3
RCCtrl rcctrl;

//#################### Serial debug ############################
static UnbufferedSerial serial_port(PC_10, PC_11, 921600);

FileHandle *mbed::mbed_override_console(int fd)
{
    return &serial_port;
}
// This allow using standard C i/o fuctions anywhere in a program
// Commands will be executed in declared serial_port 
// for example "printf("hello world\n");"
//##############################################################


uint16_t convertReadU16ToMv(uint16_t inVal)
{
  return ((inVal * MAX_VOLTAGE_PR_THRESHOLD) / UINT16_MAX);
}


uint16_t getEnableThresholdInMv()
{
  uint16_t thresholdMv = convertReadU16ToMv(pressureThresholdIn.read_u16());
  return ((thresholdMv * MAX_VOLTAGE_PR_VALUE) / MAX_VOLTAGE_PR_THRESHOLD);
}


uint16_t getDisableThresholdInMv(uint16_t pressureEnTh)
{
  uint16_t pressureDisTh = 0;
  if(pressureEnTh > HYSTERESIS_VALUE)
  {
    pressureDisTh = pressureEnTh - HYSTERESIS_VALUE;
  }
  return pressureDisTh;
}


int main()
{
  const char time[] = __TIME__;
  const char date[] = __DATE__;
  uint16_t pressureEnableThreshold = 0;
  uint16_t pressureDisableThreshold = 0;
  uint16_t pressureValue = 0;
  uint32_t relayEnabled = 0;

  printf("\r\n*****************************************************************\r\n");
  printf("ADC relay ctrl application v_%u.%u.%u \r\n", FIRMWARE_VERSION_MAJOR, FIRMWARE_VERSION_MINOR, FIRMWARE_VERSION_RELASE);
  printf("Build date: ");
  printf(date);
  printf(" ");
  printf(time);
  printf("\r\nHysteresis = %umV", HYSTERESIS_VALUE);
  printf("\r\n*****************************************************************\r\n");

  rcctrl.pllHseInit();

  while (true) 
  {
    pressureEnableThreshold = getEnableThresholdInMv();
    pressureDisableThreshold = getDisableThresholdInMv(pressureEnableThreshold);
    pressureValue = convertReadU16ToMv(pressureIn.read_u16());

    if(relayEnabled)
    {
      if(pressureValue <= pressureDisableThreshold)
      {
        relayEnabled = 0;
        relay1Coil.write(relayEnabled);
      }
    }
    else
    {
      if(pressureValue > pressureEnableThreshold)
      {
        relayEnabled = 1;
      }
      else
      {
        relayEnabled = 0;
      }
      relay1Coil.write(relayEnabled);
    }
    printf("Pressure Input: %umV,  ON: %umV, OFF: %umV\r\n", pressureValue, pressureEnableThreshold, pressureDisableThreshold);
    ThisThread::sleep_for(100ms);
  }
}