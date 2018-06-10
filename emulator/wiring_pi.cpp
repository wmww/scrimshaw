#include "wiring_pi.h"
#include <util/logger.h>

#define update log_message_off

string pin_mode_to_string(int mode)
{
	switch (mode)
	{
		case INPUT: return "INPUT"; break;
		case OUTPUT: return "OUTPUT"; break;
		case PWM_OUTPUT: return "PWM_OUTPUT"; break;
		case GPIO_CLOCK: return "GPIO_CLOCK"; break;
		case SOFT_PWM_OUTPUT: return "SOFT_PWM_OUTPUT"; break;
		case SOFT_TONE_OUTPUT: return "SOFT_TONE_OUTPUT"; break;
		case PWM_TONE_OUTPUT: return "PWM_TONE_OUTPUT"; break;
		default: return "UNKNOWN LOG LEVEL";
	}
}

int wiringPiSetupGpio ()
{
	log_message("using emulated GPIO");
	return 0;
}

void pinMode(int pin, int mode)
{
	log_message("pin " + to_string(pin) + " mode set to " + pin_mode_to_string(mode));
}

void pullUpDnControl(int pin, int pud)
{
	switch (pud)
	{
		case PUD_UP:
			log_message("pin " + to_string(pin) + " set to pull up");
			break;
		case PUD_DOWN:
			log_message("pin " + to_string(pin) + " set to pull down");
			break;
		case PUD_OFF:
			log_message("pin " + to_string(pin) + " pull disabled");
			break;
		default:
			log_message("pin " + to_string(pin) + " unknown pud " + to_string(pud));
			break;
	}
}

int digitalRead(int pin)
{
	bool state = false;
	update("pin " + to_string(pin) + " read returning " + (state ? "high" : "low"));
	return state ? HIGH : LOW;
}

void digitalWrite(int pin, int value)
{
	update("pin " + to_string(pin) + " set " + (value ? "high" : "low"));
}

int wiringPiSPISetup(int channel, int speed)
{
	update("emulated WiringPi always returns an invalid FD");
	return -1;
}

int wiringPiSPIDataRW(int channel, unsigned char *data, int len)
{
	update("transfering " + to_string(len) + " bytes of data on SPI channel " + to_string(channel));
	// What does this return? Who knows!
	// https://github.com/WiringPi/WiringPi/blob/5066e09747a2688879557b13a946d236684a38f5/wiringPi/wiringPiSPI.c#L90
	return 0;
}
