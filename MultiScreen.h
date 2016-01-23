#pragma once

#include "Camera2D.h"
#include "SpriteBatch.h"
#include "GLSLProgram.h"
#include "Window.h"
#include "Timer.h"
#include "Player.h"
#include "Level.h"
// MOVE TO PLAYER CLASS
#include "InputManager.h"

#include <netinet/in.h>

class MultiScreen
{
private:
    friend class App;
    MultiScreen();
    MultiScreen(const std::string &serverIP);
    ~MultiScreen();
    void run(Window *window);

    void initSystems();
    void initShaders();
    void runLoop();
    void draw();
    void processInput();
    void sendInput();
    void quit();
    void initSocket();


    Level m_level;
    Timer m_timer;
    Window *m_window;
    InputManager m_inputManager;
    Camera2D m_camera;
    GLSLProgram m_shaderProgram;
    SpriteBatch m_spriteBatch;

    Player m_player1;
    Player m_player2;

    int m_mainSocket;
    std::string m_serverIP;
    struct sockaddr_in stAddr;

    int m_screenWidth;
    int m_screenHeight;
    bool m_isRunning=true;
};

