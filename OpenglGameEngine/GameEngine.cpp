#include "GameEngine.h"

#include <stdexcept>

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
	GLuint vertexArrayId;
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	static const GLfloat g_vertex_buffer_data[] = {
	   -1.0f, -1.0f, 0.0f,
	   1.0f, -1.0f, 0.0f,
	   0.0f,  1.0f, 0.0f,
	};

	GLuint vertexBuffer;

	glGenBuffers(1, &vertexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader.getProgramId());

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

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDrawArrays(GL_TRIANGLES, 0, 3);
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
