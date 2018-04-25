#include <unistd.h>
#include <iostream>
 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../backend/Backend.h"
#include "../wayland/WaylandServer.h"
#include "../scene/Scene.h"

// change to toggle debug statements on and off
#define debug debug_off

// select the backend, X11_EGL is recommended to run nested in an X environment
// const Backend::Type backendType = Backend::X11_EGL;
// const backendType = Backend::X11_GLX;
// const backendType = Backend::DRM;
const Backend::Type backendType = Backend::SCRIMSHAW;

int main (int argc, char ** argv)
{
	debug("setting up backend");
	//auto backend = Backend::makeGLX(V2i(800, 800));
	Backend::setup(backendType);
	ASSERT_FATAL(Backend::instance);
	
	debug("setting up wayland server");
	
	WaylandServer::setup();
	
	Scene scene;
	scene.setup();
	Backend::instance->setInputInterface(scene.getInputInterface());
	
	//double startTime = timeSinceStart();
	//int ticks = 0;
	
	debug("starting main loop");
	while (Backend::instance)
	{
		WaylandServer::iteration();
		scene.draw();
		Backend::instance->swapBuffer();
		sleepForSeconds(0.01667);
		Backend::instance->checkEvents();
		//ticks++;
		//double endTime = timeSinceStart();
		//debug("FPS: " + to_string(ticks / (endTime - startTime)));
	}
	
	debug("shutting down wayland server");
	WaylandServer::shutdown();
	
	std::cout << "exiting" << std::endl;
}
