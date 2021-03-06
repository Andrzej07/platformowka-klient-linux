#pragma once
#include <unordered_map>
#include <glm/glm.hpp>


class InputManager
{
public:
	InputManager();
	~InputManager();

	void update();

	void setMouseCoords(float x, float y);
	glm::vec2 getMouseCoords() const { return m_mouseCoords; }
	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);
	bool isKeyDown(unsigned int keyID);
	bool isKeyPressed(unsigned int keyID); // key was just pressed

private:
	bool wasKeyDown(unsigned int keyID);
	std::unordered_map<unsigned int, bool> m_keyMap;
	std::unordered_map<unsigned int, bool> m_previousKeyMap;
	glm::vec2 m_mouseCoords;
};

