#pragma once

// This header provides the WiringPi functions for the purpose of emulation

// Handy defines

// wiringPi modes

#define WPI_MODE_PINS 0
#define WPI_MODE_GPIO 1
#define WPI_MODE_GPIO_SYS 2
#define WPI_MODE_PHYS 3
#define WPI_MODE_PIFACE 4
#define WPI_MODE_UNINITIALISED -1

// Pin modes

#define INPUT 0
#define OUTPUT 1
#define PWM_OUTPUT 2
#define GPIO_CLOCK 3
#define SOFT_PWM_OUTPUT 4
#define SOFT_TONE_OUTPUT 5
#define PWM_TONE_OUTPUT 6

#define LOW 0
#define HIGH 1

// Pull up/down/none

#define PUD_OFF 0
#define PUD_DOWN 1
#define PUD_UP 2

// PWM

#define PWM_MODE_MS 0
#define PWM_MODE_BAL 1

// Interrupt levels

#define INT_EDGE_SETUP 0
#define INT_EDGE_FALLING 1
#define INT_EDGE_RISING 2
#define INT_EDGE_BOTH 3

int wiringPiSetupGpio () ;
void pinMode(int pin, int mode);
void pullUpDnControl(int pin, int pud);
int digitalRead(int pin);
void digitalWrite(int pin, int value);
int wiringPiSPISetup (int channel, int speed) ;
int wiringPiSPIDataRW (int channel, unsigned char *data, int len) ;
