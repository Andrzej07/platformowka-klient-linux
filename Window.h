#pragma once
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <string>


enum WindowFlags { INVISIBLE = 1, FULLSCREEN = 2, BORDERLESS = 4 };

class Window
{
public:
	Window();
	~Window();

	int create(const std::string &windowName, int screenWidth, int screenHeight, unsigned int flags);

	void swapBuffer();

    int getScreenHeight() { return m_screenHeight; }
    int getScreenWidth() { return m_screenWidth; }

private:
    SDL_Window* m_sdlWindow;
    int m_screenWidth;
    int m_screenHeight;
};
