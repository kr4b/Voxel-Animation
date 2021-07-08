#pragma once

#include <chrono>

#include <GLFW/glfw3.h>

#include "state.hpp"

class Window {
public:
  Window(const int width, const int height, State* state);
  ~Window();

  bool update(const State& state);

  std::pair<int, int> get_size() const;

private:
  GLFWwindow* window;
  std::chrono::steady_clock::time_point prevTime;
  int frames;
};