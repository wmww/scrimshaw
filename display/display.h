#pragma once

#include <memory>

class Display
{
public:
	static std::unique_ptr<Display> get();

	virtual ~Display() = default;

private:
};
