#include "epdif_display.h"

#include "epd1in54.h"

#include "logger.h"

#include <cmath>

EpdifDisplay::Pins const epdif_pins = {
	17, // RST
	25, // DC
	8, // CS
	24, // BUSY
};

std::unique_ptr<Display> Display::get()
{
	return std::make_unique<EpdifDisplay>(epdif_pins, epdif_size);
}

EpdifDisplay::EpdifDisplay(Pins const& pins, Vec2i const& size)
	: pins{pins},
      size{size},
      epd{std::make_unique<Epd>(pins.rst, pins.dc, pins.cs, pins.busy, size.x, size.y)},
      data{std::unique_ptr<unsigned char[]>{new unsigned char[
          static_cast<int>(std::ceil(static_cast<double>(size.size()) / sizeof(char)))]}}
{}

EpdifDisplay::~EpdifDisplay()
{}

void EpdifDisplay::draw(Vec2i lower_left, Vec2i size, bool* data)
{

	log_warning("not implemented");
}

void EpdifDisplay::commit()
{
	log_warning("not implemented");
}
