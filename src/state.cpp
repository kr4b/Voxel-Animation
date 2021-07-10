#include "state.hpp"

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
      state->refreshSpline = true;
      state->lastCameraRot = state->cameraRot;
      state->lastCameraOff = state->cameraOff;
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
    // Up arrow => increase debug spline count
    case GLFW_KEY_UP:
      state->refreshSplineSource = true;
      state->splineCount += 1;
      break;
      // Up arrow => decrease debug spline count
    case GLFW_KEY_DOWN:
      state->refreshSplineSource = true;
      state->splineCount = glm::max(1, int(state->splineCount) - 1);
      break;
      // Up arrow => increase debug spline distance
    case GLFW_KEY_RIGHT:
      state->refreshSplineSource = true;
      state->splineDist = glm::min(1.0f, state->splineDist + 0.01f);
      break;
      // Up arrow => decrease debug spline distance
    case GLFW_KEY_LEFT:
      state->refreshSplineSource = true;
      state->splineDist = glm::max(0.01f, state->splineDist - 0.01f);
      break;

    case GLFW_KEY_R:
      state->displayRays = !state->displayRays;
      break;
    }

    return;
  }
}

void state::button_callback(GLFWwindow *win, int but, int act, int) {
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
  if (auto state = reinterpret_cast<State*>(glfwGetWindowUserPointer(win))) {
    auto dist = 1.f - state->scrollMult * float(y);
    state->cameraOff *= dist;
  }
}

void state::motion_callback(GLFWwindow* win, double x, double y) {
  if (auto state = reinterpret_cast<State*>(glfwGetWindowUserPointer(win))) {
    auto const lastX = state->lastX;
    auto const lastY = state->lastY;
    state->lastX = x;
    state->lastX = y;
    if (!std::isnan(lastX)) {
      auto const deltaX = x - lastX;
      auto const deltaY = y - lastY;

      if (state->inControl) {
        state->cameraRot =
            glm::quat(glm::degrees(state->motionRotMult * deltaX), 0.0f, 1.0f, 0.0f) *
            glm::quat(glm::degrees(state->motionRotMult * deltaY), 1.0f, 0.0f, 0.0f) * state->cameraRot;
      }
    }
  }
}