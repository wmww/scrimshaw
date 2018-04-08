#include "display.h"

#include <iostream>

int main()
{
	auto display = Display::get();
	std::cout << "got display" << std::endl;
	return 0;
}
