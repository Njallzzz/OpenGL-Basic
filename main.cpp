
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "window.h"
#include "shader.h"
#include <iostream>

using std::cout;
using std::endl;

Window *window;

void error_callback(int error, const char* description) {
    puts(description);
}

static bool wireframe = false;

void key_callback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		window->showMetrics(!window->MetricsStatus());
	} else if(key == GLFW_KEY_M) {
	} else if(key == GLFW_KEY_F && action == GLFW_PRESS) {
		if(wireframe) {
			cout << "Raserization mode changed to normal" << endl;
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			wireframe = false;
		} else {
			cout << "Raserization mode changed to wireframe" << endl;
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			wireframe = true;
		}
	} else if(key == GLFW_KEY_F) {
	} else {
    	cout << "Key: " << key << " Scancode: " << scancode << " Action: " << action << " mods: " << mods << endl;
	}
}

int
main(void) {
	wireframe = false;
	glfwSetErrorCallback(error_callback);
	int width = 1536, height = 864;
	window = new Window(&std::cout, width, height, "OpenGL Window", 120, 8);
	int res = window->init();
	if(res) {
		return res;
	}
	res = gl3wInit();
	if (res) {
		std::cout << "Fatal error, unable to initalize gl3w" << std::endl;
		return res;
	}

	GLFWwindow *pWindow = window->getWindowPointer();
	glfwSetKeyCallback(pWindow, key_callback);

	Shader shader("Assets/simple.vert", "Assets/simple.frag");
	shader.compile(&std::cout);
	shader.use();

	GLuint vao, vbo;
	float vertices[] =   {	 0.0f,  0.5f,
							-0.5f, -0.5f,
							 0.5f, -0.5f	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(shader.getVertexPointer());
	glVertexAttribPointer(shader.getVertexPointer(), 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);
	while(window->checkClosed()) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		window->swapBuffers();
		glfwPollEvents();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	return 0;
}