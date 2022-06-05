#pragma once

#include <imgui.h>

#include <glm/vec3.hpp>

#include <vector>

#include "scenes.hpp"
#include "spline_map_scene.hpp"

class SceneCollection {
private:
    std::vector<SplineMapScene*> scenes;
    int offset = 0;

public:
    SceneCollection(Window& window, Setup& setup) {
        SplineMapScene* scene1 = create_scene(scenes::Seaweed, window, setup);
        SplineMapScene* scene2 = create_scene(scenes::Carp, window, setup);
        SplineMapScene* scene3 = create_scene_from_loaded(scene1, window, setup);
        scene1->translate(glm::vec3(1.0f, 1.0f, -2.0f));
        scene2->translate(glm::vec3(0.0f, -1.0f, 0.0f));
        scene3->translate(glm::vec3(-1.0f, 1.0f, 3.0f));
        scenes.push_back(scene1);
        scenes.push_back(scene2);
        scenes.push_back(scene3);
    }

    ~SceneCollection() {
        for (SplineMapScene* scene : scenes) {
            delete scene;
        }
    }

    void update_and_render() {
        ImGui::Begin("Collection");
        ImGui::SliderInt("Offset", &this->offset, 0, scenes.size() - 1);
        ImGui::End();

        bool showUi = true;
        for (int i = 0; i < scenes.size(); i++) {
            SplineMapScene* scene = scenes[(i + offset) % scenes.size()];
            scene->set_state(get_state());
            scene->get_state().showUi = showUi;
            scene->update();
            scene->render();
            showUi = false;
        }
    }

    State& get_state() {
        return scenes[0]->get_state();
    }
};
