#pragma once

#include <string>

#include <GLFW/glfw3.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include "defaults.hpp"

// State for mostly keeping track of camera transformation
struct State {
  glm::quat cameraRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3 cameraOffset = glm::vec3(0.0f, 0.0f, -6.0f);
  glm::vec3 cameraTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 cameraVelocity = glm::vec3(0.0f);

  glm::quat lastCameraRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3 lastCameraOffset = glm::vec3(0.0f, 0.0f, -6.0f);
  glm::vec3 lastCameraTranslation = glm::vec3(0.0f, 0.0f, 0.0f);

  double lastX = 0.0;
  double lastY = 0.0;
  double prevX = 0.0;
  double prevY = 0.0;

  float pitch = 0.0f;
  float yaw = 0.0f;

  bool inControl = false;
  bool debugMode = false;
  bool showUi = true;
  bool refreshRayEmitter = true;

  static constexpr float scrollMult = 0.1f;
  static constexpr float cameraSpeed = 0.01f;
  static constexpr float motionRotMult = 0.001f;

  bool skip = false;
  double averageFps = -1.0;
  int scenarioCount = -1;

  void startBenchmark();
  void nextScenario(int frames);

  void translateCamera(glm::vec3 translation);

  void update(double deltaTime);
private:
  void updateCamera();
};

namespace state {
  void key_callback(GLFWwindow *win, int key, int, int act, int);
  void button_callback(GLFWwindow *win, int but, int act, int);
  void scroll_callback(GLFWwindow *win, double, double y);
  void motion_callback(GLFWwindow* win, double x, double y);
}