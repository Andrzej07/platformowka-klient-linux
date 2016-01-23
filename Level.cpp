#include "Level.h"
#include "SpriteBatch.h"
#include <fstream>

Level::Level()
{
}


Level::~Level()
{
}

glm::vec2 Level::load(const std::string &filepath)
{
    m_texture = ResourceManager::getTexture("Assets/placeholder_button.png");
    std::ifstream file;
    file.open(filepath, std::ios_base::in);

    float startx,starty, stopx,stopy;
    file>>startx>>starty>>stopx>>stopy;
    m_deathHeight = starty;
    while (!file.eof()){
        float cornerx,cornery,sizex,sizey;
        file>>cornerx>>cornery>>sizex>>sizey;
        if (cornery < m_deathHeight)
            m_deathHeight = cornery;
        m_levelData.emplace_back(glm::vec2(cornerx,cornery), glm::vec2(sizex,sizey));
    }
    m_deathHeight -= 25.f;
    m_finishPoint.pos = glm::vec2(stopx,stopy);
/*    // todo: read finish line ?
	for (int i = 0; i < 500; ++i)
    {
		m_levelData.emplace_back(glm::vec2(100.0f - i*10.0f, 60.0f), glm::vec2(10.0f, 6.0f));
    }*/
    // todo: read blocks from file
    return glm::vec2(startx,starty);
}

void Level::draw(SpriteBatch& spriteBatch)
{
	for (auto& block : m_levelData)
	{
		spriteBatch.draw(glm::vec4(block.pos.x, block.pos.y, block.size.x, block.size.y), glm::vec4(0,0,1,1), m_texture.id, 0, m_color);
	}
    spriteBatch.draw(glm::vec4(m_finishPoint.pos.x-2.f, m_finishPoint.pos.y-2.f, m_finishPoint.size.x, m_finishPoint.size.y), glm::vec4(0,0,1,1), m_texture.id, 0, ColorRGBA8(20,20,220,255));
}

bool Level::isBelowLevel(const glm::vec2& point)
{
    return point.y < m_deathHeight;
}
const Block& Level::getFinishPoint()
{
    return m_finishPoint;
}
