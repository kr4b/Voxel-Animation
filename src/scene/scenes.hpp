#pragma once

#include "spline_map_scene.hpp"

class Cube : public SplineMapScene {
public:
    Cube(Window &window, State &state) :
    SplineMapScene(window, state),
    base(Plane(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 1.0f))),
    spline(Spline::with_tangents(
        glm::vec3(0.0f),
        glm::vec3(0.0f, 2.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)))
    {};

protected:
    Plane  base;
    Spline spline;
};


class Pig : public SplineMapScene {
public:
    Pig(Window &window, State &state) :
    SplineMapScene(window, state),
    volume(load_fld_volume("assets/pig8.fld", FLDInfo { 3, { 256, 256, 180 }, 1, 1})),
    base(Plane(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 1.0f))),
    spline(Spline::with_tangents(
        glm::vec3(0.0f),
        glm::vec3(0.0f, 2.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)))
    {};

protected:
    Volume volume;
    Plane  base;
    Spline spline;
};
