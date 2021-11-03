#include "Shader.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

void Shader::addShader(const char* file_path, GLenum shader_type)
{
	if (shader_type != GL_VERTEX_SHADER && shader_type != GL_FRAGMENT_SHADER) {
		throw std::runtime_error("Invalid shader_type passed to file: " + *file_path + '\n');

		exit(-1);
	}

	GLuint shaderId = glCreateShader(shader_type);

	std::string shaderCode;
	std::ifstream shaderStream(file_path, std::ios::in);

	if (shaderStream.is_open()) {
		std::stringstream sstr;
		sstr << shaderStream.rdbuf();
		shaderCode = sstr.str();
		shaderStream.close();
	}
	else {
		throw std::runtime_error("Impossible to open file: " + *file_path + '\n');
		exit(-1);
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	std::cout << "Compiling shader: " << file_path << std::endl;
	char const* sourcePointer = shaderCode.c_str();
	glShaderSource(shaderId, 1, &sourcePointer, nullptr);
	glCompileShader(shaderId);

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> shaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(shaderId, infoLogLength, NULL, &shaderErrorMessage[0]);

		printf("%s\n", &shaderErrorMessage[0]);
	}

	shaders.push_back(shaderId);
}

void Shader::linkProgram()
{
	std::cout << "Linking program" << std::endl;
	
	programId = glCreateProgram();
	
	for (int shader : shaders) {
		glAttachShader(programId, shader);
	}

	glLinkProgram(programId);

	GLint result;
	int infoLogLength;

	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programId, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
	}

	for (int shader : shaders) {
		glDetachShader(programId, shader);
		glDeleteShader(shader);
	}
}

GLuint const Shader::getProgramId()
{
	return programId;
}

