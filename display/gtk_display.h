#pragma once

#include "display.h"

#include <gtk/gtk.h>

class GtkDisplay : public Display
{
public:
	GtkDisplay(Vec2i const& size);
	~GtkDisplay() = default;

	void update(Vec2i lower_left, Vec2i size, bool* data);

private:
	void on_window_draw();

	Vec2i const size;
	GtkWidget* window;
	GtkWidget* canvas;
};
