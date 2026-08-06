#include "global.h"
#include "main.h"


#define HYSTERESIS_100_MV         100
#define HYSTERESIS_200_MV         200
#define HYSTERESIS_300_MV         300
#define HYSTERESIS_400_MV         400
#define HYSTERESIS_500_MV         500
#define HYSTERESIS_VALUE          HYSTERESIS_100_MV

#define ADC_SUPPLY_MV             3300

#define MAX_VOLTAGE_PR_THRESHOLD  3300
#define MAX_VOLTAGE_PR_VALUE      2000

#define OPA_COEFF_NUMERATOR       4
#define OPA_COEFF_DENOMINATOR     10


AnalogIn pressureThresholdIn(PA_6);
AnalogIn pressureIn(PA_7);
DigitalOut relay1Coil(PA_11);                           //RELAY_1_COIL_UC_3V3
RCCtrl rcctrl;
THRESHOLD threshold(&pressureThresholdIn, 0, 5000, HYSTERESIS_VALUE);


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
  return ((inVal * ADC_SUPPLY_MV) / UINT16_MAX);
}


int main()
{
  const char time[] = __TIME__;
  const char date[] = __DATE__;
  uint16_t pressureAdcVoltage = 0;
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
    pressureAdcVoltage = convertReadU16ToMv(pressureIn.read_u16());
    uint32_t pressureOpaInVoltage = (pressureAdcVoltage * OPA_COEFF_DENOMINATOR) / OPA_COEFF_NUMERATOR;
    
    threshold.cyclicHandling();

    if(relayEnabled)
    {
      if(pressureOpaInVoltage <= threshold.disable)
      {
        relayEnabled = 0;
        relay1Coil.write(relayEnabled);
      }
    }
    else
    {
      if(pressureOpaInVoltage > threshold.enable)
      {
        relayEnabled = 1;
      }
      else
      {
        relayEnabled = 0;
      }
      relay1Coil.write(relayEnabled);
    }
    
    printf("ADC_IN: %5umV, OPA_IN: %5lumV, ON: %5umV, OFF: %5umV\r\n", pressureAdcVoltage, pressureOpaInVoltage, threshold.enable, threshold.disable);
    ThisThread::sleep_for(100ms);
  }
}