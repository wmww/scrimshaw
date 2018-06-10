#pragma once

#include <memory>
#include "util/vec2.h"
#include "pixel_buffer.h"

Vec2i const epdif_logical_size{296, 128};
Vec2<bool> const epdif_flip{false, true};
bool const epdif_swap_x_y{true};

class Display
{
public:
	static std::unique_ptr<Display> get();

	Display() = default;
	~Display() = default;
	Display(Display const&) = delete;
	Display(Display&&) = default;
	Display& operator=(Display const&) = delete;

	virtual void draw(PixelBuffer buffer, Vec2i lower_left) = 0;
	virtual void commit() = 0;
	virtual Vec2i get_size() = 0;
	virtual bool is_dead() = 0;

private:
};
