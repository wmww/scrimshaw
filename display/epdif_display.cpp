#include "epdif_display.h"

#include "epdif/epd1in54.h"

#include "logger.h"

EpdifDisplay::EpdifDisplay(Pins const& pins, Vec2i const& size)
	: pins{pins}, size{size}, epd{std::make_unique<Epd>(pins.rst, pins.dc, pins.cs, pins.busy, size.x, size.y)}
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

