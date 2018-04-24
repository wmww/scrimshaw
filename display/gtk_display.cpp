#include "gtk_display.h"

#include <iostream>

GtkDisplay::GtkDisplay(Vec2i const& request_size) : size{request_size}
{
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
		GCallback(+[](GtkWidget*, GdkEvent*, gpointer data) { static_cast<GtkDisplay*>(data)->on_window_draw(); }),
		this);
	gtk_widget_set_app_paintable(canvas, TRUE);
	gtk_widget_show_all(window);
	gtk_main();
}

void GtkDisplay::on_window_draw()
{
	GError* err = nullptr;

	guchar* pix_data = new guchar[size.area() * 3];

	Vec2i i;
	for (i.y = 0; i.y < size.y; i.y++)
	{
		for (i.x = 0; i.x < size.x; i.x++)
		{
			pix_data[(i.x + i.y * size.x) * 3 + 0] = 0;
			pix_data[(i.x + i.y * size.x) * 3 + 1] = 128;
			pix_data[(i.x + i.y * size.x) * 3 + 2] =
				std::min((i - (size / 2.0)).length() * 255 * 2 / size.length(), 255);
		}
	}

	// Create pixbuf
	GdkPixbuf* pix = gdk_pixbuf_new_from_data(pix_data,
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
		std::cerr << "GTK ERROR:" << err->message << std::endl;
		g_error_free(err);
	}
	if (canvas == nullptr)
	{
		std::cerr << "null cancas" << std::endl;
	}
	std::cerr << "window size: " << size.x << std::endl;
	cairo_t* cr = gdk_cairo_create(gtk_widget_get_window(canvas));
	//    cr = gdk_cairo_create (da->window);
	gdk_cairo_set_source_pixbuf(cr, pix, 0, 0);
	cairo_paint(cr);
	//    cairo_fill (cr);
	cairo_destroy(cr);
	//    return FALSE;
}
