#pragma once

#include <map>
#include "GLTexture.h"

namespace Hamsetr {
	class TextureCache
	{
	public:
		TextureCache();
		~TextureCache();

		GLTexture getTexture(std::string& filePath);

	private:
		std::map<std::string, GLTexture> _textureMap;
	};
}
