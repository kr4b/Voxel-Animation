#pragma once

#include <memory>
#include <utility>

#include <glm/vec3.hpp>

#include "spline_map_scene.hpp"

namespace scenes {
    enum Scene {
        Cube,
        Seaweed,
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
        "seaweed",
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

static std::pair<std::shared_ptr<Volume>, glm::ivec3> create_volume(scenes::Scene scene) {
    switch (scene) {
        case scenes::Pig: // Piggy Pink
            return std::make_pair(std::make_shared<Volume>(load_fld_volume("assets/pig8.fld")), glm::ivec3(253, 221, 230));
        case scenes::Bonsai: // Bark Brown
            return std::make_pair(std::make_shared<Volume>(load_fld_volume("assets/bonsai.fld")), glm::ivec3(102, 73, 58));
        case scenes::Carp: // Apricot Orange
            return std::make_pair(std::make_shared<Volume>(load_fld_volume("assets/carp8.fld")), glm::ivec3(253, 180, 130));
        case scenes::Orange: // Sweet Orange
            return std::make_pair(std::make_shared<Volume>(load_fld_volume("assets/orange.fld")), glm::ivec3(250, 141, 73));
        case scenes::Stent: // Dark Flesh
            return std::make_pair(std::make_shared<Volume>(load_fld_volume("assets/stent8.fld")), glm::ivec3(186, 141, 135));
        case scenes::Tomato: // Tomato
            return std::make_pair(std::make_shared<Volume>(load_fld_volume("assets/tomato.fld")), glm::ivec3(206, 41, 57));
        case scenes::Tooth: // Neutral Pearl White
            return std::make_pair(std::make_shared<Volume>(load_fld_volume("assets/tooth.fld")), glm::ivec3(242, 238, 231));
        case scenes::Bunny: // White Fur
            return std::make_pair(std::make_shared<Volume>(load_mhd_volume("assets/bunny.mhd")), glm::ivec3(244, 241, 240));
        case scenes::Seaweed: // Nori Seaweed Green
            return std::make_pair(std::make_shared<Volume>(load_seaweed()), glm::ivec3(70, 72, 38));
        case scenes::Cube:
        default:
            return std::make_pair(std::make_shared<Volume>(load_cube()), glm::ivec3(255));
    }
}

static SplineMapScene* create_scene(scenes::Scene scene, Window& window, Setup& setup) {
    const auto pair = create_volume(scene);
    return new SplineMapScene(window, setup, pair.first, pair.second);
}

static SplineMapScene* create_scene_from_loaded(SplineMapScene* scene, Window& window, Setup& setup) {
    return new SplineMapScene(window, setup, scene->get_volume(), scene->get_color());
}
