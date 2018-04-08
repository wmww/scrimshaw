#include "epdif_display.h"
#include "epd1in54.h"

EpdifDisplay::EpdifDisplay(Pins const& pins, Vec2i const& size)
	:
	pins{pins},
	size{size},
	epd{std::make_unique<Epd>(pins.rst, pins.dc, pins.cs, pins.busy, size.x, size.y)}
{}

EpdifDisplay::~EpdifDisplay() {}
