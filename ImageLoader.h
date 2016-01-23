#pragma once
#include "GLTexture.h"
#include <string>

class ImageLoader
{
public:
	static GLTexture loadPNG(const std::string filename);
};

