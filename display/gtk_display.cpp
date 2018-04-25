#include "gtk_display.h"

#include "logger.h"

#include <unistd.h>

GtkDisplay::GtkDisplay(Vec2i const& request_size)
	: size{request_size},
	  pixels{new Color[request_size.size()]},
	  pixbuf{gdk_pixbuf_new_from_data((const guchar*)pixels, GDK_COLORSPACE_RGB, false, 8, size.x, size.y, size.x * 3,
									  +[](guchar* pixels, gpointer data) { delete[] pixels; }, nullptr)}
{
	for (int i = 0; i < request_size.size(); i++)
	{
		pixels[i] = off_color;
	}
	gtk_init(0, nullptr);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	// gtk_widget_set_size_request (window, size.x, size.y);
	gtk_window_set_default_size(GTK_WINDOW(window), size.x, size.y);
	// Windows named 'Hedgehog' are configured to be floating in my window manager, long story.
	gtk_window_set_title(GTK_WINDOW(window), "Hedgehog");
	// g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);
	canvas = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(window), canvas);
	g_signal_connect(
		canvas,
		"draw",
		GCallback(+[](GtkWidget*, GdkEvent*, gpointer data) { static_cast<GtkDisplay*>(data)->draw_window(); }),
		this);
	gtk_widget_set_app_paintable(canvas, TRUE);
	gtk_widget_show_all(window);
	// Create pixbuf
	gtk_thread = std::thread([this]() {
		while (!die)
		{
			while (gtk_events_pending())
			{
				gtk_main_iteration();
			}
			usleep(0.02 * 1000000);
			executor.iteration();
		}
	});
}

GtkDisplay::~GtkDisplay()
{
	die = true;
	gtk_thread.join();
	g_object_unref(pixbuf);
}

void GtkDisplay::update(Vec2i lower_left, Vec2i update_size, bool* data)
{
	// don't worry about the extra copy, the GTK backend is for testing and doesn't need to be optimized
	bool* data_copy = new bool[update_size.size()];
	memcpy(data_copy, data, sizeof(bool) * update_size.size());

	executor.run([lower_left, update_size, data_copy, this]() {
		Vec2i i;
		for (i.y = 0; i.y < update_size.y; i.y++)
		{
			for (i.x = 0; i.x < update_size.x; i.x++)
			{
				Vec2i location = i + lower_left;
				if (location.x >= 0 && location.y >= 0 && location.x < size.x && location.y < size.y)
				{
					pixels[location.x + size.x * location.y] =
						data_copy[i.x + update_size.x * i.y] ? on_color : off_color;
				}
			}
		}

		delete[] data_copy;

		draw_window();
	});
}

void GtkDisplay::draw_window()
{
	cairo_rectangle_int_t cairo_rect = {0, 0, size.x, size.y};
	cairo_region_t* region = cairo_region_create_rectangle(&cairo_rect);
	GdkDrawingContext* drawing_context = gdk_window_begin_draw_frame(gtk_widget_get_window(canvas), region);
	cairo_region_destroy(region);
	cairo_t* cairo_context = gdk_drawing_context_get_cairo_context(drawing_context);
	gdk_cairo_set_source_pixbuf(cairo_context, pixbuf, 0, 0);
	cairo_paint(cairo_context);
	gdk_window_end_draw_frame(gtk_widget_get_window(canvas), drawing_context);
}
