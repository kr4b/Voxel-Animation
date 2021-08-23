#pragma once

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat4x4.hpp>

#include <algorithm>
#include <iostream>
#include <optional>
#include <deque>
#include <vector>

#include "math_util.hpp"
#include "plane.hpp"
#include "spline.hpp"
#include "better_plane.hpp"

#define MAX_SPLINE_CHAIN_LENGTH 5

class SplineChain {
public:
    Spline splines[MAX_SPLINE_CHAIN_LENGTH];
    size_t length = 0;

    SplineChain();
    SplineChain(std::vector<Spline>);

    void init_vao();
    void render() const;
    void update_buffers();
    void clean();

    static SplineChain from_points(std::deque<glm::vec3>, float);
    static SplineChain from_points_with_outer_tangents(glm::vec3, glm::vec3, std::deque<glm::vec3>, float);
    static SplineChain from_points_with_tangents(std::vector<glm::vec3>, std::vector<glm::vec3>);

    glm::vec3 position_on_chain(const float t) const;
    glm::vec3 position_on_transformed_chain(const float t) const;

    std::vector<float> get_extremes() const;
    std::optional<float> intersect_spline_plane(const glm::vec3) const;
    std::optional<float> intersect_spline_plane(const BetterPlane&) const;

    SplineChain transform(const glm::mat4x4 &matrix) const {
        std::vector<Spline> transformed_splines;

        for (int i = 0; i < this->length; i++) {
            transformed_splines.push_back(this->splines[i].transform(matrix));
        }

        return SplineChain(transformed_splines);
    }

    void with_transform(const Plane&);

private:
    GLuint lineVao;
    GLuint buffers[4];
};
