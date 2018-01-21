#include "Window.h"
#include "FatalErrors.h"

namespace Hamsetr {
	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {

		Uint32 flags = SDL_WINDOW_OPENGL;
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;

		if (currentFlags & INVISIBLE) {
			flags |= SDL_WINDOW_HIDDEN;
		}

		if (currentFlags & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		if (currentFlags & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
		if (_sdlWindow == nullptr) {
			fatalError("SDL Window could not be created");
		}

		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
		if (glContext == nullptr) {
			fatalError("SDL_GL context could not be created!");
		}

		glewExperimental = true;
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("Could not init glew");
		}

		std::printf("*** OpenGL version: %s ***\n", glGetString(GL_VERSION));

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		SDL_GL_SetSwapInterval(0); //V-sync

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swapBuffer() {
		SDL_GL_SwapWindow(_sdlWindow);
	}
}