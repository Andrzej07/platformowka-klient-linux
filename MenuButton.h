#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include "Vertex.h" // contains color struct :(

class SpriteBatch;
class SpriteFont;
class MenuButton
{
public:
	MenuButton(glm::vec2 pos, glm::vec2 size, std::string caption);
	~MenuButton();

	void draw(SpriteBatch &spriteBatch, SpriteFont *spriteFont);

	bool isMouseOnButton(const glm::vec2 &mousePos);


	std::string getCaption() { return m_caption; }

private:
	glm::vec2 m_size;
	glm::vec2 m_captPos = glm::vec2(0.0f);
	glm::vec2 m_pos;
	std::string m_caption;
	ColorRGBA8 m_color = ColorRGBA8(120, 120, 120, 240);
	GLuint m_texID = 0;
};

