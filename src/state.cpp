#include "state.hpp"
#include <imgui.h>

void state::key_callback(GLFWwindow *win, int key, int, int act, int) {
  if (auto state = reinterpret_cast<State*>(glfwGetWindowUserPointer(win))) {
    if (act != GLFW_PRESS) {
      return;
    }

    switch (key) {
    // Escape => exit
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(win, GLFW_TRUE);
      break;
    // Space => create new debug spline
    case GLFW_KEY_SPACE:
      state->refreshRayEmitter = true;
      state->lastCameraRot = state->cameraRot;
      state->lastCameraOff = state->cameraOff;
      state->lastX = state->prevX;
      state->lastY = state->prevY;
      break;
    // Tab => toggle debug mode
    case GLFW_KEY_TAB: {
      state->debugMode = !state->debugMode;
      break;
    }
    // Backspace => return to camera transformation when spline was created
    case GLFW_KEY_BACKSPACE:
      state->cameraRot = state->lastCameraRot;
      state->cameraOff = state->lastCameraOff;
      break;
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
    state->cameraOff *= dist;
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
        state->cameraRot =
            glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), float(glm::degrees(state->motionRotMult * deltaX)), glm::vec3(0.0f, 1.0f, 0.0f)) *
            glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), float(glm::degrees(state->motionRotMult * deltaY)), glm::vec3(1.0f, 0.0f, 0.0f)) *
            state->cameraRot;
      }
    }
  }
}
