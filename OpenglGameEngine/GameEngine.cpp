#include "GameEngine.h"

#include <stdexcept>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

GameEngine::GameEngine()
{
	initGLFW();
	initWindow();
	initGLEW();
	initShaders();
}

GameEngine::~GameEngine()
{
	glDeleteProgram(shader.getProgramId());

	glfwDestroyWindow(window);

	glfwTerminate();
}

void GameEngine::runLoop()
{
	GLuint matrixId = glGetUniformLocation(shader.getProgramId(), "MVP");

	GLuint vertexArrayId;
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f,-1.0f, // triangle 2 : begin
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triangle 2 : end
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	GLuint vertexBuffer;

	glGenBuffers(1, &vertexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	float rotation = 0.0f;

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glUseProgram(shader.getProgramId());

		std::cout << rotation << std::endl;

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

		glm::mat4 view = glm::lookAt(
			glm::vec3(4, 3, 3),
			glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0)
		);

		glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 mvp = projection * view * model;

		glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

		rotation += 0.2f;
		if (rotation > 360.f) {
			rotation = 0.f;
		}

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			nullptr
		);

		glDrawArrays(GL_TRIANGLES, 0, 12*3);

		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexArrayId);
	glDeleteProgram(shader.getProgramId());
}

void GameEngine::initWindow()
{
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	window = glfwCreateWindow(1024, 768, "OpenGl Game Engine | C++", nullptr, nullptr);

	if (window == NULL) {
		throw std::runtime_error("Failed to open GLFW window\n");

		exit(-1);
	}

	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
}

void GameEngine::initGLFW()
{
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW\n");
		exit(-1);
	}
}

void GameEngine::initGLEW()
{
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initilize GLEW\n");

		exit(-1);
	}
}

void GameEngine::initShaders()
{
	shader = Shader{};

	shader.addShader("basic_shader.vertex", GL_VERTEX_SHADER);
	shader.addShader("basic_shader.fragment", GL_FRAGMENT_SHADER);

	shader.linkProgram();
}
