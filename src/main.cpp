#include "defaults.hpp"
#include "window.hpp"

#include "scene/scenes.hpp"

#include <glm/vec3.hpp>

int main() {
    State state;
    Window window(1280, 720, &state);
    Cube scene(window, state);

    while (window.update(state)) {
        scene.render();
    }
  
    return 0;
}
