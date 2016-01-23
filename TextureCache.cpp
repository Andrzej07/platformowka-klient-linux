#include "TextureCache.h"
#include "Errors.h"
#include "ImageLoader.h"

TextureCache::TextureCache()
{
}


TextureCache::~TextureCache()
{
}

GLTexture TextureCache::getTexture(const std::string &texturePath)
{
	std::map<std::string, GLTexture>::iterator iter = _textureMap.find(texturePath);

	if (iter == _textureMap.end())
	{
		GLTexture newTexture = ImageLoader::loadPNG(texturePath);

		//	std::pair<std::string, GLTexture> newPair(texturePath, newTexture);

		//	_textureMap.insert(newPair);
		_textureMap.insert(make_pair(texturePath, newTexture));
		return newTexture;
	}

	return iter->second;
}
