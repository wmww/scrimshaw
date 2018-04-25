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
	
    void draw(Texture texture, V2d pos)
    {
        display->draw(
            Vec2i(pos.x, pos.y),
            Vec2i(texture.get_dim().x, texture.get_dim().y),
            texture.get_data());
    }
    
	void swapBuffer()
	{
		display->commit();
	}
	
	void checkEvents()
	{
		debug("BackendScrimshaw::checkEvents() not implemented");
	}
	
	std::unique_ptr<Display> display;
};

unique_ptr<Backend> makeScrimshawBackend()
{
	return make_unique<BackendScrimshaw>();
}

