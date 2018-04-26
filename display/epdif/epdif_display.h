#pragma once

#include "display/display.h"
#include "executor.h"

#include <thread>
#include <atomic>

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

	void draw(PixelBuffer buffer, Vec2i lower_left) override;
	void commit() override;
	Vec2i get_size() override { return size; }
	bool is_dead() override { return false; }

private:
	Pins const pins;
	Vec2i const size;
	std::unique_ptr<Epd> const epd;
    std::unique_ptr<unsigned char[]> const data;

    std::atomic<bool> die{false};
	std::thread gtk_thread;
	Executor executor;
};
