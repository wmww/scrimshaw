#include <unistd.h>
#include <iostream>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "hedgehog/backend/Backend.h"
#include "hedgehog/wayland/WaylandServer.h"
#include "hedgehog/scene/Scene.h"

// change to toggle debug statements on and off
#define debug debug_off

int main(int argc, char** argv)
{
	debug("setting up backend");
	Backend::setup(Backend::SCRIMSHAW);
	ASSERT_FATAL(Backend::instance);

	debug("setting up wayland server");

	WaylandServer::setup();

	Scene scene;
	scene.setup();
	Backend::instance->setInputInterface(scene.getInputInterface());

	// double startTime = timeSinceStart();
	// int ticks = 0;

	debug("starting main loop");
	while (Backend::instance)
	{
		WaylandServer::iteration();
		// scene.draw();
		Backend::instance->swapBuffer();
		// sleepForSeconds(0.01667);
		sleepForSeconds(0.1);
		Backend::instance->checkEvents();
		// ticks++;
		// double endTime = timeSinceStart();
		// debug("FPS: " + to_string(ticks / (endTime - startTime)));

		// if (timeSinceStart() > 150)
		//	break;
	}

	debug("shutting down wayland server");
	WaylandServer::shutdown();

	debug("exiting");
}
