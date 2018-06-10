#include "wiring_pi.h"
#include <util/logger.h>

int wiringPiSetupGpio ()
{
	return 0;
}

void pinMode(int pin, int mode)
{

}

void pullUpDnControl(int pin, int pud)
{

}

int digitalRead(int pin)
{
	return LOW;
}

void digitalWrite(int pin, int value)
{

}

int wiringPiSPISetup (int channel, int speed)
{
	return -1;
}

int wiringPiSPIDataRW (int channel, unsigned char *data, int len)
{

}
