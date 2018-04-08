#pragma once

#include "../display.h"
#include "../vec2.h"

class EpdifDisplay: public Display
{
public:
	struct Pins
	{
		int rst, dc, cs, busy;
	};

	EpdifDisplay(const Pins& pins, const Vec2i& size);

private:
	const Pins pins;
	const Vec2i size;
};
