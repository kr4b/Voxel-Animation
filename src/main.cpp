#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "defaults.hpp"
#include "window.hpp"

#include "scene/scenes.hpp"
#include "scene/debug_scene.hpp"

int main() {
    State* state = NULL;
    Window window(1280, 720);
    Setup setup;
    SplineMapScene* scenePointers[scenes::len] = { NULL };
    int index = scenes::Cube;

    while (window.update(state)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (scenePointers[index] == NULL) {
            scenePointers[index] = create_scene((scenes::Scene) index, window, setup);
        }

        state = &scenePointers[index]->get_state();

        scenePointers[index]->update();
        scenePointers[index]->render();

        ImGui::Begin("Scene");
        ImGui::ListBox("Scenes", &index, scenes::names, scenes::len);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
  
    return 0;
}
