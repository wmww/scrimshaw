#pragma once

#include "display/display.h"
#include "executor.h"

#include <gtk/gtk.h>

#include <thread>
#include <atomic>

class GtkDisplay : public Display
{
public:
	GtkDisplay(Vec2i const& size);
	~GtkDisplay();

	void draw(PixelBuffer buffer, Vec2i lower_left) override;
	void commit() override;
	Vec2i get_size() override { return size; }
	bool is_dead() override { return die; }

private:
	void draw_window();

	PixelBuffer::ColorRGB const on_color{12, 27, 15};
	PixelBuffer::ColorRGB const off_color{198, 243, 204};

	Vec2i const size;
	PixelBuffer::ColorRGB* const pixels;
	GtkWidget* window;
	GtkWidget* canvas;
	GdkPixbuf* const pixbuf;

	std::atomic<bool> die{false};
	std::thread gtk_thread;
	Executor executor;
};