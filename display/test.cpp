#include "display.h"

#include "../util/logger.h"

int main()
{
	auto display = Display::get();
	log_message("got display");
	return 0;
}
