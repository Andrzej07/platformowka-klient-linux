#pragma once
#include <GL/glew.h>
#include "Vertex.h"
#include <glm/glm.hpp>
#include <vector>

// THIS CLASS ALLOWS US TO MAKE LESS DRAW CALLS SPEEDING UP THE DRAWING


enum GlyphSortType { NONE, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE};

class Glyph
{
public:
	Glyph() {}
	Glyph(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint Texture, float Depth, const ColorRGBA8 &color);
    Glyph(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint Texture, float Depth, const ColorRGBA8 &color, float angle);
	GLuint texture;
	float depth;

	Vertex topLeft;
	Vertex topRight;
	Vertex bottomRight;
	Vertex bottomLeft;
private:
	glm::vec2 rotatePoint(glm::vec2, float angle);
};

struct RenderBatch
{
	RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : 
		offset(Offset), 
		numVertices(NumVertices), 
		texture(Texture) {}
	GLuint offset;
	GLuint numVertices;
	GLuint texture;
};

class SpriteBatch
{
public:
	SpriteBatch();
	~SpriteBatch();

	void init();

	void begin(GlyphSortType sortType = TEXTURE);
	void end();

	void draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color);
	void draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color, float angle);
	void draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const ColorRGBA8 &color, const glm::vec2 &dir);

	void renderBatch();
	void createRenderBatches();

private:
	void createVertexArray();
	void sortGlyphs();

	static bool compareFrontToBack(Glyph*, Glyph*);
	static bool compareBackToFront(Glyph*, Glyph*);
	static bool compareTexture(Glyph*, Glyph*);

	GlyphSortType _sortType;
	GLuint _vbo;
	GLuint _vao;

	std::vector<Glyph*> _glyphPointers;  // for sorting
	std::vector<Glyph> _glyphs;  // acrutal glyphs
	std::vector<RenderBatch> _renderBatches;
};

