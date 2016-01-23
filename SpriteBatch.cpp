#include "SpriteBatch.h"
#include <algorithm>


Glyph::Glyph(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint Texture, float Depth, const ColorRGBA8 &color)
{
	texture = Texture;
	depth = Depth;

	topLeft.color = color;
	topLeft.setPosition(destRect.x, destRect.y + destRect.w);  // uvRect[3]
	topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

	bottomLeft.color = color;
	bottomLeft.setPosition(destRect.x, destRect.y);
	bottomLeft.setUV(uvRect.x, uvRect.y);

	bottomRight.color = color;
	bottomRight.setPosition(destRect.x + destRect.z, destRect.y);  // uvRect[2]
	bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	topRight.color = color;
	topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
	topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
}

Glyph::Glyph(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint Texture, float Depth, const ColorRGBA8 &color, float angle)
{
	texture = Texture;
	depth = Depth;

	glm::vec2 halfDims(destRect.z / 2, destRect.w / 2);
	//Get points centered at origin
	glm::vec2 tl(-halfDims.x, halfDims.y);
	glm::vec2 bl(-halfDims.x, -halfDims.y);
	glm::vec2 tr(halfDims.x, halfDims.y);
	glm::vec2 br(halfDims.x, -halfDims.y);

	//Rotate them
	tl = rotatePoint(tl, angle) + halfDims;
	br = rotatePoint(br, angle) + halfDims;
	bl = rotatePoint(bl, angle) + halfDims;
	tr = rotatePoint(tr, angle) + halfDims;

	topLeft.color = color;
	topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y);  // uvRect[3]
	topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

	bottomLeft.color = color;
	bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y);
	bottomLeft.setUV(uvRect.x, uvRect.y);

	bottomRight.color = color;
	bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y);  // uvRect[2]
	bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

	topRight.color = color;
	topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y);
	topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
}
glm::vec2 Glyph::rotatePoint(glm::vec2 pos, float angle)
{
	glm::vec2 newVec;
	newVec.x = pos.x * cos(angle) - pos.y * sin(angle);
	newVec.y = pos.x * sin(angle) + pos.y * cos(angle);
	return newVec;
}

SpriteBatch::SpriteBatch():
	_vbo(0),
	_vao(0)
{
}


SpriteBatch::~SpriteBatch()
{
}

void SpriteBatch::draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color)
{
	_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
}
void SpriteBatch::draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color, float angle)
{
	_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);

}
void SpriteBatch::draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color, const glm::vec2 &dir)
{
	const glm::vec2 right(1, 0);
	float angle = acos(glm::dot(right, dir));
	if (dir.y < 0) angle = -angle;
	_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);

}
void SpriteBatch::init()
{
	createVertexArray();
}

void SpriteBatch::begin(GlyphSortType sortType /*default:TEXTURE*/)
{
	_sortType = sortType;
	_renderBatches.clear();
	_glyphs.clear();
}

void SpriteBatch::end()
{
	_glyphPointers.resize(_glyphs.size());
	for (unsigned int i = 0; i < _glyphs.size(); ++i)
	{
		_glyphPointers[i] = &_glyphs[i];
	}
	sortGlyphs();
	createRenderBatches();
}

void SpriteBatch::createRenderBatches()
{
	if (_glyphs.empty()) return;
	std::vector<Vertex> vertices(_glyphPointers.size() * 6);

	int offset = 0;
	int cv = 0; //currentVertex

	_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture);
	vertices[cv++] = _glyphPointers[0]->topLeft;
	vertices[cv++] = _glyphPointers[0]->bottomLeft;
	vertices[cv++] = _glyphPointers[0]->bottomRight;
	vertices[cv++] = _glyphPointers[0]->bottomRight;
	vertices[cv++] = _glyphPointers[0]->topRight;
	vertices[cv++] = _glyphPointers[0]->topLeft;
	offset += 6;

	for (unsigned int cg = 1; cg < _glyphPointers.size(); ++cg)//currentGlyph
	{
		if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture)
			_renderBatches.emplace_back(offset, 6, _glyphPointers[cg]->texture);
		else
			_renderBatches.back().numVertices += 6;
		vertices[cv++] = _glyphPointers[cg]->topLeft;
		vertices[cv++] = _glyphPointers[cg]->bottomLeft;
		vertices[cv++] = _glyphPointers[cg]->bottomRight;
		vertices[cv++] = _glyphPointers[cg]->bottomRight;
		vertices[cv++] = _glyphPointers[cg]->topRight;
		vertices[cv++] = _glyphPointers[cg]->topLeft;
		offset += 6;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	//orphan the buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	//upload the data
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void SpriteBatch::renderBatch()
{
	glBindVertexArray(_vao);
	for (auto &batch : _renderBatches)
	{
		glBindTexture(GL_TEXTURE_2D, batch.texture);
		glDrawArrays(GL_TRIANGLES, batch.offset, batch.numVertices);
	}
	glBindVertexArray(0);
}

void SpriteBatch::createVertexArray()
{
	if (!_vao)	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao); // remembers everything we do below so we can do it faster later

	if(!_vbo) glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2); 

	//Position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	//ColorRGBA8 attribute pointer, normalize 255 to 1
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	//UV attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glBindVertexArray(0);  // unbinds all the stuff above
		
}

bool SpriteBatch::compareFrontToBack(Glyph* A , Glyph* B)
{
	return(A->depth < B->depth);
}
bool SpriteBatch::compareBackToFront(Glyph* A, Glyph* B)
{
	return(A->depth > B->depth);
}
bool SpriteBatch::compareTexture(Glyph* A, Glyph* B)
{
	return(A->texture < B->texture);
}
void SpriteBatch::sortGlyphs()
{
	switch (_sortType)
	{
	case BACK_TO_FRONT:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
		break;
	case FRONT_TO_BACK:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
		break;
	case TEXTURE:
		std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
		break;
    case NONE:
        break;
	}
}

