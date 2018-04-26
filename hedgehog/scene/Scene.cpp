#include "Scene.h"
#include "../backend/Backend.h"

#include <memory>

// change to toggle debug statements on and off
#define debug debug_off

Scene Scene::instance;

struct Scene::Impl: InputInterface
{
	struct Window
	{
		weak_ptr<WindowInterface> interface;
		V2d pos = V2d();
		V2d size = V2d(0.5, 0.5);
	};
	
	vector<Window> windows;
	V2d lastMousePos;
	
	Window getActiveWindow()
	{
		// TODO: OPTIMIZATION: do this well
		Window active;
		for (auto i: windows)
		{
			if (!i.interface.expired())
				active = i;
		}
		return active;
	}
	
	void pointerMotion(V2d newPos)
	{
		Window window = getActiveWindow();
		if (auto interface = window.interface.lock())
		{
			auto input = interface->getInputInterface().lock();
			ASSERT_ELSE(input, return);
			lastMousePos = newPos;
			V2d transformed = V2d((newPos.x - window.pos.x) / window.size.x, (newPos.y - window.pos.y) / window.size.y);
			input->pointerMotion(transformed);
		}
	}
	
	void pointerLeave()
	{
		Window window = getActiveWindow();
		if (auto interface = window.interface.lock())
		{
			auto input = interface->getInputInterface().lock();
			ASSERT_ELSE(input, return);
			input->pointerLeave();
		}
	}
	
	void pointerClick(uint button, bool down)
	{
		Window window = getActiveWindow();
		if (auto interface = window.interface.lock())
		{
			auto input = interface->getInputInterface().lock();
			ASSERT_ELSE(input, return);
			input->pointerClick(button, down);
		}
	}
	
	void keyPress(uint key, bool down)
	{
		Window window = getActiveWindow();
		if (auto interface = window.interface.lock())
		{
			auto input = interface->getInputInterface().lock();
			ASSERT_ELSE(input, return);
			input->keyPress(key, down);
		}
	}
};

void Scene::setup()
{
	impl = make_shared<Impl>();
	assert(instance.impl == nullptr);
	instance.impl = impl;
}

void Scene::addWindow(weak_ptr<WindowInterface> window)
{
	debug("adding window to scene");
	ASSERT_ELSE(impl, return);
	Impl::Window data;
	data.interface = window;
	data.pos = V2d(impl->windows.size() % 2 ? 0 : 0.5, (impl->windows.size() / 2) % 2 ? 0 : 0.5);
	impl->windows.push_back(data);
}

weak_ptr<InputInterface> Scene::getInputInterface()
{
	ASSERT_ELSE(impl, return weak_ptr<InputInterface>());
	return impl;
}

void Scene::draw()
{
	ASSERT_ELSE(impl, return);
    
    warning("Scene::draw() does nothing");
    
    /*
	for (auto window: impl->windows)
	{
		auto interface = window.interface.lock();
		if (interface && interface->texture.isValid())
		{
			impl->draw(interface->texture, window.pos);
		}
	}*/
	
	/*
	if (impl->cursorTexture.isValid())
	{
		impl->renderer.draw(impl->cursorTexture, impl->lastMousePos, V2d(0.2, 0.2));
	}
	*/
}
