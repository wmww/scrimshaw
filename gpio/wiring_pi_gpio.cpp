#include "gpio/gpio.h"
#include "gpio/unique_gpio.h"
#include "util/logger.h"

#ifdef EMULATED_WIRING_PI

#include <emulator/wiring_pi.h>

#else

#include <wiringPi.h>
#include <wiringPiSPI.h>

#endif

class WiringPiGpio : public Gpio
{
public:
	class Output : public UniqueGpio::Output
	{
	public:
		Output(int pin_) : UniqueGpio::Output(pin_)
		{
			assert_else(pin, return );
			pinMode(this->pin.value(), OUTPUT);
		}

		void set(bool state) override
		{
			if (state == cached_state)
				return;
			cached_state = state;
			assert_else(pin, return );
			digitalWrite(this->pin.value(), state ? HIGH : LOW);
		}

		bool cached_state{false};
	};

	class Input : public UniqueGpio::Input
	{
	public:
		Input(int pin_) : UniqueGpio::Input(pin_)
		{
			assert_else(pin, return );
			pinMode(pin.value(), INPUT);
		}

		bool get() override
		{
			assert_else(pin, return false);
			return digitalRead(pin.value()) == HIGH;
		}
	};

	class Spi : public UniqueGpio::Spi
	{
	public:
		Spi(int channel_) : UniqueGpio::Spi(channel_)
		{
			assert_else(channel, return );
			assert_nonfatal(wiringPiSPISetup(channel.value(), 2000000) > 0);
		}

		void send_byte(unsigned char data) override
		{
			assert_else(channel, return );
			wiringPiSPIDataRW(channel.value(), &data, 1);
		}
	};

	WiringPiGpio()
	{
		// usie Broadcom GPIO pin mapping
		assert_fatal(wiringPiSetupGpio() == 0);
	}

	unique_ptr<Gpio::Output> output(int pin) override { return make_unique<Output>(pin); }
	unique_ptr<Gpio::Input> input(int pin) override { return make_unique<Input>(pin); }
	unique_ptr<Gpio::Spi> spi(int channel) override { return make_unique<Spi>(channel); }
};

unique_ptr<Gpio> Gpio::instance_;

unique_ptr<Gpio> Gpio::make()
{
	return make_unique<WiringPiGpio>();
}
