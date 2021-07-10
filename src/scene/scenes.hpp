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
            glm::vec3(0.0f, 2.0f, 0.0f),
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
