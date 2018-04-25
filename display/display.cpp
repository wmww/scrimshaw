#include "display.h"

#include "epdif_display.h"
#include "gtk_display.h"

EpdifDisplay::Pins const epdif_pins = {
	17, // RST
	25, // DC
	8, // CS
	24, // BUSY
};

Vec2i const epdif_size{296, 128}; // this is wrong

std::unique_ptr<Display> Display::get()
{
	// return std::make_unique<EpdifDisplay>(epdif_pins, epdif_size);
	return std::make_unique<GtkDisplay>(epdif_size);
}
