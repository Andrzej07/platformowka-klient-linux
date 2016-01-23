#pragma once
#include <map>
#include "GLTexture.h"

class TextureCache
{
public:
	TextureCache();
	~TextureCache();

	GLTexture getTexture(const std::string &texturePath);

private:
	std::map<std::string, GLTexture> _textureMap;
};

