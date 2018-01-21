#pragma once

#include "GLTexture.h"
#include <string>

namespace Hamsetr {
	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};
}

