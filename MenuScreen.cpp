#include "MenuScreen.h"
#include <ctype.h>
#include <iostream>
#include <sstream>

const float CAMERA_SCALE = 1.7f;
MenuScreen::MenuScreen() :
	m_screenWidth(1440),
	m_screenHeight(900)
{
}


MenuScreen::~MenuScreen()
{
}

void MenuScreen::initSystems()
{
	//m_spriteFont = new SpriteFont("Fonts/leadcoat.ttf", 32);
	m_spriteFont = new SpriteFont("Fonts/Aller_Bd.ttf", 64);
	m_camera.init(m_screenWidth, m_screenHeight);
	m_camera.setScale(CAMERA_SCALE);
	m_spriteBatch.init();
}

MenuReturn MenuScreen::run(Window *window, std::string &serverIP)
{
	m_window = window;
	initSystems();
	initShaders();
	initMenu();

    m_multiInputText = "127.0.0.1";
    multiTextDots = 3;
	runLoop();
    serverIP = m_multiInputText;
	return m_menuReturn;
}

void MenuScreen::initMenu()
{
	m_menuPositions[MAIN] = glm::vec2(0.0f);
	const int NUM_BUTTONS = 4;
	const glm::vec2 BUTTON_SIZE = glm::vec2(150.0f, 50.0f);
	const std::string CAPTIONS[NUM_BUTTONS] = { "Single Player", "Multiplayer", "Editor", "Exit" };
	for (int i = 0; i < NUM_BUTTONS; ++i)
	{
		m_menuButtons.emplace_back(MenuButton(glm::vec2(-50.0f, 50.0f - (BUTTON_SIZE.y+1.0f) * float(i)), BUTTON_SIZE, CAPTIONS[i]));
	}

	m_menuPositions[SINGLE] = glm::vec2(1000.0f, 0.0f);


	m_menuButtons.emplace_back(MenuButton(glm::vec2(1000.0f-BUTTON_SIZE.x/2, 51.0f), BUTTON_SIZE, "Launch"));
	m_menuButtons.emplace_back(MenuButton(glm::vec2(1000.0f - BUTTON_SIZE.x / 2, 0.0f), BUTTON_SIZE, "Back"));

	m_menuPositions[MULTI] = glm::vec2(0.0f, 1000.0f);
	m_menuButtons.emplace_back(MenuButton(glm::vec2(0.0f - BUTTON_SIZE.x / 2, 1051.0f), BUTTON_SIZE, "Join server"));
	m_menuButtons.emplace_back(MenuButton(glm::vec2(0.0f - BUTTON_SIZE.x / 2, 1000.0f), BUTTON_SIZE, "Back"));
}

void MenuScreen::initShaders()
{
	// Compile our texture shaders and define attributes
	m_shaderProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_shaderProgram.addAttribute("vertexPosition");
	m_shaderProgram.addAttribute("vertexColor");
	m_shaderProgram.addAttribute("vertexUV");
	m_shaderProgram.linkShaders();
}
void MenuScreen::runLoop()
{
	// Add game states?
	while (m_isRunning)
	{
		m_timer.start();
		m_inputManager.update();
		// Poll SDL events for input
		processInput();
		// Recalculate camera matrix if needed
		m_camera.update();
		// Update physics, objects etc
		while (m_timer.canGetTimeChunk())
			update(m_timer.getTimeChunk());
		// Draw game
		draw();
		m_window->swapBuffer();
		m_timer.stop();
	}
}
void MenuScreen::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);

	// Grab the camera matrix
	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();

	m_shaderProgram.use();

	// Make sure the shader uses texture 0
	GLint textureUniform = m_shaderProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);
	// Send camera matrix to shader - transforms world coords into screen space coords
	GLint pUniform = m_shaderProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);


	m_spriteBatch.begin(BACK_TO_FRONT);
	// DRAW STUFF using m_spriteBatch.draw
	
	// draw buttons..
	for (auto &button : m_menuButtons)
	{
		button.draw(m_spriteBatch, m_spriteFont);
	}
	// draw input text (IP for multi, level name for single)
	m_spriteFont->draw(m_spriteBatch,"SINGLE PLAYER", m_menuPositions[SINGLE] + glm::vec2(-7.0f*16.0f, 120.0f),
		glm::vec2(0.7f), 0, ColorRGBA8(0, 0, 0, 255));
	m_spriteFont->draw(m_spriteBatch, m_singleInputText.c_str(), m_menuPositions[SINGLE] - glm::vec2(m_singleInputText.length() * 10, 100.0f),
	glm::vec2(0.7f), 0, ColorRGBA8(0, 0, 0, 255));
	m_spriteFont->draw(m_spriteBatch, "MULTIPLAYER", m_menuPositions[MULTI] + glm::vec2(-6.5f*16.0f, 120.0f),
		glm::vec2(0.7f), 0, ColorRGBA8(0, 0, 0, 255));
	m_spriteFont->draw(m_spriteBatch, m_multiInputText.c_str(), m_menuPositions[MULTI]-glm::vec2(m_multiInputText.length()*10,100.0f),
		glm::vec2(0.7f), 0, ColorRGBA8(0, 0, 0, 255));
	// Test-example draw
	/*	glm::vec4 destRect; // POSITION and SIZE  (x_pos, y_pos, x_size, y_size)
	// Position is bottom left corner
	destRect = glm::vec4(0, 0, 10.0f, 10.0f);
	glm::vec4 uvRect = glm::vec4(0, 0, 1.0f, 1.0f);  // TEXTURE COORDINATES
	GLTexture texture = ResourceManager::getTexture("Assets/dude.png"); // TEXTURE PICTURE returns struct containing SIZE and OpenGL texture handle
	m_spriteBatch.draw(destRect, uvRect, texture.id, 0, ColorRGBA8(255, 0, 0, 255));   */

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();
	m_shaderProgram.unuse();
}
void MenuScreen::update(float frameTime)
{
	for (size_t i = 0; i < m_menuButtons.size(); ++i)
	{
		glm::vec2 mousePos = m_camera.convertScreenToWorld(m_inputManager.getMouseCoords());
		if (m_menuButtons[i].isMouseOnButton(mousePos))
		{
			m_highlightedButton = i;
			break;
		}
		else
			m_highlightedButton = -1;
	}
	for (size_t i = m_highlightedButton + 1; i < m_menuButtons.size(); ++i)
	{
		glm::vec2 mousePos = m_camera.convertScreenToWorld(m_inputManager.getMouseCoords());
		m_menuButtons[i].isMouseOnButton(mousePos);
	}
}

void MenuScreen::validateIPString(char next)
{
    if(isdigit(next) && m_multiInputText.empty())
    {
      m_multiInputText += next;
    }
    else if(isdigit(next))
    {
        m_multiInputText += next;
        std::vector<int> vect;

        std::stringstream ss(m_multiInputText);

        int i;

        while (ss >> i)
        {
            vect.push_back(i);

            if (ss.peek() == '.')
                ss.ignore();
        }
        if(vect.back() > 255)
            m_multiInputText.pop_back();
    }
    else if(next=='.' && !m_multiInputText.empty())
    {
        if(m_multiInputText.back() != '.' && multiTextDots<3)
        {
            m_multiInputText += next;
            multiTextDots++;
        }
    }
}

void MenuScreen::processInput()
{
	// Polls SDL events and reports them to input manager
	// TODO: Move to player class ?
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			exit(0);
			break;
		case SDL_TEXTINPUT:		
			if (m_whereAreWe == SINGLE)
				m_singleInputText += evnt.text.text;
			else if (m_whereAreWe == MULTI)
                validateIPString(*evnt.text.text);
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
	if (m_inputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		if (m_highlightedButton != -1)
		{
			MenuButton *button = &m_menuButtons[m_highlightedButton];
			std::string capt = button->getCaption();
			if (capt == "Exit")
			{
				SDL_Quit();
				exit(0);
			}
			else if (capt == "Single Player")
				moveToMenuGroup(SINGLE);
			else if (capt == "Multiplayer")
				moveToMenuGroup(MULTI);
			else if (capt == "Back")
				moveToMenuGroup(MAIN);
			else if (capt == "Launch")
			{
				m_menuReturn = MenuReturn::SINGLEPLAYER;
				m_isRunning = false;
			}
            else if (capt == "Join server")
            {
                if(multiTextDots == 3)
                {
                    m_menuReturn = MenuReturn::MULTIPLAYER;
                    m_isRunning = false;
                }
            }
		}
	}

	if (m_inputManager.isKeyPressed(SDLK_BACKSPACE))
    {
		if (m_whereAreWe == SINGLE)
        {
			m_singleInputText.pop_back();
        }
		else if (m_whereAreWe == MULTI)
        {
            if(m_multiInputText.back() == '.')
                multiTextDots--;
			m_multiInputText.pop_back();
        }
    }
/*	float scale = m_camera.getScale();
    if (m_inputManager.isKeyDown(SDLK_q))
		m_camera.setScale(scale + 0.1f);
	if (m_inputManager.isKeyDown(SDLK_e))
		m_camera.setScale(scale - 0.1f); */
}

void MenuScreen::moveToMenuGroup(MenuGroups group)
{
	// if find bla bla bla
	m_whereAreWe = group;
	m_camera.setPosition(m_menuPositions[group]);
}
