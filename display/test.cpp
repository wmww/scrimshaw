#include "display.h"

#include "../util/logger.h"

#include <unistd.h>

int main()
{
	auto display = Display::get();

	log_message("got display");

	auto size = display->get_size();
	bool* const pixels = new bool[size.size()];
	Vec2i i;
	for (i.y = 0; i.y < size.y; i.y++)
	{
		for (i.x = 0; i.x < size.x; i.x++)
		{
			bool* pix = pixels + i.x + size.x * i.y;

			if ((i - (size / 2.0)).length() / size.length() < 0.3)
				*pix = true;
			else
				*pix = false;
		}
	}

	log_message("rendered circle");

	display->update({}, size, pixels);

	usleep(4.0 * 1000000);

	return 0;
}
