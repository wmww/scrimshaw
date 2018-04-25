#include "Backend.h"
#include "../../display/display.h"

// change to toggle debug statements on and off
#define debug debug_off

extern bool stop;
bool libinput_setup();
void libinput_destroy();
void libinput_check_events(InputInterface * interface);

struct BackendScrimshaw: Backend
{
	BackendScrimshaw()
	{
		display = Display::get();
        ASSERT(libinput_setup());
	}
	
	~BackendScrimshaw()
    {
        libinput_destroy();
    }
	
    Vec2i getDim()
    {
        return display->get_size();
    }
    
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
        if (display->is_dead())
        {
            Backend::instance = nullptr;
            return;
        }

		if (auto input = inputInterface.lock())
		{
			libinput_check_events(&*input);
			if (stop)
				Backend::instance = nullptr;
		}
	}
	
	std::unique_ptr<Display> display;
};

unique_ptr<Backend> makeScrimshawBackend()
{
	return make_unique<BackendScrimshaw>();
}

