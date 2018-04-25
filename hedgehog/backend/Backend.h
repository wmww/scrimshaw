#pragma once

#include "../main/util.h"
#include "../scene/InputInterface.h"
#include "Texture.h"

class Backend
{
public:
    virtual void draw(Texture texture, V2d pos) = 0;
	virtual void swapBuffer() = 0;
	virtual void checkEvents() = 0;
	string getKeymap() { return keymapString; }
	void setInputInterface(weak_ptr<InputInterface> ptr) { inputInterface = ptr; }
	
	enum Type {X11_GLX, X11_EGL, DRM, SCRIMSHAW};
	static void setup(Type type);
	
	static unique_ptr<Backend> instance;
	
protected:
	Backend();
	
	weak_ptr<InputInterface> inputInterface;
	
private:
	string keymapString;
};

