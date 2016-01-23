#include "GLSLProgram.h"
#include "Errors.h"
#include <fstream>
#include <vector>
#include "IOManager.h"

GLSLProgram::GLSLProgram() :
	_numAttributes(0),
	_programID(0),
	_vertexShaderID(0),
	_fragmentShaderID(0)
{
}


GLSLProgram::~GLSLProgram()
{
}


void GLSLProgram::compileShaders(const std::string &vertexShaderFilepath, const std::string &fragmentShaderFilepath)
{

	std::string fragSource;
	std::string vertSource;

	IOManager::readFileToBuffer(vertexShaderFilepath, vertSource);
	IOManager::readFileToBuffer(fragmentShaderFilepath, fragSource);

	compileShadersFromSource(vertSource.c_str(), fragSource.c_str());
}

void GLSLProgram::compileShadersFromSource(const char* vertexSource, const char* fragmentSource)
{
	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0)
	{
		fatalError("Fragment shader failed to be created!");
	}

	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0)
	{
		fatalError("Vertex shader failed to be created!");
	}

	compileShader(vertexSource, " Vertex shader ", _vertexShaderID);
	compileShader(fragmentSource, " Fragment shader ", _fragmentShaderID);
}


void GLSLProgram::compileShader(const char* source, const std::string &identifier, GLuint shaderID)
{
	_programID = glCreateProgram();

	const char* contentsPtr = source;
	glShaderSource(shaderID, 1, &contentsPtr, nullptr);
	glCompileShader(shaderID);

	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

		glDeleteShader(shaderID);

		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shader " + identifier + " failed to compile!");
	}
}

void GLSLProgram::linkShaders()
{
	glAttachShader(_programID, _vertexShaderID);
	glAttachObjectARB(_programID, _fragmentShaderID);

	glLinkProgram(_programID);

	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

		glDeleteProgram(_programID);

		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shaders failed to link!");
	}

	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

void GLSLProgram::addAttribute(const std::string &attributeName)
{
	glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
}

void GLSLProgram::use()
{
	glUseProgram(_programID);
	for (int i = 0; i < _numAttributes; i++)
	{
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::unuse()
{
	glUseProgram(0);
	for (int i = 0; i < _numAttributes; i++)
	{
		glDisableVertexAttribArray(i);
	}
}

GLint GLSLProgram::getUniformLocation(const std::string& uniformName)
{
    GLuint location = glGetUniformLocation(_programID, uniformName.c_str());

	if (location == GL_INVALID_INDEX)
	{
		fatalError("Uniform " + uniformName + " not found in shader!");
	}

	return location;
}

void GLSLProgram::dispose()
{
	if (_programID)
		glDeleteProgram(_programID);
}

