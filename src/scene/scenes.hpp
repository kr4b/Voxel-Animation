#pragma once

#include "spline_map_scene.hpp"

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

static Volume create_volume(scenes::Scene scene) {
    switch (scene) {
        case scenes::Pig:
            return load_fld_volume("assets/pig8.fld");
        case scenes::Bonsai:
            return load_fld_volume("assets/bonsai.fld");
        case scenes::Carp:
            return load_fld_volume("assets/carp8.fld");
        case scenes::Orange:
            return load_fld_volume("assets/orange.fld");
        case scenes::Stent:
            return load_fld_volume("assets/stent8.fld");
        case scenes::Tomato:
            return load_fld_volume("assets/tomato.fld");
        case scenes::Tooth:
            return load_fld_volume("assets/tooth.fld");
        case scenes::Bunny:
            return load_mhd_volume("assets/bunny.mhd");
        case scenes::Cube:
        default:
            return load_cube();
    }
}

static SplineMapScene* create_scene(scenes::Scene scene, Window& window, Setup& setup) {
    return new SplineMapScene(window, setup, create_volume(scene));
}
