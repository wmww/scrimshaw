#pragma once

#include "gpio.h"

// allows for creating pins that are checked for uniqueness
namespace UniqueGpio
{
class Output : public Gpio::Output
{
protected:
	Output(int pin);
	virtual ~Output();
	optional<int> pin;
};

class Input : public Gpio::Input
{
protected:
	Input(int pin);
	virtual ~Input();
	optional<int> pin;
};

class Spi : public Gpio::Spi
{
protected:
	Spi(int channel);
	virtual ~Spi();
	optional<int> channel;
};
}; // namespace UniqueGpio
