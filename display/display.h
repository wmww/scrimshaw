#pragma once

#include <memory>
#include "vec2.h"
#include "pixel_buffer.h"

Vec2i const epdif_size{128, 296};

class Display
{
public:
	static std::unique_ptr<Display> get();

	virtual ~Display() = default;

	virtual void draw(PixelBuffer buffer, Vec2i lower_left) = 0;
	virtual void commit() = 0;
	virtual Vec2i get_size() = 0;
	virtual bool is_dead() = 0;

private:
};
