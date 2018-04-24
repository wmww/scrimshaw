#pragma once

#include <memory>
#include "vec2.h"

class Display
{
public:
	static std::unique_ptr<Display> get();

	virtual ~Display() = default;

	virtual void update(Vec2i lower_left, Vec2i size, bool* data) = 0;

private:
};
