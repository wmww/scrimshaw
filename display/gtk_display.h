#pragma once

#include "display.h"
#include "executor.h"

#include <gtk/gtk.h>

#include <thread>
#include <mutex>
#include <functional>

class GtkDisplay : public Display
{
public:
	GtkDisplay(Vec2i const& size);
	~GtkDisplay();

	void draw(Vec2i lower_left, Vec2i size, bool* data) override;
    void commit() override;
	Vec2i get_size() override { return size; }

private:
	struct Color
	{
		unsigned char r, g, b;
	};

	void draw_window();
	void chuck_to_gtk(std::function<void()> action);

	Color const on_color{12, 27, 15};
	Color const off_color{198, 243, 204};

	Vec2i const size;
	Color* const pixels;
	GtkWidget* window;
	GtkWidget* canvas;
	GdkPixbuf* const pixbuf;

	bool die{false};
	std::thread gtk_thread;
	Executor executor;
};
