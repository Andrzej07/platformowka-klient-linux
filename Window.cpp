#include "Window.h"
#include "Errors.h"


Window::Window()
{
}


Window::~Window()
{
}

int Window::create(const std::string &windowName, int screenWidth, int screenHeight, unsigned int passedFlags)
{
	Uint32 flags = SDL_WINDOW_OPENGL;

	if (passedFlags & INVISIBLE)
	{
		flags |= SDL_WINDOW_HIDDEN;
	}
	if (passedFlags & FULLSCREEN)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}
	if (passedFlags & BORDERLESS)
	{
		flags |= SDL_WINDOW_BORDERLESS;
	}

    m_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags | SDL_WINDOW_OPENGL);
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    if (m_sdlWindow == nullptr)
	{
        printf("SDL_CreateWindow_Error: %s\n", SDL_GetError());
		fatalError("SDL Window could not be created!");
	}

    SDL_GLContext glContext = SDL_GL_CreateContext(m_sdlWindow);
	if (glContext == nullptr)
	{
		fatalError("SDL_GL context could not be created!");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		fatalError("glewInit failed!");
	}

	printf("****  OpenGL version: %s  ****\n", glGetString(GL_VERSION));

	glClearColor(1, 1, 1, 1);

	// Activate vsync
	SDL_GL_SetSwapInterval(1);

	// Enable alpha blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	SDL_GL_SetSwapInterval(1);
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

	return 0;
}

void Window::swapBuffer()
{
    SDL_GL_SwapWindow(m_sdlWindow);
}
void Window::MakeCurrent()
{
    SDL_GL_MakeCurrent(m_sdlWindow, m_glContext);
}
