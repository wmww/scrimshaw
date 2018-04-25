#include "Texture.h"
#include <wayland-server-protocol.h>
#include <wayland-server.h>

// change to toggle debug statements on and off
#define debug debug_off

std::string nameOfShmFormat(wl_shm_format format)
{
    switch (format)
    {
    case WL_SHM_FORMAT_ARGB8888:
		return "WL_SHM_FORMAT_ARGB8888";
	case WL_SHM_FORMAT_XRGB8888:
		return "WL_SHM_FORMAT_XRGB8888";
	case WL_SHM_FORMAT_C8:
		return "WL_SHM_FORMAT_C8";
	case WL_SHM_FORMAT_RGB332:
		return "WL_SHM_FORMAT_RGB332";
	case WL_SHM_FORMAT_BGR233:
		return "WL_SHM_FORMAT_BGR233";
	case WL_SHM_FORMAT_XRGB4444:
		return "WL_SHM_FORMAT_XRGB4444";
	case WL_SHM_FORMAT_XBGR4444:
		return "WL_SHM_FORMAT_XBGR4444";
	case WL_SHM_FORMAT_RGBX4444:
		return "WL_SHM_FORMAT_RGBX4444";
	case WL_SHM_FORMAT_BGRX4444:
		return "WL_SHM_FORMAT_BGRX4444";
	case WL_SHM_FORMAT_ARGB4444:
		return "WL_SHM_FORMAT_ARGB4444";
	case WL_SHM_FORMAT_ABGR4444:
		return "WL_SHM_FORMAT_ABGR4444";
	case WL_SHM_FORMAT_RGBA4444:
		return "WL_SHM_FORMAT_RGBA4444";
	case WL_SHM_FORMAT_BGRA4444:
		return "WL_SHM_FORMAT_BGRA4444";
	case WL_SHM_FORMAT_XRGB1555:
		return "WL_SHM_FORMAT_XRGB1555";
	case WL_SHM_FORMAT_XBGR1555:
		return "WL_SHM_FORMAT_XBGR1555";
	case WL_SHM_FORMAT_RGBX5551:
		return "WL_SHM_FORMAT_RGBX5551";
	case WL_SHM_FORMAT_BGRX5551:
		return "WL_SHM_FORMAT_BGRX5551";
	case WL_SHM_FORMAT_ARGB1555:
		return "WL_SHM_FORMAT_ARGB1555";
	case WL_SHM_FORMAT_ABGR1555:
		return "WL_SHM_FORMAT_ABGR1555";
	case WL_SHM_FORMAT_RGBA5551:
		return "WL_SHM_FORMAT_RGBA5551";
	case WL_SHM_FORMAT_BGRA5551:
		return "WL_SHM_FORMAT_BGRA5551";
	case WL_SHM_FORMAT_RGB565:
		return "WL_SHM_FORMAT_RGB565";
	case WL_SHM_FORMAT_BGR565:
		return "WL_SHM_FORMAT_BGR565";
	case WL_SHM_FORMAT_RGB888:
		return "WL_SHM_FORMAT_RGB888";
	case WL_SHM_FORMAT_BGR888:
		return "WL_SHM_FORMAT_BGR888";
	case WL_SHM_FORMAT_XBGR8888:
		return "WL_SHM_FORMAT_XBGR8888";
	case WL_SHM_FORMAT_RGBX8888:
		return "WL_SHM_FORMAT_RGBX8888";
	case WL_SHM_FORMAT_BGRX8888:
		return "WL_SHM_FORMAT_BGRX8888";
	case WL_SHM_FORMAT_ABGR8888:
		return "WL_SHM_FORMAT_ABGR8888";
	case WL_SHM_FORMAT_RGBA8888:
		return "WL_SHM_FORMAT_RGBA8888";
	case WL_SHM_FORMAT_BGRA8888:
		return "WL_SHM_FORMAT_BGRA8888";
	case WL_SHM_FORMAT_XRGB2101010:
		return "WL_SHM_FORMAT_XRGB2101010";
	case WL_SHM_FORMAT_XBGR2101010:
		return "WL_SHM_FORMAT_XBGR2101010";
	case WL_SHM_FORMAT_RGBX1010102:
		return "WL_SHM_FORMAT_RGBX1010102";
	case WL_SHM_FORMAT_BGRX1010102:
		return "WL_SHM_FORMAT_BGRX1010102";
	case WL_SHM_FORMAT_ARGB2101010:
		return "WL_SHM_FORMAT_ARGB2101010";
	case WL_SHM_FORMAT_ABGR2101010:
		return "WL_SHM_FORMAT_ABGR2101010";
	case WL_SHM_FORMAT_RGBA1010102:
		return "WL_SHM_FORMAT_RGBA1010102";
	case WL_SHM_FORMAT_BGRA1010102:
		return "WL_SHM_FORMAT_BGRA1010102";
	case WL_SHM_FORMAT_YUYV:
		return "WL_SHM_FORMAT_YUYV";
	case WL_SHM_FORMAT_YVYU:
		return "WL_SHM_FORMAT_YVYU";
	case WL_SHM_FORMAT_UYVY:
		return "WL_SHM_FORMAT_UYVY";
	case WL_SHM_FORMAT_VYUY:
		return "WL_SHM_FORMAT_VYUY";
	case WL_SHM_FORMAT_AYUV:
		return "WL_SHM_FORMAT_AYUV";
	case WL_SHM_FORMAT_NV12:
		return "WL_SHM_FORMAT_NV12";
	case WL_SHM_FORMAT_NV21:
		return "WL_SHM_FORMAT_NV21";
	case WL_SHM_FORMAT_NV16:
		return "WL_SHM_FORMAT_NV16";
	case WL_SHM_FORMAT_NV61:
		return "WL_SHM_FORMAT_NV61";
	case WL_SHM_FORMAT_YUV410:
		return "WL_SHM_FORMAT_YUV410";
	case WL_SHM_FORMAT_YVU410:
		return "WL_SHM_FORMAT_YVU410";
	case WL_SHM_FORMAT_YUV411:
		return "WL_SHM_FORMAT_YUV411";
	case WL_SHM_FORMAT_YVU411:
		return "WL_SHM_FORMAT_YVU411";
	case WL_SHM_FORMAT_YUV420:
		return "WL_SHM_FORMAT_YUV420";
	case WL_SHM_FORMAT_YVU420:
		return "WL_SHM_FORMAT_YVU420";
	case WL_SHM_FORMAT_YUV422:
		return "WL_SHM_FORMAT_YUV422";
	case WL_SHM_FORMAT_YVU422:
		return "WL_SHM_FORMAT_YVU422";
	case WL_SHM_FORMAT_YUV444:
		return "WL_SHM_FORMAT_YUV444";
	case WL_SHM_FORMAT_YVU444:
		return "WL_SHM_FORMAT_YVU444";
    }
}

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

void Texture::loadFromData(void * data, V2i dim, uint32_t format)
{
	setupEmpty();
    ASSERT(format == WL_SHM_FORMAT_ARGB8888);
	impl->data = new bool[dim.x * dim.y];
	impl->dim = dim;
    for (int i = 0; i < dim.x * dim.y; i++)
    {
        struct Color
        {
            unsigned char a, r, g, b;
        };
        
        Color color = ((Color*)data)[i];
        
        impl->data[i] = (color.r + color.g + color.b) > 128 * 3;
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

