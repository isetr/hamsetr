#include "ResourceManager.h"

namespace Hamsetr {
	TextureCache ResourceManager::_textureCache;

	GLTexture ResourceManager::getTexture(std::string filePath) {
		return _textureCache.getTexture(filePath);
	}
}
