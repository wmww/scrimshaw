#include "unique_gpip.h"
#include "util/logger.h"

#include <set>

namespace
{
std::set<int> used_pins;
std::set<int> used_channels;

optional<int> register_pin(int pin)
{
	if (used_pins.find(pin) != used_pins.end())
	{
		log_warning("tried to use pin " + to_string(pin) + " multiple times");
		return nullopt;
	}
	else
	{
		used_pins.insert(pin);
		return pin;
	}
}

void unregister_pin(int pin)
{
	used_pins.erase(pin);
}

optional<int> register_spi_channel(int channel)
{
	if (used_channels.find(channel) != used_channels.end())
	{
		log_warning("tried to use SPI channel " + to_string(channel) + " multiple times");
		return nullopt;
	}
	else
	{
		used_channels.insert(channel);
		return channel;
	}
}

void unregister_spi_channel(int channel)
{
	used_channels.erase(channel);
}
} // namespace

UniqueGpio::Output::Output(int pin) : pin{register_pin(pin)}
{}

UniqueGpio::Output::~Output()
{
	if (pin)
		unregister_pin(pin.value());
}

UniqueGpio::Input::Input(int pin) : pin{register_pin(pin)}
{}

UniqueGpio::Input::~Input()
{
	if (pin)
		unregister_pin(pin.value());
}

UniqueGpio::Spi::Spi(int channel) : channel{register_spi_channel(channel)}
{}

UniqueGpio::Spi::~Spi()
{
	if (channel)
		unregister_spi_channel(channel.value());
}
