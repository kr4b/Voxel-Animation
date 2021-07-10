#include <iostream>

#include "defaults.hpp"
#include "window.hpp"

void error_callback(int error, const char* description) {
  printf("GLFW Error: %s\n", description);
}

Window::Window(const int width, const int height, State* state) : frames(0) {
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

  glfwSwapInterval(1);

  this->prevTime = std::chrono::high_resolution_clock::now();
}

Window::~Window() {
  glfwDestroyWindow(this->window);
  glfwTerminate();
}

bool Window::update(const State& state) {
  this->frames += 1;
  auto now = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> timePassed = now - this->prevTime;
  
  if (int(timePassed.count()) > 1000) {
    char newTitle[sizeof(windowDefaults.name) + 32];
    snprintf(newTitle, sizeof(newTitle), "%s%s - %d FPS", windowDefaults.name, state.debugMode ? " - Debug" : "", frames);
    glfwSetWindowTitle(this->window, newTitle);
    this->prevTime = now;
    this->frames = 0;
  }
  glfwSwapBuffers(this->window);
  glfwPollEvents();
  return !glfwWindowShouldClose(this->window);
}

std::pair<int, int> Window::get_size() const {
  int width, height;
  glfwGetFramebufferSize(this->window, &width, &height);
  return std::make_pair(width, height);
}