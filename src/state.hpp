#pragma once

#include <string>

#include <GLFW/glfw3.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include "defaults.hpp"

struct State {
  glm::quat cameraRot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3 cameraOff = glm::vec3(0.0f, 0.0f, -6.0f);

  glm::quat lastCameraRot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3 lastCameraOff = glm::vec3(0.0f, 0.0f, -3.0f);

  double lastX = 0.0;
  double lastY = 0.0;
  double prevX = 0.0;
  double prevY = 0.0;

  bool inControl = false;
  bool debugMode = false;
  bool refreshRayEmitter = true;
  bool displayRays = false;

  size_t rayCount = 1;
  float rayGap = 0.1f;

  static constexpr float scrollMult = 0.1f;
  static constexpr float motionRotMult = 0.00006f;
};

namespace state {
  void key_callback(GLFWwindow *win, int key, int, int act, int);
  void button_callback(GLFWwindow *win, int but, int act, int);
  void scroll_callback(GLFWwindow *win, double, double y);
  void motion_callback(GLFWwindow* win, double x, double y);
}