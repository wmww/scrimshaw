#pragma once

#include "vec2.h"

#include <memory>

using std::move;

class PixelBuffer
{
public:
	struct ColorRGB
	{
		unsigned char r, g, b;
	};

	PixelBuffer() = default;
	~PixelBuffer() = default;

	PixelBuffer(PixelBuffer const&) = delete;
	PixelBuffer(PixelBuffer&&) = default;

	static const char* wl_shm_format_get_name(uint32_t format);

	void create_empty(Vec2i size_);
	void copy_from_wl_shm_data(void const* input_data, Vec2i input_size, uint32_t format, Vec2i input_clip_lower_left,
							   Vec2i input_clip_size, Vec2i final_size);

	void copy_into_rgb_buffer(ColorRGB* output_data, Vec2i output_size, Vec2i lower_left, ColorRGB on_color,
							  ColorRGB off_color);
	void send_packed_bits(void (*func)(unsigned char));
	void send_packed_bits_transformed(void (*func)(unsigned char), bool flip_x, bool flip_y, bool swap_x_y);

	void clear()
	{
		data.reset();
		size = {};
	}

	inline bool has_data() const { return data != nullptr; }
	inline bool* get_data_ptr() const { return data.get(); }
	inline Vec2i get_size() const { return size; }

private:
	std::unique_ptr<bool[]> data;
	Vec2i size;
};
