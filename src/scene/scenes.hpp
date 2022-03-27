#pragma once

#include "spline_map_scene.hpp"

class Cube : public SplineMapScene {
public:
    Cube(Window &window, Setup& setup) :
    SplineMapScene(window, setup, load_cube()) {};
};

class Pig : public SplineMapScene {
public:
    Pig(Window &window, Setup& setup) :
    SplineMapScene(
        window,
        setup,
        load_fld_volume("assets/pig8.fld"))
    {};
};

class Bonsai : public SplineMapScene {
public:
    Bonsai(Window &window, Setup& setup) :
    SplineMapScene(
        window,
        setup,
        load_fld_volume("assets/bonsai.fld"))
    {};
};

class Carp : public SplineMapScene {
public:
    Carp(Window &window, Setup& setup) :
    SplineMapScene(
        window,
        setup,
        load_fld_volume("assets/carp8.fld"))
    {};
};

class Orange : public SplineMapScene {
public:
    Orange(Window &window, Setup& setup) :
    SplineMapScene(
        window,
        setup,
        load_fld_volume("assets/orange.fld"))
    {};
};

class Stent : public SplineMapScene {
public:
    Stent(Window &window, Setup& setup) :
    SplineMapScene(
        window,
        setup,
        load_fld_volume("assets/stent8.fld"))
    {};
};

class Tomato : public SplineMapScene {
public:
    Tomato(Window &window, Setup& setup) :
    SplineMapScene(
        window,
        setup,
        load_fld_volume("assets/tomato.fld"))
    {};
};

class Tooth : public SplineMapScene {
public:
    Tooth(Window &window, Setup& setup) :
    SplineMapScene(
        window,
        setup,
        load_fld_volume("assets/tooth.fld"))
    {};
};

class Bunny : public SplineMapScene {
public:
    Bunny(Window &window, Setup& setup) :
    SplineMapScene(
        window,
        setup,
        load_mhd_volume("assets/bunny.mhd"))
    {};
};

namespace scenes {
    enum Scene {
        Cube,
        Pig,
        Bonsai,
        Carp,
        Orange,
        Stent,
        Tomato,
        Tooth,
        Bunny
    };

    static const char* const names[] = {
        "cube",
        "pig",
        "bonsai",
        "carp",
        "orange",
        "stent",
        "tomato",
        "tooth",
        "bunny",
    };

    static const int len = sizeof(names) / sizeof(names[0]);
}

static SplineMapScene* create_scene(scenes::Scene scene, Window& window, Setup& setup) {
    switch (scene) {
        case scenes::Pig:
            return new Pig(window, setup);
            break;
        case scenes::Bonsai:
            return new Bonsai(window, setup);
            break;
        case scenes::Carp:
            return new Carp(window, setup);
            break;
        case scenes::Orange:
            return new Orange(window, setup);
            break;
        case scenes::Stent:
            return new Stent(window, setup);
            break;
        case scenes::Tomato:
            return new Tomato(window, setup);
            break;
        case scenes::Tooth:
            return new Tooth(window, setup);
            break;
        case scenes::Bunny:
            return new Bunny(window, setup);
            break;
        case scenes::Cube:
        default:
            return new Cube(window, setup);
    }
}
