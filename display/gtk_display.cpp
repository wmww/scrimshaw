#include "gtk_display.h"

#include "logger.h"

#include <unistd.h>

GtkDisplay::GtkDisplay(Vec2i const& request_size) : size{request_size}, pixels{new Color[request_size.size()]}
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
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);
	canvas = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(window), canvas);
	g_signal_connect(
		canvas,
		"draw",
		GCallback(+[](GtkWidget*, GdkEvent*, gpointer data) { static_cast<GtkDisplay*>(data)->draw_window(); }),
		this);
	gtk_widget_set_app_paintable(canvas, TRUE);
	gtk_widget_show_all(window);
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
}

void GtkDisplay::update(Vec2i lower_left, Vec2i update_size, bool* data)
{
	bool* data_copy = new bool[update_size.size()];
	executor.run([=]() { log_warning("not implemented"); });
}

void GtkDisplay::draw_window()
{
	GError* err = nullptr;

	// Create pixbuf
	GdkPixbuf* pix = gdk_pixbuf_new_from_data((const guchar*)pixels,
											  GDK_COLORSPACE_RGB,
											  false,
											  8,
											  size.x,
											  size.y,
											  size.x * 3,
											  +[](guchar* pixels, gpointer data) { delete[] pixels; },
											  nullptr);
	if (err)
	{
		log_error(std::string("GTK ERROR: ") + err->message);
		g_error_free(err);
		return;
	}
	cairo_rectangle_int_t cairo_rect = {0, 0, size.x, size.y};
	cairo_region_t* region = cairo_region_create_rectangle(&cairo_rect);
	GdkDrawingContext* drawing_context = gdk_window_begin_draw_frame(gtk_widget_get_window(canvas), region);
	cairo_region_destroy(region);
	cairo_t* cairo_context = gdk_drawing_context_get_cairo_context(drawing_context);
	gdk_cairo_set_source_pixbuf(cairo_context, pix, 0, 0);
	cairo_paint(cairo_context);
	gdk_window_end_draw_frame(gtk_widget_get_window(canvas), drawing_context);
}
