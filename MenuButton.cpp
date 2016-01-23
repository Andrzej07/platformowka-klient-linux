#include "MenuButton.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "ResourceManager.h"

namespace {
	const int LETTER_WIDTH = 16;
}

MenuButton::MenuButton(glm::vec2 pos, glm::vec2 size, std::string caption) :
	m_size(size), m_pos(pos), m_caption(caption)
{
	m_texID = ResourceManager::getTexture("Assets/placeholder_button.png").id;
	float textSize = static_cast<float> ((m_caption.length() + 2) * LETTER_WIDTH);
	if (textSize > m_size.x)
		m_size.x = textSize;
	
	m_captPos.y = m_pos.y + 5; // 5 looks good :D
	m_captPos.x = LETTER_WIDTH + m_pos.x + (m_size.x - textSize) / 2;
}
MenuButton::~MenuButton()
{
}

void MenuButton::draw(SpriteBatch &spriteBatch, SpriteFont *spriteFont)
{	
	spriteBatch.draw(glm::vec4(m_pos.x, m_pos.y, m_size.x, m_size.y), glm::vec4(0, 0, 1, 1), m_texID, 0.1f, m_color);

	spriteFont->draw(spriteBatch, m_caption.c_str(), m_captPos,
	glm::vec2(0.5f), 0, ColorRGBA8(0, 0, 0, 255));
}

bool MenuButton::isMouseOnButton(const glm::vec2 &mousePos)
{
	bool y = (mousePos.y > m_pos.y) && (mousePos.y < m_pos.y + m_size.y);
	bool x = (mousePos.x > m_pos.x) && (mousePos.x < m_pos.x + m_size.x);
	bool is = x && y;
	m_color = ColorRGBA8(200 * !is, 200 * is, 0, 250);
	return is;
}