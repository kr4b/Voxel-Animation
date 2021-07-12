#include "defaults.hpp"
#include "window.hpp"

#include "scene/scenes.hpp"
#include "scene/debug_scene.hpp"

#include <glm/vec3.hpp>

int main() {
    State state;
    Window window(1280, 720, &state);
    Pig scene(window, state);

    while (window.update(state)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.update();
        scene.render();
    }
  
    return 0;
}
