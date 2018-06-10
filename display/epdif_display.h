#pragma once

#include "display/display.h"
#include "util/executor.h"

#include <thread>
#include <atomic>

class EpdDriver;

enum class DisplayMode { off, full_update, partial_update };

// EPDIF stands for Electronic Paper Display InterFace. I know Electronic Paper Display InterFace Display is a bad name,
// but I just refer to all things to do with [this model](https://www.waveshare.com/wiki/2.9inch_e-Paper_Module) as
// EPDIF

class EpdifDisplay : public Display
{
public:
	struct Pins
	{
		int reset, dc, cs, busy, spi_channel;
	};

	EpdifDisplay(Pins const& pins, Vec2i size_, Vec2<bool> flip_, bool swap_x_y_);
	~EpdifDisplay();

	void draw(PixelBuffer buffer, Vec2i lower_left) override;
	void commit() override;
	Vec2i get_size() override { return size; }
	bool is_dead() override { return false; }

private:
	// default thread
	std::thread render_thread;
	std::vector<std::pair<PixelBuffer, Vec2i>> pending_buffers;

	// render thread
	std::unique_ptr<EpdDriver> const epd;
	bool should_commit{false};
	PixelBuffer last_buffer;
	PixelBuffer pending_buffer;

	// thread safe
	Vec2i const size;
	std::atomic<bool> die{false};
	Executor executor;
};
