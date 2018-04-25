#pragma once

#include <memory>
#include "vec2.h"

Vec2i const epdif_size{296, 128};

class Display
{
public:
	static std::unique_ptr<Display> get();

	virtual ~Display() = default;

	virtual void draw(Vec2i lower_left, Vec2i size, bool* data) = 0;
	virtual void commit() = 0;
	virtual Vec2i get_size() = 0;
	virtual bool is_dead() = 0;

private:
};
