#pragma once

#include <GL/glew.h>

#include <vector>

class Shader {
public:
	void addShader(const char* file_path, GLenum shader_type);
	void linkProgram();

	GLuint const getProgramId();
private: 
	std::vector<GLuint> shaders;
	GLuint programId;
};