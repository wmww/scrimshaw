#pragma once

#include "../main/util.h"

// there is almost no cost to creating a texture, initialization is lazy and done on first load
class Texture
{
public:
	Texture() {}
	
	void setupEmpty();
	void loadFromData(void * data, V2i dim, uint32_t format);
    V2i get_dim();
    bool * get_data();
	
	inline bool isNull() { return impl == nullptr; };
	inline bool isValid() { return !isNull(); };
	
private:
	struct Impl;
	shared_ptr<Impl> impl;
};

