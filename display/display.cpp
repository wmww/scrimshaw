
#include "display.h"
#include "epdif/epdif_display.h"
#include "vec2.h"

const EpdifDisplay::Pins epdif_pins = {
	17, // RST
	25, // DC
	8, // CS
	24, // BUSY
};

const Vec2i epdif_size{200, 200}; // this is wrong

std::unique_ptr<Display> Display::get()
{
	return std::make_unique<EpdifDisplay>(epdif_pins, epdif_size);
}
