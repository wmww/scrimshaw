#include "epdif_display.h"
#include "epd1in54.h"

EpdifDisplay::EpdifDisplay(const Pins& pins, const Vec2i& size)
	:
	pins{pins},
	size{size}
{}
