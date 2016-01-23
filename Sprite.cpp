#include "Sprite.h"
#include "Vertex.h"
#include <cstddef>
#include "ResourceManager.h"


Sprite::Sprite() :
	_vboID(0)
{
}


Sprite::~Sprite()
{
	if (_vboID != 0)
	{
		glDeleteBuffers(1, &_vboID);
	}
}

void Sprite::init(float x, float y, float width, float height, const std::string &texturePath)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;

	_texture = ResourceManager::getTexture(texturePath);

	if (_vboID == 0)
	{
		glGenBuffers(1, &_vboID);
	}

	const int numVertex = 6;
	Vertex vertexData[numVertex]; // 6 vertices,  x y for each

	vertexData[0].setPosition(x + width, y + height);
	vertexData[0].setUV(1, 1);

	vertexData[1].setPosition(x, y + height);
	vertexData[1].setUV(0, 1);

	vertexData[2].setPosition(x, y);
	vertexData[2].setUV(0, 0);

	//vertexData[3].setPosition(x,y);
	//vertexData[3].setUV(0,0);
	vertexData[3] = vertexData[2];

	vertexData[4].setPosition(x + width, y);
	vertexData[4].setUV(1, 0);

	vertexData[5] = vertexData[0];
	//vertexData[5].setUV(1, 1);

	for (int i = 0; i < numVertex; i++)
	{
		vertexData[i].setColor(0, 255, 255, 255);
	}

	vertexData[1].color.r = 255;
	vertexData[1].color.g = 100;


	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::draw()
{
	glBindTexture(GL_TEXTURE_2D, _texture.id);

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);

	glDrawArrays(GL_TRIANGLES, 0, 6);

/*	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2); */

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
