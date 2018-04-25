#include "Backend.h"
#include "../../display/display.h"

// change to toggle debug statements on and off
#define debug debug_off

struct BackendScrimshaw: Backend
{
	BackendScrimshaw()
	{
		display = Display::get();
	}
	
	~BackendScrimshaw() = default;
	
	void swapBuffer()
	{
		warning("BackendScrimshaw::swapBuffer() not implemented");
	}
	
	void checkEvents()
	{
		warning("BackendScrimshaw::checkEvents() not implemented");
	}
	
	std::unique_ptr<Display> display;
};

unique_ptr<Backend> makeScrimshawBackend()
{
	return make_unique<BackendScrimshaw>();
}

