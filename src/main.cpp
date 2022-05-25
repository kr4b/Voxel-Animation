#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "defaults.hpp"
#include "window.hpp"

#include "scene/scenes.hpp"
#include "scene/debug_scene.hpp"

void showBenchmark(State* state, int& frames) {
    if (frames >= 0) {
        state->nextScenario(frames);
        frames = -1;
    }

    ImGui::Begin("Benchmark");
    if (ImGui::Button("Start benchmark")) {
        state->startBenchmark();
    }

    ImGui::Separator();
    ImGui::Text("Average FPS:");

    if (state->averageFps < 0) {
        ImGui::Text("~");
    } else {
        char fps[10];
        std::snprintf(fps, sizeof(fps), "%.2f", state->averageFps);
        ImGui::Text(fps);
    }

    ImGui::End();
}

int main() {
    State* state = NULL;
    Window window(1280, 720);
    Setup setup;
    SplineMapScene* scenePointers[scenes::len] = { NULL };
    int index = scenes::Cube;
    int frames = -1;

    while (window.update(state, frames)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Initialize scene
        if (scenePointers[index] == NULL) {
            scenePointers[index] = create_scene((scenes::Scene) index, window, setup);
        }

        state = &scenePointers[index]->get_state();

        scenePointers[index]->update();
        scenePointers[index]->render();

        // Scene selection
        ImGui::Begin("Scene");
        ImGui::ListBox("Scenes", &index, scenes::names, scenes::len);
        ImGui::End();

        showBenchmark(state, frames);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    for (SplineMapScene* scene : scenePointers) {
        if (scene) {
            delete scene;
        }
    }
  
    return 0;
}
