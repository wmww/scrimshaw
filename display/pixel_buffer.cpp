#include "pixel_buffer.h"
#include "logger.h"

#include <wayland-server-protocol.h>

const char* PixelBuffer::wl_shm_format_get_name(uint32_t format)
{
	switch (format)
	{
	case WL_SHM_FORMAT_ARGB8888: return "WL_SHM_FORMAT_ARGB8888";
	case WL_SHM_FORMAT_XRGB8888: return "WL_SHM_FORMAT_XRGB8888";
	case WL_SHM_FORMAT_C8: return "WL_SHM_FORMAT_C8";
	case WL_SHM_FORMAT_RGB332: return "WL_SHM_FORMAT_RGB332";
	case WL_SHM_FORMAT_BGR233: return "WL_SHM_FORMAT_BGR233";
	case WL_SHM_FORMAT_XRGB4444: return "WL_SHM_FORMAT_XRGB4444";
	case WL_SHM_FORMAT_XBGR4444: return "WL_SHM_FORMAT_XBGR4444";
	case WL_SHM_FORMAT_RGBX4444: return "WL_SHM_FORMAT_RGBX4444";
	case WL_SHM_FORMAT_BGRX4444: return "WL_SHM_FORMAT_BGRX4444";
	case WL_SHM_FORMAT_ARGB4444: return "WL_SHM_FORMAT_ARGB4444";
	case WL_SHM_FORMAT_ABGR4444: return "WL_SHM_FORMAT_ABGR4444";
	case WL_SHM_FORMAT_RGBA4444: return "WL_SHM_FORMAT_RGBA4444";
	case WL_SHM_FORMAT_BGRA4444: return "WL_SHM_FORMAT_BGRA4444";
	case WL_SHM_FORMAT_XRGB1555: return "WL_SHM_FORMAT_XRGB1555";
	case WL_SHM_FORMAT_XBGR1555: return "WL_SHM_FORMAT_XBGR1555";
	case WL_SHM_FORMAT_RGBX5551: return "WL_SHM_FORMAT_RGBX5551";
	case WL_SHM_FORMAT_BGRX5551: return "WL_SHM_FORMAT_BGRX5551";
	case WL_SHM_FORMAT_ARGB1555: return "WL_SHM_FORMAT_ARGB1555";
	case WL_SHM_FORMAT_ABGR1555: return "WL_SHM_FORMAT_ABGR1555";
	case WL_SHM_FORMAT_RGBA5551: return "WL_SHM_FORMAT_RGBA5551";
	case WL_SHM_FORMAT_BGRA5551: return "WL_SHM_FORMAT_BGRA5551";
	case WL_SHM_FORMAT_RGB565: return "WL_SHM_FORMAT_RGB565";
	case WL_SHM_FORMAT_BGR565: return "WL_SHM_FORMAT_BGR565";
	case WL_SHM_FORMAT_RGB888: return "WL_SHM_FORMAT_RGB888";
	case WL_SHM_FORMAT_BGR888: return "WL_SHM_FORMAT_BGR888";
	case WL_SHM_FORMAT_XBGR8888: return "WL_SHM_FORMAT_XBGR8888";
	case WL_SHM_FORMAT_RGBX8888: return "WL_SHM_FORMAT_RGBX8888";
	case WL_SHM_FORMAT_BGRX8888: return "WL_SHM_FORMAT_BGRX8888";
	case WL_SHM_FORMAT_ABGR8888: return "WL_SHM_FORMAT_ABGR8888";
	case WL_SHM_FORMAT_RGBA8888: return "WL_SHM_FORMAT_RGBA8888";
	case WL_SHM_FORMAT_BGRA8888: return "WL_SHM_FORMAT_BGRA8888";
	case WL_SHM_FORMAT_XRGB2101010: return "WL_SHM_FORMAT_XRGB2101010";
	case WL_SHM_FORMAT_XBGR2101010: return "WL_SHM_FORMAT_XBGR2101010";
	case WL_SHM_FORMAT_RGBX1010102: return "WL_SHM_FORMAT_RGBX1010102";
	case WL_SHM_FORMAT_BGRX1010102: return "WL_SHM_FORMAT_BGRX1010102";
	case WL_SHM_FORMAT_ARGB2101010: return "WL_SHM_FORMAT_ARGB2101010";
	case WL_SHM_FORMAT_ABGR2101010: return "WL_SHM_FORMAT_ABGR2101010";
	case WL_SHM_FORMAT_RGBA1010102: return "WL_SHM_FORMAT_RGBA1010102";
	case WL_SHM_FORMAT_BGRA1010102: return "WL_SHM_FORMAT_BGRA1010102";
	case WL_SHM_FORMAT_YUYV: return "WL_SHM_FORMAT_YUYV";
	case WL_SHM_FORMAT_YVYU: return "WL_SHM_FORMAT_YVYU";
	case WL_SHM_FORMAT_UYVY: return "WL_SHM_FORMAT_UYVY";
	case WL_SHM_FORMAT_VYUY: return "WL_SHM_FORMAT_VYUY";
	case WL_SHM_FORMAT_AYUV: return "WL_SHM_FORMAT_AYUV";
	case WL_SHM_FORMAT_NV12: return "WL_SHM_FORMAT_NV12";
	case WL_SHM_FORMAT_NV21: return "WL_SHM_FORMAT_NV21";
	case WL_SHM_FORMAT_NV16: return "WL_SHM_FORMAT_NV16";
	case WL_SHM_FORMAT_NV61: return "WL_SHM_FORMAT_NV61";
	case WL_SHM_FORMAT_YUV410: return "WL_SHM_FORMAT_YUV410";
	case WL_SHM_FORMAT_YVU410: return "WL_SHM_FORMAT_YVU410";
	case WL_SHM_FORMAT_YUV411: return "WL_SHM_FORMAT_YUV411";
	case WL_SHM_FORMAT_YVU411: return "WL_SHM_FORMAT_YVU411";
	case WL_SHM_FORMAT_YUV420: return "WL_SHM_FORMAT_YUV420";
	case WL_SHM_FORMAT_YVU420: return "WL_SHM_FORMAT_YVU420";
	case WL_SHM_FORMAT_YUV422: return "WL_SHM_FORMAT_YUV422";
	case WL_SHM_FORMAT_YVU422: return "WL_SHM_FORMAT_YVU422";
	case WL_SHM_FORMAT_YUV444: return "WL_SHM_FORMAT_YUV444";
	case WL_SHM_FORMAT_YVU444: return "WL_SHM_FORMAT_YVU444";
	default: return "WL_SHM_FORMAT_UNKNOWN";
	}
}

/*
void clamp_range(int& a_size, int& a_clip_corner, int& a_clip_size, int& b_size, int& b_clip_corner, int& b_clip_size)
{
	if (a_clip_corner < 0)
	{
		int b_offset = ((- a_clip_corner) * b_clip_size) / a_clip_size;
		b_clip_corner += b_offset;
		b_clip_size -= b_offset;
		a_clip_size += a_clip_corner;
		a_clip_corner = 0;
	}
	int a_over = a_clip_corner + a_clip_size - a_size
	if (a_over > 0)
	{
		b_clip_size -= (a_over * b_clip_size) / a_clip_size;
		a_clip_size = a_size - a_clip_corner;
	}
	return a_clip_size > 0 && b_clip_size > 0;
}

void copy_buffer(void const* input_data, Vec2i input_size, Vec2i input_clip_corner, Vec2i input_clip_size,
                 void* output_data, Vec2i output_size, Vec2i output_clip_corner, Vec2i output_clip_size)
{
#define call_clamp(a, b, c) \
	clamp_range(a_size.c, a_clip_corner.c, a_clip_size.c, b_size.c b_clip_corner.c, b_clip_size.a)
	
#define for_range(a) for (output_point.a = output_clip_corner.a; output_point.a < output_max.a; output_point.a++, input_point.a = ((output_point.a - output_clip_corner.a) * input_clip_size.a) / output_clip_size.a + input_clip_corner.a)
	
	if (call_clamp(input, output, x) &&
	    call_clamp(input, output, y) &&
		call_clamp(output, input, x) &&
	    call_clamp(output, input, y))
	{
		Vec2i output_max = output_clip_corner + output_clip_size;
		Vec2i output_point;
		Vec2i input_point;
		
		for (y)
		{
			
		}
	}
	
#undef call_clamp
}
*/

void PixelBuffer::copy_from_wl_shm_data(void const* input_data, Vec2i input_size, uint32_t format,
											   Vec2d input_clip_lower_left, Vec2d input_clip_size, Vec2i final_size)
{
	if (format != WL_SHM_FORMAT_ARGB8888)
	{
		log_message(std::string() + "unsupported SHM format " + wl_shm_format_get_name(format));
		return;
	}

	struct Color
	{
		unsigned char a, r, g, b;
	};

	data = std::unique_ptr<bool[]>(new bool[final_size.size()]);
	size = final_size;

	Vec2i point;
	for (point.y = 0; point.y < size.y; point.y++)
	{
		for (point.x = 0; point.x < size.x; point.x++)
		{
			Vec2i input_point = (point * input_clip_size) / size + input_clip_lower_left;
			if (input_point.x >= 0 && input_point.y >= 0 && input_point.x < input_size.x &&
				input_point.y < input_size.y)
			{
				Color color = static_cast<Color const*>(input_data)[input_point.x + input_size.x * input_point.y];
				data[point.x + size.x * point.y] = (color.r + color.g + color.b) > 128 * 3;
			}
		}
	}
}

void PixelBuffer::copy_into_rgb_buffer(ColorRGB* output_data, Vec2i output_size, Vec2i lower_left, ColorRGB on_color,
									   ColorRGB off_color)
{
	if (!has_data())
	{
		log_warning("empty buffer");
		return;
	}
	Vec2i point;
	for (point.y = 0; point.y < size.y; point.y++)
	{
		for (point.x = 0; point.x < size.x; point.x++)
		{
			Vec2i output_point = point + lower_left;
			if (output_point.x >= 0 && output_point.y >= 0 && output_point.x < output_size.x &&
				output_point.y < output_size.y)
			{
				output_data[output_point.x + output_size.x * output_point.y] =
					data[point.x + size.x * point.y] ? on_color : off_color;
			}
		}
	}
}

void PixelBuffer::copy_into_packed_bit_buffer(void* outout_data, Vec2i output_size, Vec2i lower_left)
{
	if (!has_data())
	{
		log_warning("empty buffer");
		return;
	}
	log_warning("not implemented");
}
