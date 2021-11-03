#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

class GameEngine {
public:
	GameEngine();
	~GameEngine();

	void runLoop();
private:
	GLFWwindow* window;
	Shader shader;

	void initWindow();
	void initGLFW();
	void initGLEW();
	void initShaders();
};