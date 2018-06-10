#pragma once

#include "display/display.h"
#include "util/executor.h"

#include <gtk/gtk.h>

#include <thread>
#include <atomic>

class GtkDisplay : public Display
{
public:
	GtkDisplay(Vec2i size_, Vec2<bool> flip_, bool swap_x_y_);
	~GtkDisplay();

	void draw(PixelBuffer buffer, Vec2i lower_left) override;
	void commit() override;
	Vec2i get_size() override { return external_size; }
	bool is_dead() override { return die; }

private:
	void draw_window();
	void clear_window(PixelBuffer::ColorRGB color);

	PixelBuffer::ColorRGB const on_color{12, 27, 15};
	PixelBuffer::ColorRGB const off_color{198, 243, 204};

	Vec2i const external_size;
	Vec2i const internal_size;
	Vec2<bool> const flip;
	bool const swap_x_y;
	PixelBuffer::ColorRGB* const pixels;
	GtkWidget* window;
	GtkWidget* canvas;
	GdkPixbuf* const pixbuf;

	std::atomic<bool> die{false};
	bool should_commit{false};
	bool has_drawn{false};
	std::thread gtk_thread;
	Executor executor;
};
