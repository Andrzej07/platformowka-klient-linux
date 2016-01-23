#include "MultiScreen.h"
#include "ResourceManager.h"
#include "Errors.h"
#include <cmath>
#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
//#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PLAYER_SIZE 10

#define PORT 1234   //The port on which to send data
#define PORT2 1235   //The port on which to send data

MultiScreen::MultiScreen()
{
}

MultiScreen::MultiScreen(const std::string &serverIP) :
    m_serverIP(serverIP)
{
  //  m_screenHeight = m_window->getScreenHeight();
  //  m_screenWidth = m_window->getScreenWidth();
}


MultiScreen::~MultiScreen()
{
}

void MultiScreen::run(Window *window)
{
    m_window = window;
    m_screenHeight = m_window->getScreenHeight();
    m_screenWidth = m_window->getScreenWidth();
    initSystems();
    initShaders();

    runLoop();
}
void MultiScreen::initSystems()
{
    m_camera.init(m_screenWidth, m_screenHeight);
    m_spriteBatch.init();

}

void MultiScreen::initShaders()
{
    // Compile our texture shaders and define attributes
    m_shaderProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    m_shaderProgram.addAttribute("vertexPosition");
    m_shaderProgram.addAttribute("vertexColor");
    m_shaderProgram.addAttribute("vertexUV");
    m_shaderProgram.linkShaders();
}

void MultiScreen::initSocket()
{
    int enable = 1;


    /* address structure */
    memset(&stAddr, 0, sizeof(stAddr));
    stAddr.sin_family = AF_INET;
    //stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stAddr.sin_port = htons(PORT);
    if(inet_aton(m_serverIP.c_str(), &stAddr.sin_addr)==0)
        exit(1);

    /* create a socket */
    m_mainSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_mainSocket < 0)
    {
        fatalError("Could not create socket\n");
    }
    setsockopt(m_mainSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&enable, sizeof(enable));

    socklen_t slen = sizeof(stAddr);
    char msg = 'c';
    // Report to game server
    if (sendto(m_mainSocket, &msg, sizeof(char) , 0 , (struct sockaddr *) &stAddr, slen)==-1)
    {
        fatalError("Failed to report to server (first message).\n");
    }
    char answer;
    // Receive player number from server
    if (recvfrom(m_mainSocket, &answer, sizeof(char), 0, (struct sockaddr *) &stAddr, &slen) == -1)
    {
        fatalError("Failed to receive player number from server.\n");
    }
    if (answer=='2'){
        m_player1.setColor(2);
        m_player2.setColor(1);
        stAddr.sin_port = htons(PORT2);
    }
    else
    {
        m_player1.setColor(1);
        m_player2.setColor(2);
    }
    char levelName[20];
    memset(&levelName, '\0', 20);
    recvfrom(m_mainSocket, levelName, 20, 0, (struct sockaddr *) &stAddr, &slen);

    std::cout << std::string(levelName) << "\n";
    glm::vec2 playerPos = m_level.load("Levels/" + std::string(levelName));

    m_player1.init(playerPos, glm::vec2(PLAYER_SIZE), "Assets/dude.png");
    m_player2.init(playerPos, glm::vec2(PLAYER_SIZE), "Assets/dude.png");
    // Send ready for game
    msg = 'r';
    sendto(m_mainSocket, &msg, sizeof(char), 0, (struct sockaddr *)&stAddr, slen);

    do{
        recvfrom(m_mainSocket, &answer, sizeof(char), 0, (struct sockaddr*)&stAddr, &slen);
    }while(answer != 'g');
    /* bind a name to a socket */

    /*
    nBind = bind(m_mainSocket, (struct sockaddr*)&stAddr, sizeof(struct sockaddr));
    if (nBind < 0)
    {
        exit(1);
    } */
    fcntl(m_mainSocket, F_SETFL, O_NONBLOCK,1);

}

void MultiScreen::runLoop()
{
        /* block for connection request
    while(1)
    {
         nTmp = sizeof(stClientAddr);
     int status = recvfrom(nSocket, bufor, BUFSIZE, 0, (struct sockaddr*) &stClientAddr, &nTmp);
     status = sendto(nSocket, bufor, strlen(bufor)+1, 0, (struct sockaddr*)&stClientAddr, sizeof(stClientAddr));

    }
  */
    initSocket();

    while (m_isRunning)
    {
        m_timer.start();
        m_inputManager.update();
        // Poll SDL events for input
        processInput();
        sendInput();
        // Recalculate camera matrix if needed
        m_camera.setPosition(m_player1.getPosition());
        m_camera.update();

        float x[4];
        socklen_t slen = sizeof(stAddr);
        int rcvd = recvfrom(m_mainSocket, x, 4*sizeof(float), 0,(struct sockaddr*) &stAddr, &slen);
        if(rcvd!=-1)
        {
            if(fabs(x[0]-666.f)<0.1f && fabs(x[1]-666.f)<0.1f && fabs(x[2]-666.f)<0.1f && fabs(x[3]-666.f)<0.1f)
            {
                quit();
            }
            else
            {
                m_player1.setPosition(glm::vec2(x[0],x[1]));
                m_player2.setPosition(glm::vec2(x[2], x[3]));
            }
        }
        // Draw game
        draw();
        m_window->swapBuffer();
        m_timer.stop();
    }
}
void MultiScreen::draw()
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
    m_player2.draw(m_spriteBatch);

    m_spriteBatch.end();
    m_spriteBatch.renderBatch();
    m_shaderProgram.unuse();
}

void MultiScreen::sendInput()
{
    socklen_t slen = sizeof(stAddr);
    char msg;
    if(m_inputManager.isKeyDown(SDLK_a))
    {
        msg = 'a';
        sendto(m_mainSocket, &msg, sizeof(char) , 0 , (struct sockaddr *) &stAddr, slen);
    }
    if(m_inputManager.isKeyDown(SDLK_s))
    {
        msg = 's';
        sendto(m_mainSocket, &msg, sizeof(char) , 0 , (struct sockaddr *) &stAddr, slen);
    }
    if(m_inputManager.isKeyDown(SDLK_w))
    {
        msg = 'w';
        sendto(m_mainSocket, &msg, sizeof(char) , 0 , (struct sockaddr *) &stAddr, slen);
    }
    if(m_inputManager.isKeyDown(SDLK_d))
    {
        msg = 'd';
        sendto(m_mainSocket, &msg, sizeof(char) , 0 , (struct sockaddr *) &stAddr, slen);
    }
}

void MultiScreen::quit()
{
    //SDL_Quit();
    socklen_t slen = sizeof(stAddr);
    char msg = 'q';
    sendto(m_mainSocket, &msg, sizeof(char) , 0 , (struct sockaddr *) &stAddr, slen);
    close(m_mainSocket);
    //exit(0);
    m_isRunning = false;
}

void MultiScreen::processInput()
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
        quit();
    }

    float scale = m_camera.getScale();
    if (m_inputManager.isKeyDown(SDLK_q))
        m_camera.setScale(scale + 0.1f);
    if (m_inputManager.isKeyDown(SDLK_e))
        m_camera.setScale(scale - 0.1f);
}
