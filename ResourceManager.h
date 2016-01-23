#pragma once
#include "TextureCache.h"

class ResourceManager
{
public:
	static GLTexture getTexture(const std::string &texturePath);

private:
	static TextureCache _textureCache;
};

