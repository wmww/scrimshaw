#pragma once

#include <memory>

class Display
{
public:
	static std::unique_ptr<Display> get();

private:
};
