#include "gtk_display.h"

#include "util/logger.h"

#include <unistd.h>
#include <functional>

std::unique_ptr<Display> Display::get()
{
	return std::make_unique<GtkDisplay>(epdif_logical_size, epdif_flip, epdif_swap_x_y);
}

GtkDisplay::GtkDisplay(Vec2i size_, Vec2<bool> flip_, bool swap_x_y_)
	: external_size{size_},
	  internal_size{PixelBuffer::swap_if_needed(size_, swap_x_y_)},
	  flip{flip_},
	  swap_x_y{swap_x_y_},
	  pixels{new PixelBuffer::ColorRGB[size_.size()]},
	  pixbuf{gdk_pixbuf_new_from_data((const guchar*)pixels, GDK_COLORSPACE_RGB, false, 8, internal_size.x,
									  internal_size.y, internal_size.x * 3,
									  +[](guchar* pixels, gpointer data) { delete[] pixels; }, nullptr)}
{
	for (int i = 0; i < internal_size.size(); i++)
	{
		pixels[i] = off_color;
	}
	gtk_init(0, nullptr);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	// gtk_widget_set_size_request (window, size.x, size.y);
	gtk_window_set_default_size(GTK_WINDOW(window), internal_size.x, internal_size.y);
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
			if (should_commit)
			{
				redraw(on_color);
				while (gtk_events_pending())
				{
					gtk_main_iteration();
				}
				usleep(0.2 * 1000000);
				redraw(off_color);
				while (gtk_events_pending())
				{
					gtk_main_iteration();
				}
				usleep(0.2 * 1000000);
				redraw(std::experimental::nullopt);
				while (gtk_events_pending())
				{
					gtk_main_iteration();
				}
				usleep(0.2 * 1000000);
				should_commit = false;
			}
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

// I would refactor to not use globals normally, but this is just testing code
int _draw_start_x;
int _draw_end_x;
int _draw_end_y;
int _draw_x;
int _draw_y;
PixelBuffer::ColorRGB* _draw_pixels;
Vec2i _draw_pixels_size;
PixelBuffer::ColorRGB _draw_on_color;
PixelBuffer::ColorRGB _draw_off_color;

void draw_func(unsigned char data)
{
	for (int i = 7; i >= 0; i--)
	{
		PixelBuffer::ColorRGB color = ((data >> i) & 0x01) ? _draw_on_color : _draw_off_color;
		if (_draw_y < _draw_end_y)
			_draw_pixels[_draw_x + _draw_pixels_size.x * _draw_y] = color;
		else
			log_warning("y went too far");
		_draw_x++;
		if (_draw_x >= _draw_end_x)
		{
			_draw_x = _draw_start_x;
			_draw_y++;
		}
	}
}

void GtkDisplay::draw(PixelBuffer buffer, Vec2i external_lower_left)
{
	executor.run([external_lower_left, buffer = std::make_shared<PixelBuffer>(std::move(buffer)), this]() {
		Vec2i external_upper_right = external_lower_left + buffer->get_size();
		Vec2i external_lower_left_transformed =
			PixelBuffer::transform_to_display(external_lower_left, external_size, flip, swap_x_y);
		Vec2i external_upper_right_transformed =
			PixelBuffer::transform_to_display(external_upper_right - Vec2i{1, 1}, external_size, flip, swap_x_y);
		Vec2i internal_lower_left =
			Vec2i{std::min(external_lower_left_transformed.x, external_upper_right_transformed.x),
				  std::min(external_lower_left_transformed.y, external_upper_right_transformed.y)};
		Vec2i internal_upper_right =
			Vec2i{std::max(external_lower_left_transformed.x, external_upper_right_transformed.x),
				  std::max(external_lower_left_transformed.y, external_upper_right_transformed.y)} +
			Vec2i{1, 1};
		_draw_start_x = internal_lower_left.x;
		_draw_end_x = internal_upper_right.x;
		_draw_end_y = internal_upper_right.y;
		_draw_x = internal_lower_left.x;
		_draw_y = internal_lower_left.y;
		_draw_pixels = pixels;
		_draw_pixels_size = internal_size;
		_draw_on_color = on_color;
		_draw_off_color = off_color;
		buffer->send_packed_bits_transformed(
			+[](unsigned char data) {
				draw_func(data);
				/*for (int i = 7; i >= 0; i--)
				{
					PixelBuffer::ColorRGB color = ((data >> i) & 0x01) ? _draw_on_color : _draw_off_color;
					if (_draw_y < _draw_end_y)
						_draw_pixels[_draw_x + _draw_pixels_size.x * _draw_y] = color;
					else
						log_warning("y went too far");
					_draw_x++;
					if (_draw_x >= _draw_end_x)
					{
						_draw_x = _draw_start_x;
						_draw_y++;
					}
				}*/
			},
			flip,
			swap_x_y);
		// buffer->copy_into_rgb_buffer(pixels, size, lower_left, on_color, off_color);
		has_drawn = true;
	});
}

void GtkDisplay::commit()
{
	executor.run([this]() {
		if (has_drawn)
		{
			should_commit = true;
			has_drawn = false;
		}
	});
}

void GtkDisplay::draw_window()
{
	if (die)
		return;
	cairo_rectangle_int_t cairo_rect = {0, 0, internal_size.x, internal_size.y};
	cairo_region_t* region = cairo_region_create_rectangle(&cairo_rect);
	GdkWindow* window = gtk_widget_get_window(canvas);
	GdkDrawingContext* drawing_context = gdk_window_begin_draw_frame(window, region);
	cairo_region_destroy(region);
	cairo_t* cairo_context = gdk_drawing_context_get_cairo_context(drawing_context);
	if (clear_color)
	{
		auto color = clear_color.value();
		cairo_set_source_rgb(cairo_context, color.r / 255.0, color.g / 255.0, color.b / 255.0);
		cairo_rectangle(cairo_context, 0, 0, internal_size.x, internal_size.y);
		cairo_fill(cairo_context);
	}
	else
	{
		gdk_cairo_set_source_pixbuf(cairo_context, pixbuf, 0, 0);
		cairo_paint(cairo_context);
	}
	gdk_window_end_draw_frame(window, drawing_context);
}

void GtkDisplay::redraw(std::experimental::optional<PixelBuffer::ColorRGB> clear_color)
{
	if (die)
		return;
	this->clear_color = clear_color;
	cairo_rectangle_int_t cairo_rect = {0, 0, internal_size.x, internal_size.y};
	gdk_window_invalidate_rect(gtk_widget_get_window(window), &cairo_rect, FALSE);
}
