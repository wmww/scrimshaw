#pragma once

#include "hedgehog/util.h"
#include "InputInterface.h"

class WindowInterface
{
public:
	virtual void setSize(V2i size) = 0;
	virtual weak_ptr<InputInterface> getInputInterface() = 0;
};

#include "Scene.h" // must be at bottom for include order
