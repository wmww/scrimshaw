#pragma once

#include "display.h"

#include "vec2.h"

#include <gtk/gtk.h>

class GtkDisplay : public Display
{
public:
	GtkDisplay(Vec2i const& size);
	~GtkDisplay() = default;
};
