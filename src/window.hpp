#pragma once

#include <GLFW/glfw3.h>

#include "state.hpp"

class Window {
public:
  Window(const int width, const int height, State* state);
  ~Window();

  inline bool is_open() const { return !glfwWindowShouldClose(this->window); }
private:
  GLFWwindow* window;
};