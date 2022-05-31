#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "defaults.hpp"
#include "window.hpp"

#include "scene/scenes.hpp"
#include "scene/debug_scene.hpp"
#include "scene/scene_collection.hpp"

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
        ImGui::Text("%.2f", state->averageFps);
    }

    ImGui::End();
}

int main() {
    State* state = NULL;
    Window window(1280, 720);
    Setup setup;
    SceneCollection collection(window, setup);
    SplineMapScene* scenePointers[scenes::len] = { NULL };
    int index = scenes::Cube;
    int mode = 0;
    int frames = -1;
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    while (window.update(state, frames)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Initialize scene
        if (scenePointers[index] == NULL) {
            scenePointers[index] = create_scene((scenes::Scene) index, window, setup);
        }

        if (mode == 0) {
            state = &collection.get_state();
            collection.update_and_render();
        } else {
            state = &scenePointers[index]->get_state();
            scenePointers[index]->update();
            scenePointers[index]->render();
        }

        // Scene selection
        ImGui::Begin("Scene");
        const char* const names[] = { "Collection", "Individual" };
        ImGui::ListBox("Mode", &mode, names, 2);
        if (mode == 1) {
            ImGui::ListBox("Scenes", &index, scenes::names, scenes::len);
        }
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
