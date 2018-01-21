#pragma once

#include <GL/glew.h>

namespace Hamsetr {
	struct Position {
		float x, y;
	};

	class ColorRGBA8 {
	public: 
		ColorRGBA8() : r(0), g(0), b(0), a(0) {}
		ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) : r(R), g(G), b(B), a(A) {}
		GLubyte r, g, b, a;
	};

	struct UV {
		float u, v;
	};

	struct Vertex {
		Position position;
		ColorRGBA8 color;
		UV uv;

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}

		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}
	};
}