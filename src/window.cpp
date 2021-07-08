#include <iostream>

#include <glad/glad.h>

#include "defaults.hpp"
#include "window.hpp"

void error_callback(int error, const char* description) {
  printf("GLFW Error: %s\n", description);
}

Window::Window(const int width, const int height, State* state) {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  glfwSetErrorCallback(error_callback);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

  this->window = glfwCreateWindow(width, height, windowDefaults.name, NULL, NULL);
  if (!this->window) {
    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwMakeContextCurrent(this->window);
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize OpenGL context");
  }

  glfwSetWindowUserPointer(this->window, (void*) state);

  glfwSetKeyCallback(this->window, state::key_callback);
  glfwSetMouseButtonCallback(this->window, state::button_callback);
  glfwSetScrollCallback(this->window, state::scroll_callback);
  glfwSetCursorPosCallback(this->window, state::motion_callback);
}

Window::~Window() {
  glfwDestroyWindow(this->window);
  glfwTerminate();
}
