#pragma once

#include <memory>
#include <tuple>

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

static std::tuple<std::shared_ptr<Volume>, glm::ivec3, float> create_volume(scenes::Scene scene) {
    switch (scene) {
        case scenes::Pig: // Piggy Pink
            return std::make_tuple(std::make_shared<Volume>(load_fld_volume("assets/pig8.fld")), glm::ivec3(253, 221, 230), 0.0f);
        case scenes::Bonsai: // Bark Brown
            return std::make_tuple(std::make_shared<Volume>(load_fld_volume("assets/bonsai.fld")), glm::ivec3(102, 73, 58), 0.0f);
        case scenes::Carp: // Apricot Orange
            return std::make_tuple(std::make_shared<Volume>(load_fld_volume("assets/carp8.fld", glm::ivec3(1, 2, 0))), glm::ivec3(253, 180, 130), glm::radians(89.9f));
        case scenes::Orange: // Sweet Orange
            return std::make_tuple(std::make_shared<Volume>(load_fld_volume("assets/orange.fld")), glm::ivec3(250, 141, 73), 0.0f);
        case scenes::Stent: // Dark Flesh
            return std::make_tuple(std::make_shared<Volume>(load_fld_volume("assets/stent8.fld")), glm::ivec3(186, 141, 135), 0.0f);
        case scenes::Tomato: // Tomato
            return std::make_tuple(std::make_shared<Volume>(load_fld_volume("assets/tomato.fld")), glm::ivec3(206, 41, 57), 0.0f);
        case scenes::Tooth: // Neutral Pearl White
            return std::make_tuple(std::make_shared<Volume>(load_fld_volume("assets/tooth.fld")), glm::ivec3(242, 238, 231), 0.0f);
        case scenes::Bunny: // White Fur
            return std::make_tuple(std::make_shared<Volume>(load_mhd_volume("assets/bunny.mhd")), glm::ivec3(244, 241, 240), 0.0f);
        case scenes::Seaweed: // Nori Seaweed Green
            return std::make_tuple(std::make_shared<Volume>(load_seaweed()), glm::ivec3(70, 72, 38), 0.0f);
        case scenes::Cube:
        default:
            return std::make_tuple(std::make_shared<Volume>(load_cube()), glm::ivec3(255), 0.0f);
    }
}

static SplineMapScene* create_scene(scenes::Scene scene, Window& window, Setup& setup) {
    const auto tuple = create_volume(scene);
    return new SplineMapScene(window, setup, std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
}

static SplineMapScene* create_scene_from_loaded(SplineMapScene* scene, Window& window, Setup& setup) {
    return new SplineMapScene(window, setup, scene->get_volume(), scene->get_color(), scene->get_rotation());
}
