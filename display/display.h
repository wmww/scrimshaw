#pragma once

#include <memory>
#include "vec2.h"

class Display
{
public:
	static std::unique_ptr<Display> get();

	virtual ~Display() = default;

private:
};
