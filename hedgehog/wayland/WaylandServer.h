#pragma once

#include "hedgehog/util.h"

namespace WaylandServer
{
void setup();

void shutdown();

void iteration();

uint32_t nextSerialNum();

}; // namespace WaylandServer
