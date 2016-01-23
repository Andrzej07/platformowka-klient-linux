#include "GameScreen.h"
#include "ResourceManager.h"

#define PLAYER_SIZE 10

GameScreen::GameScreen() :
	m_screenWidth(1440),
	m_screenHeight(900)
{
}


GameScreen::~GameScreen()
{
}

void GameScreen::run(Window *window)
{
	m_window = window;
	initSystems();
	initShaders();

	runLoop();
}
void GameScreen::initSystems()
{
	m_camera.init(m_screenWidth, m_screenHeight);
	m_spriteBatch.init();

    glm::vec2 playerPos = m_level.load("Levels/level0.txt");

	// Init players
    m_player1.init(playerPos, glm::vec2(PLAYER_SIZE), "Assets/dude.png");
}

void GameScreen::initShaders()
{
	// Compile our texture shaders and define attributes
	m_shaderProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_shaderProgram.addAttribute("vertexPosition");
	m_shaderProgram.addAttribute("vertexColor");
	m_shaderProgram.addAttribute("vertexUV");
	m_shaderProgram.linkShaders();
}
void GameScreen::runLoop()
{
	// Add game states?
    while (!isFinished)
	{
		m_timer.start();
		m_inputManager.update();
		// Poll SDL events for input
		processInput();
		// Recalculate camera matrix if needed
		m_camera.setPosition(m_player1.getPosition());
		m_camera.update();
		// Update physics, objects etc
        //while (m_timer.canGetTimeChunk())
        while(m_timer.canGetTimeChunk())
			update(m_timer.getTimeChunk());
		// Draw game
		draw();
		m_window->swapBuffer();
		m_timer.stop();
	}
}
void GameScreen::draw()
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


	m_spriteBatch.begin();
	// NOTE: Use culling with m_camera.isBoxInView

	m_level.draw(m_spriteBatch);
	m_player1.draw(m_spriteBatch);

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
void GameScreen::update(float frameTime)
{
	// TODO: Update player state
	m_player1.update(frameTime, m_level, &m_inputManager);
    if(m_level.isBelowLevel(m_player1.getPosition()))
        m_player1.returnToLastGround();

    if(m_player1.intersects(m_level.getFinishPoint()))
        isFinished = true;
}
void GameScreen::processInput()
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
	if (m_inputManager.isKeyPressed(SDLK_ESCAPE))
	{
        //SDL_Quit();
        //exit(0);
        isFinished = true;
	}

	float scale = m_camera.getScale();
	if (m_inputManager.isKeyDown(SDLK_q))
		m_camera.setScale(scale + 0.1f);
	if (m_inputManager.isKeyDown(SDLK_e))
		m_camera.setScale(scale - 0.1f);
}
