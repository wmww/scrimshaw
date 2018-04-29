#pragma once

#include "hedgehog/util.h"
#include "hedgehog/scene/InputInterface.h"

#include "display/pixel_buffer.h"

class Backend
{
public:
	virtual Vec2i getDim() = 0;
	virtual void draw(PixelBuffer buffer, Vec2d position) = 0;
	virtual void swapBuffer() = 0;
	virtual void checkEvents() = 0;
	string getKeymap() { return keymapString; }
	void setInputInterface(weak_ptr<InputInterface> ptr) { inputInterface = ptr; }
	weak_ptr<InputInterface> get_input_interface() { return inputInterface; }

	enum Type { X11_GLX, X11_EGL, DRM, SCRIMSHAW };
	static void setup(Type type);

	static unique_ptr<Backend> instance;

protected:
	Backend();

	weak_ptr<InputInterface> inputInterface;

private:
	string keymapString;
};
