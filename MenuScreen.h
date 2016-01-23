#pragma once

#include "Camera2D.h"
#include "SpriteBatch.h"
#include "GLSLProgram.h"
#include "Window.h"
#include "Timer.h"
#include "SpriteFont.h"
#include "MenuButton.h"
#include "InputManager.h"
#include <vector>
#include <map>

enum MenuGroups { MAIN, SINGLE, MULTI };
enum class MenuReturn { NONE, SINGLEPLAYER, MULTIPLAYER, EDITOR };
class MenuScreen
{
private:
	friend class App;
	MenuScreen();
	~MenuScreen();
    MenuReturn run(Window *window, std::string&);

	void initSystems();
	void initMenu();
	void initShaders();
	void runLoop();
	void draw();
	void update(float frameTime);
	void processInput();
	void moveToMenuGroup(MenuGroups group);

    void validateIPString(char);

	Window *m_window;
	Timer m_timer;
	InputManager m_inputManager;
	Camera2D m_camera;
	GLSLProgram m_shaderProgram;
	SpriteBatch m_spriteBatch;
	SpriteFont *m_spriteFont;

	MenuReturn m_menuReturn = MenuReturn::NONE;

	std::vector<MenuButton> m_menuButtons;
	std::map<MenuGroups, glm::vec2> m_menuPositions;
	MenuGroups m_whereAreWe = MAIN;
    std::string m_multiInputText;
    int multiTextDots = 0;
    std::string m_singleInputText;

	int m_highlightedButton = -1; // index of highlighted button

	bool m_isRunning = true;

    int m_screenWidth;
    int m_screenHeight;
};
