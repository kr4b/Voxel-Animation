#pragma once

#include <chrono>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "state.hpp"

class Window {
public:
  Window(const int width, const int height);
  ~Window();

  bool update(State* state, int& frames);

  std::pair<int, int> get_size() const;

  inline double get_delta_time() const { return this->deltaTime; };

private:
  GLFWwindow* window;
  std::chrono::high_resolution_clock ::time_point prevUpdate, prevFrame;
  double deltaTime;
  int frames;
};