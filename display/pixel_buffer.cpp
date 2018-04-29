#include "pixel_buffer.h"
#include "logger.h"

#include "wl_shm_format.h"

#include <string.h>

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
	int a_over = a_clip_corner + a_clip_size - a_size;
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

#define for_range(a) for (output_point.a = output_clip_corner.a; output_point.a < output_max.a; output_point.a++,
input_point.a = ((output_point.a - output_clip_corner.a) * input_clip_size.a) / output_clip_size.a +
input_clip_corner.a)

	if (call_clamp(input, output, x) &&
		call_clamp(input, output, y) &&
		call_clamp(output, input, x) &&
		call_clamp(output, input, y))
	{
		Vec2i output_max = output_clip_corner + output_clip_size;
		Vec2i output_point;
		Vec2i input_point;

		for_range (y)
		{

		}
	}

#undef call_clamp
}
*/

void PixelBuffer::create_empty(Vec2i size_)
{
	data = std::unique_ptr<bool[]>(new bool[size_.size()]);
	size = size_;
	memset(data.get(), 0, sizeof(bool) * size.size());
}

void PixelBuffer::copy_from_wl_shm_data(void const* input_data, Vec2i input_size, uint32_t format,
										Vec2i input_clip_lower_left, Vec2i input_clip_size, Vec2i final_size)
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

bool PixelBuffer::copy_from_pixel_buffer(PixelBuffer* other, Vec2i lower_left)
{
	if (!has_data())
	{
		log_warning("empty buffer");
		return false;
	}
	bool damage = false;
	Vec2i point;
	for (point.y = 0; point.y < size.y; point.y++)
	{
		for (point.x = 0; point.x < size.x; point.x++)
		{
			Vec2i output_point = point + lower_left;
			if (output_point.x >= 0 && output_point.y >= 0 && output_point.x < other->get_size().x &&
				output_point.y < other->get_size().y)
			{
				bool* pixel = data.get() + point.x + size.x * point.y;
				bool const* other_pixel = other->get_data_ptr() + output_point.x + other->get_size().x * output_point.y;
				if (*other_pixel != *pixel)
				{
					damage = true;
					*pixel = *other_pixel;
				}
			}
		}
	}
	return damage;
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
					data[point.x + size.x * point.y] ? off_color : on_color;
			}
		}
	}
}

void PixelBuffer::send_packed_bits(void (*func)(unsigned char))
{
	if (!has_data())
	{
		log_warning("empty buffer");
		return;
	}
	if (size.x % 8 != 0)
	{
		log_warning("width " + std::to_string(size.x) + " is not divisible by 8, will attempt to clip");
	}

	for (int y = 0; y < size.y; y++)
	{
		bool* row_start = data.get() + y * size.x;
		for (int x = 0; x < size.x; x += 8)
		{
			unsigned char packed_data = (row_start[x + 0] * (0x80 >> 0)) | (row_start[x + 1] * (0x80 >> 1)) |
										(row_start[x + 2] * (0x80 >> 2)) | (row_start[x + 3] * (0x80 >> 3)) |
										(row_start[x + 4] * (0x80 >> 4)) | (row_start[x + 5] * (0x80 >> 5)) |
										(row_start[x + 6] * (0x80 >> 6)) | (row_start[x + 7] * (0x80 >> 7));
			func(packed_data);
		}
	}
}

void PixelBuffer::send_packed_bits_transformed(void (*func)(unsigned char), Vec2<bool> flip, bool swap_x_y)
{
	if (!has_data())
	{
		log_warning("empty buffer");
		return;
	}
	auto output_size = swap_if_needed(size, swap_x_y);
	if (output_size.x % 8 != 0)
	{
		log_warning("width " + std::to_string(size.x) + " is not divisible by 8, will attempt to clip");
	}
	Vec2i output_point;
	for (output_point.y = 0; output_point.y < output_size.y; output_point.y++)
	{
		for (int output_chunk_x = 0; output_chunk_x + 7 < output_size.x; output_chunk_x += 8)
		{
			unsigned char packed_data = 0;
			for (int i = 0; i < 8; i++)
			{
				output_point.x = output_chunk_x + i;
				Vec2i input_point = transform_to_display(output_point, output_size, flip, swap_x_y);
				// if (input_point.x >= 0 && input_point.y >= 0 && input_point.x < size.x && input_point.y < size.y)
				packed_data |= (0x80 >> i) * data.get()[input_point.x + size.x * input_point.y];
				// else
				//	log_warning("input point is " + input_point.to_string() + " (size is " + size.to_string() + ")");
			}
			func(packed_data);
		}
	}
}
