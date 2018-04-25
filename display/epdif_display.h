#pragma once

#include "display.h"

class Epd;

class EpdifDisplay : public Display
{
public:
	struct Pins
	{
		unsigned int rst, dc, cs, busy;
	};

	EpdifDisplay(Pins const& pins, Vec2i const& size);
	~EpdifDisplay();

	void update(Vec2i lower_left, Vec2i size, bool* data);
	Vec2i get_size() { return size; }

private:
	Pins const pins;
	Vec2i const size;
	std::unique_ptr<Epd> const epd;
};
