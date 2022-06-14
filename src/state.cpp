#include "state.hpp"
#include <imgui.h>

#include <glm/gtx/quaternion.hpp>

const static glm::vec3 scenarios[] = {
  glm::vec3(4.1f, 2.1f, 4.5f),
  glm::vec3(3.5f, -3.9f, -3.3f),
  glm::vec3(-4.9f, -2.9f, 4.5f),
  glm::vec3(1.9f, 4.8f, 2.2f),
  glm::vec3(2.8f, -3.1f, 0.65f),
  glm::vec3(2.5f, 1.9f, 4.9f),
  glm::vec3(-4.2f, -3.3f, -0.91f),
  glm::vec3(5.7f, -2.2f, 0.085f),
  glm::vec3(-0.81f, -5.1f, -0.99f),
  glm::vec3(-4.8f, -1.4f, 4.6f),
  glm::vec3(2.8f, 4.2f, -1.1f),
  glm::vec3(-5.4f, 4.2f, 4.0f),
  glm::vec3(4.1f, 2.5f, -1.4f),
  glm::vec3(4.7f, 0.78f, 0.14f)
};

void State::updateCamera() {
  this->cameraRotation = glm::conjugate(glm::quatLookAt(glm::normalize(-scenarios[this->scenarioCount]), glm::vec3(0.0f, 1.0f, 0.0f)));
  this->cameraOffset = -scenarios[this->scenarioCount];
}

void State::startBenchmark() {
  this->scenarioCount = 0;
  this->averageFps = -1.0;
  this->skip = true;

  this->updateCamera();
}

void State::nextScenario(int frames) {
  if (scenarioCount == -1 || this->skip) {
    this->skip = false;
    return;
  }

  this->averageFps = this->averageFps * this->scenarioCount + double(frames);
  this->scenarioCount += 1;
  this->averageFps /= this->scenarioCount;

  if (this->scenarioCount >= sizeof(scenarios) / sizeof(scenarios[0])) {
    this->scenarioCount = -1;
    return;
  }

  this->updateCamera();
}

void State::translateCamera(glm::vec3 translation) {
  this->cameraTranslation += translation;
}

void State::update(double deltaTime) {
  this->cameraOffset += glm::inverse(glm::toMat3(this->cameraRotation)) * (float(deltaTime) * this->cameraVelocity);
}

void key_press(GLFWwindow *win, State* state, int key) {
  switch (key) {
    // Escape => exit
    case GLFW_KEY_ESCAPE: {
      glfwSetWindowShouldClose(win, GLFW_TRUE);
      break;
    }
    // Space => create new debug rays
    case GLFW_KEY_SPACE: {
      state->refreshRayEmitter = true;
      state->lastCameraRotation = state->cameraRotation;
      state->lastCameraOffset = state->cameraOffset;
      state->lastX = state->prevX;
      state->lastY = state->prevY;
      break;
    }
    // Tab => toggle debug mode
    case GLFW_KEY_TAB: {
      state->debugMode = !state->debugMode;
      break;
    }
    // Backspace => return to camera transformation when debug rays were created
    case GLFW_KEY_BACKSPACE: {
      break;
      state->cameraRotation = state->lastCameraRotation;
      state->cameraOffset = state->lastCameraOffset;
      break;
    }
    // W => move forward
    case GLFW_KEY_W: {
      state->cameraVelocity.z = state->cameraSpeed;
      break;
    }
    // S => move backward
    case GLFW_KEY_S: {
      state->cameraVelocity.z = -state->cameraSpeed;
      break;
    }
    // A => move left
    case GLFW_KEY_A: {
      state->cameraVelocity.x = state->cameraSpeed;
      break;
    }
    // D => move right
    case GLFW_KEY_D: {
      state->cameraVelocity.x = -state->cameraSpeed;
      break;
    }
  }
}

void key_release(GLFWwindow *win, State* state, int key) {
  switch (key) {
    // W,S => stop moving forward,backward
    case GLFW_KEY_W:
    case GLFW_KEY_S: {
      state->cameraVelocity.z = 0.0f;
      break;
    }
    // A,D => stop moving left,right
    case GLFW_KEY_A:
    case GLFW_KEY_D: {
      state->cameraVelocity.x = 0.0f;
      break;
    }
  }
}

void state::key_callback(GLFWwindow *win, int key, int, int act, int) {
  if (auto state = reinterpret_cast<State*>(glfwGetWindowUserPointer(win))) {
    if (act == GLFW_PRESS) {
      key_press(win, state, key);
    } else if (act == GLFW_RELEASE) {
      key_release(win, state, key);
    }

    return;
  }
}

void state::button_callback(GLFWwindow *win, int but, int act, int) {
  if (ImGui::GetIO().WantCaptureMouse) return;
  if (auto state = reinterpret_cast<State*>(glfwGetWindowUserPointer(win))) {
    if (GLFW_MOUSE_BUTTON_LEFT == but) {
      if ((state->inControl = (GLFW_PRESS == act)))
        glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      else
        glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }
}

void state::scroll_callback(GLFWwindow *win, double, double y) {
  if (ImGui::GetIO().WantCaptureMouse) return;
  if (auto state = reinterpret_cast<State*>(glfwGetWindowUserPointer(win))) {
    auto dist = 1.f - state->scrollMult * float(y);
    state->cameraOffset *= dist;
  }
}

void state::motion_callback(GLFWwindow* win, double x, double y) {
  if (ImGui::GetIO().WantCaptureMouse) return;
  if (auto state = reinterpret_cast<State*>(glfwGetWindowUserPointer(win))) {
    auto const prevX = state->prevX;
    auto const prevY = state->prevY;
    state->prevX = x;
    state->prevY = y;
    if (!std::isnan(prevX)) {
      auto const deltaX = x - prevX;
      auto const deltaY = y - prevY;

      if (state->inControl) {
        state->yaw += float(state->motionRotMult * deltaX);
        state->pitch += float(state->motionRotMult * deltaY);
        state->cameraRotation = glm::angleAxis(state->pitch, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::angleAxis(state->yaw, glm::vec3(0.0f, 1.0f, 0.0f));
      }
    }
  }
}
