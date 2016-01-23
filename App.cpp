#include "App.h"
#include <string>

App::App() :
m_screenWidth(800),
m_screenHeight(640)
{
}


App::~App()
{
    if(m_menuscr)
        delete m_menuscr;
    if(m_gamescr)
        delete m_gamescr;
    if(m_multiscr)
        delete m_multiscr;
    SDL_Quit();
}

void App::run()
{
	createWindow();
	while (true)
	{
        std::string serverIP;
        m_menuscr = new MenuScreen();
        MenuReturn userChoice = m_menuscr->run(&m_window, serverIP);
		switch (userChoice)
		{
		case MenuReturn::SINGLEPLAYER:
            m_gamescr = new GameScreen();
            m_gamescr->run(&m_window);
            delete m_gamescr;
			break;
		case MenuReturn::MULTIPLAYER:
            m_multiscr = new MultiScreen(serverIP);
            m_multiscr->run(&m_window);
            delete m_multiscr;
			break;
		case MenuReturn::EDITOR:
			break;
		case MenuReturn::NONE:
			break;
		}
        delete m_menuscr;
	}
}

void App::createWindow()
{
	SDL_Init(SDL_INIT_EVERYTHING);
    if(!SDL_WasInit(SDL_INIT_EVERYTHING))
    {
        printf("SDL_INIT_ERROR: %s\n", SDL_GetError());
        exit(1);
    }

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	m_window.create("Super Platformer", m_screenWidth, m_screenHeight, 0);
}
