#pragma once
#include <string>
#include <GL/glew.h>

class GLSLProgram
{
public:
	GLSLProgram();
	~GLSLProgram();

	void compileShaders(const std::string &vertexShaderFilepath, const std::string &fragmentShaderFilepath);
	void compileShadersFromSource(const char* vertexSource, const char* fragmentSource);

	void linkShaders();
	void addAttribute(const std::string &attributeName);
	void use();
	void unuse();
	void dispose();

	GLint getUniformLocation(const std::string& uniformName);

private:
	void compileShader(const char* source, const std::string &identifier, GLuint shaderID);

	int _numAttributes;
	GLuint _programID;
	GLuint _vertexShaderID;
	GLuint _fragmentShaderID;
};

