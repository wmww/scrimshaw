#include "Texture.h"
#include <wayland-server-protocol.h>
#include <wayland-server.h>

// change to toggle debug statements on and off
#define debug debug_off

struct Texture::Impl
{
	V2i dim;
    bool * data = nullptr;
	
	Impl() {}
	
	~Impl()
    {
        if (data)
            delete[] data;
    }
};

void Texture::setupEmpty()
{
	if (!impl)
		impl = make_shared<Impl>();
}

void Texture::loadFromBgrData(void * data, V2i dim)
{
	setupEmpty();
	impl->data = new bool[dim.x * dim.y];
	impl->dim = dim;
    for (int i = 0; i < dim.x * dim.y; i++)
    {
        struct ColorBGR
        {
            unsigned char b, g, r;
        };
        
        ColorBGR color = ((ColorBGR*)data)[i];
        
        impl->data[i] = (color.b + color.g + color.r) > 128 * 3;
    }
}

V2i Texture::get_dim()
{
    if (impl)
        return impl->dim;
    else
        return V2i();
}

bool * Texture::get_data()
{
    if (impl)
        return impl->data;
    else
        return nullptr;
}

