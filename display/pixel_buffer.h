#pragma once

#include "util/vec2.h"

#include <memory>

using std::move;

class PixelBuffer
{
public:
	static inline Vec2i swap_if_needed(Vec2i point, bool swap_x_y)
	{
		return swap_x_y ? Vec2i{point.y, point.x} : point;
	}

	static inline Vec2i transform_to_display(Vec2i point, Vec2i input_size, Vec2<bool> flip, bool flip_x_y)
	{
		if (flip.x)
			point.x = input_size.x - point.x - 1;

		if (flip.y)
			point.y = input_size.y - point.y - 1;

		return swap_if_needed(point, flip_x_y);
	}

	struct ColorRGB
	{
		unsigned char r, g, b;
	};

	PixelBuffer() = default;
	~PixelBuffer() = default;
	PixelBuffer(PixelBuffer const&) = delete;
	PixelBuffer(PixelBuffer&&) = default;
	PixelBuffer& operator=(PixelBuffer const&) = delete;

	static const char* wl_shm_format_get_name(uint32_t format);

	void create_empty(Vec2i size_);
	void copy_from_wl_shm_data(void const* input_data, Vec2i input_size, uint32_t format, Vec2i input_clip_lower_left,
							   Vec2i input_clip_size, Vec2i final_size);

	// returns true if there was damage
	bool copy_from_pixel_buffer(PixelBuffer*, Vec2i lower_left);
	void copy_into_rgb_buffer(ColorRGB* output_data, Vec2i output_size, Vec2i lower_left, ColorRGB on_color,
							  ColorRGB off_color);
	void send_packed_bits(void (*func)(unsigned char));
	void send_packed_bits_transformed(void (*func)(unsigned char), Vec2<bool> flip, bool swap_x_y);
	std::pair<std::unique_ptr<unsigned char[]>, size_t> pack_bits_transformed(Vec2<bool> flip, bool swap_x_y);

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
