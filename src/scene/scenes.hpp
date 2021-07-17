#pragma once

#include "spline_map_scene.hpp"

class Cube : public SplineMapScene {
public:
    Cube(Window &window, State &state) :
    SplineMapScene(
        window,
        state,
        load_cube(),
        Plane(
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 1.0f)),
        Spline::with_tangents(
            glm::vec3(0.0f),
            glm::vec3(0.0f, 2.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f)))
    {};
};


class Pig : public SplineMapScene {
public:
    Pig(Window &window, State &state) :
    SplineMapScene(
        window,
        state,
        load_fld_volume("assets/pig8.fld", FLDInfo { 3, { 256, 256, 180 }, 1, 1}),
        Plane(
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 1.0f)),
        Spline::with_tangents(
            glm::vec3(0.0f),
            glm::vec3(0.0f, 2.0f, 1.0f),
            glm::vec3(5.0f, 3.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -2.0f)))
    {};
};

class Bonsai : public SplineMapScene {
public:
    Bonsai(Window &window, State &state) :
    SplineMapScene(
        window,
        state,
        load_fld_volume("assets/bonsai.fld", FLDInfo { 3, { 256, 256, 182 }, 1, 1}),
        Plane(
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 1.0f)),
        Spline::with_tangents(
            glm::vec3(0.0f),
            glm::vec3(0.0f, 2.0f, 1.0f),
            glm::vec3(5.0f, 3.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -2.0f)))
    {};
};

namespace scenes {
    enum Scene {
        Cube,
        Pig,
        Bonsai
    };

    static const char* const names[] = {
        "cube",
        "pig",
        "bonsai",
    };

    static const int len = sizeof(names) / sizeof(names[0]);
}

static SplineMapScene* create_scene(scenes::Scene scene, Window& window, State& state) {
    switch (scene) {
        case scenes::Cube:
            return new Cube(window, state);
            break;
        case scenes::Pig:
            return new Pig(window, state);
            break;
        case scenes::Bonsai:
            return new Bonsai(window, state);
            break;
    }
}
