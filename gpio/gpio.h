#pragma once

#include <util/std.h>

class Gpio
{
public:
	Gpio() = default;
	Gpio(Gpio&&) = default;
	virtual ~Gpio() = default;

	class Output
	{
	public:
		virtual void write(bool state) = 0;
		virtual ~Output() = default;
	};

	class Input
	{
	public:
		virtual bool read() = 0;
		virtual ~Input() = default;
	};

	class Spi
	{
	public:
		virtual void send_byte(unsigned char data) = 0;
		virtual ~Spi() = default;
	};

	static Gpio* instance()
	{
		if (!instance_)
			instance_ = make();
		return instance_.get();
	}

	virtual unique_ptr<Output> output(int pin) = 0;
	virtual unique_ptr<Input> input(int pin) = 0;
	virtual unique_ptr<Spi> spi(int channel) = 0;

private:
	Gpio(Gpio const&) = delete;
	Gpio& operator=(Gpio const&) = delete;

	// defined differently in wiringpi, or emulation file
	static unique_ptr<Gpio> make();

	static unique_ptr<Gpio> instance_;
};
