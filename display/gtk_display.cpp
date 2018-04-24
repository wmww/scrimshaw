#include "gtk_display.h"

GtkDisplay::GtkDisplay(Vec2i const& size)
	: app{gtk_application_new("net.widap.scrimshaw_test", G_APPLICATION_FLAGS_NONE)}, size{size}
{
	g_signal_connect(
		app, "activate",
		G_CALLBACK(+[](GtkApplication* app, gpointer self) { static_cast<GtkDisplay*>(self)->setup_window(); }), this);
	int status = g_application_run(G_APPLICATION(app), 0, nullptr);
	g_object_unref(app);
}

void GtkDisplay::setup_window()
{
	GtkWidget* window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Window");
	gtk_window_set_default_size(GTK_WINDOW(window), size.x, size.y);
	gtk_widget_show_all(window);
}
