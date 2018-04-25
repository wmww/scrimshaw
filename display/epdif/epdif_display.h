#pragma once

#include "display/display.h"

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

	void draw(Vec2i lower_left, Vec2i size, bool* data) override;
	void commit() override;
	Vec2i get_size() override { return size; }
	bool is_dead() override { return false; }

private:
	Pins const pins;
	Vec2i const size;
	std::unique_ptr<Epd> const epd;
};
