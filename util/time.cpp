#include "util/time.h"

#include <chrono>
#include <thread>

void sleep_for(double seconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds((int)(seconds * 1000)));
}

namespace
{
std::chrono::high_resolution_clock::time_point program_start_time = std::chrono::high_resolution_clock::now();
}

double get_time()
{
	return (std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() -
	program_start_time))
	.count();
}
