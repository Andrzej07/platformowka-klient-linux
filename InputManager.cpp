#include "InputManager.h"


InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

void InputManager::setMouseCoords(float x, float y)
{
	m_mouseCoords = glm::vec2(x,y);
}
void InputManager::pressKey(unsigned int keyID)
{
	m_keyMap[keyID] = true;
}

void InputManager::releaseKey(unsigned int keyID)
{
	m_keyMap[keyID] = false;
}
	
bool InputManager::isKeyDown(unsigned int keyID)
{
	auto it = m_keyMap.find(keyID);
	
	if(it != m_keyMap.end()) return it->second;
	else return false;
}
bool InputManager::wasKeyDown(unsigned int keyID)
{
	auto it = m_previousKeyMap.find(keyID);

	if (it != m_previousKeyMap.end()) return it->second;
	else return false;
}

bool InputManager::isKeyPressed(unsigned int keyID)
{
	if (isKeyDown(keyID) && !wasKeyDown(keyID)){
		return true;
	}
	return false;
}

void InputManager::update()
{
	for (auto &it : m_keyMap)
	{
		m_previousKeyMap[it.first] = it.second;
	}
}
