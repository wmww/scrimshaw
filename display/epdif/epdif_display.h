#pragma once

#include "display/display.h"
#include "executor.h"

#include <thread>
#include <atomic>

class Epd;

enum class DisplayMode { off, full_update, partial_update };

class EpdifDisplay : public Display
{
public:
	struct Pins
	{
		unsigned int reset, dc, cs, busy;
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
	std::unique_ptr<Epd> const epd;
	bool should_commit{false};
	PixelBuffer last_buffer;
	PixelBuffer pending_buffer;

	// thread safe
	Vec2i const size;
	std::atomic<bool> die{false};
	Executor executor;
};
