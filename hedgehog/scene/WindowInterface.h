#pragma once

#include "../main/util.h"
#include "InputInterface.h"
#include "../backend/Texture.h"

class WindowInterface
{
public:
	virtual void setSize(V2i size) = 0;
	virtual weak_ptr<InputInterface> getInputInterface() = 0;
	
	Texture texture;
};

#include "Scene.h" // must be at bottom for include order
