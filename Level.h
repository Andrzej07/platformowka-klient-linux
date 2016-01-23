#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Vertex.h"
#include "ResourceManager.h"

class SpriteBatch;
struct Block
{
	Block(glm::vec2 pos, glm::vec2 size) :
		pos(pos), size(size) {}
	glm::vec2 pos;
	glm::vec2 size;
	// rotation? etc.
};

class Level
{
public:
	Level();
	~Level();

	void draw(SpriteBatch& spriteBatch);

	glm::vec2 load(const std::string &filepath);
    bool isBelowLevel(const glm::vec2&);
    const Block& getFinishPoint();

	const std::vector<Block>& getLevelData() const { return m_levelData; }

private:
	std::vector<Block> m_levelData;
	ColorRGBA8 m_color = ColorRGBA8(0,0,0,255);
	GLTexture m_texture;

    Block m_finishPoint = Block(glm::vec2(0.f), glm::vec2(4.f));
    float m_deathHeight = 0.0f;
};

