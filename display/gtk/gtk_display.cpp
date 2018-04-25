#include "gtk_display.h"

#include "logger.h"

#include <unistd.h>
#include <functional>

std::unique_ptr<Display> Display::get()
{
	return std::make_unique<GtkDisplay>(epdif_size);
}

GtkDisplay::GtkDisplay(Vec2i const& request_size)
	: size{request_size},
	  pixels{new PixelBuffer::ColorRGB[request_size.size()]},
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
	gtk_window_set_resizable(GTK_WINDOW(window), false);
	// Windows named 'Hedgehog' are configured to be floating in my window manager, long story.
	gtk_window_set_title(GTK_WINDOW(window), "Hedgehog");
	g_signal_connect(window,
					 "delete-event",
					 G_CALLBACK(+[](GtkWidget* widget, GdkEvent* event, GtkDisplay* self) { self->die = true; }),
					 this);
	canvas = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(window), canvas);
	g_signal_connect(
		canvas, "draw", GCallback(+[](GtkWidget*, GdkEvent*, GtkDisplay* self) { self->draw_window(); }), this);
	gtk_widget_set_app_paintable(canvas, TRUE);
	gtk_widget_show_all(window);
	// Create pixbuf
	gtk_thread = std::thread([this]() {
		while (!die)
		{
			executor.iteration();
			while (gtk_events_pending())
			{
				gtk_main_iteration();
			}
			usleep(0.02 * 1000000);
		}
		gtk_widget_hide(window);
		while (gtk_events_pending())
		{
			gtk_main_iteration();
		}
	});
}

GtkDisplay::~GtkDisplay()
{
	die = true;
	gtk_thread.join();
	g_object_unref(pixbuf);
}

void GtkDisplay::draw(PixelBuffer buffer, Vec2i lower_left)
{
	executor.run([lower_left, buffer = std::make_shared<PixelBuffer>(std::move(buffer)), this]() {
		buffer->copy_into_rgb_buffer(pixels, size, lower_left, on_color, off_color);
	});
}

void GtkDisplay::commit()
{
	executor.run([this]() { draw_window(); });
}

void GtkDisplay::draw_window()
{
	if (die)
		return;
	cairo_rectangle_int_t cairo_rect = {0, 0, size.x, size.y};
	cairo_region_t* region = cairo_region_create_rectangle(&cairo_rect);
	GdkDrawingContext* drawing_context = gdk_window_begin_draw_frame(gtk_widget_get_window(canvas), region);
	cairo_region_destroy(region);
	cairo_t* cairo_context = gdk_drawing_context_get_cairo_context(drawing_context);
	gdk_cairo_set_source_pixbuf(cairo_context, pixbuf, 0, 0);
	cairo_paint(cairo_context);
	gdk_window_end_draw_frame(gtk_widget_get_window(canvas), drawing_context);
}
