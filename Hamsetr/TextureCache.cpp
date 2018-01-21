#include <iostream>

#include "TextureCache.h"
#include "ImageLoader.h"

namespace Hamsetr {
	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(std::string& filePath) {
		auto mit = _textureMap.find(filePath);

		if (mit == _textureMap.end()) {
			GLTexture newTexture = ImageLoader::loadPNG(filePath);

			_textureMap.insert(make_pair(filePath, newTexture));
			return newTexture;
		}

		return mit->second;
	}
}