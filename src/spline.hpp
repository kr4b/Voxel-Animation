#pragma once

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat4x4.hpp>

#include <algorithm>
#include <iostream>
#include <optional>
#include <vector>

#include "math_util.hpp"
#include "plane.hpp"

// Cubic Hermite spline where a, b, c, d describe the constants in front of t^3, t^2, t^1, t^0 respectively
// https://en.wikipedia.org/wiki/Cubic_Hermite_spline
class Spline {
public:
    glm::vec3 a, b, c, d;
    Spline* transformedSpline = NULL;

    Spline();
    Spline(const glm::vec3, const glm::vec3, const glm::vec3, const glm::vec3);
    ~Spline();

    void init_vao();
    void render() const;
    void update_buffers();
    void clean();

    // Create spline with given starting and ending points and two tangents
    static Spline with_tangents(const glm::vec3, const glm::vec3, const glm::vec3, const glm::vec3);
    // Create spline with given starting and ending points and two control points
    static Spline with_control_points(const glm::vec3, const glm::vec3, const glm::vec3, const glm::vec3, const float);
    static Spline with_control_points(const glm::vec3, const glm::vec3, const glm::vec3, const glm::vec3);

    glm::vec3 position_on_spline(const float t) const;

    void set_color(const glm::vec3);

    std::vector<float> get_extremes() const;
    std::optional<float> intersect_spline_plane(const glm::vec3) const;
    void intersect_spline_plane(const Plane&, glm::vec3&) const;

    Spline transform(const glm::mat4x4&) const;
    void with_transform(const Plane&);

private:
    GLuint lineVao;
    GLuint buffers[4];
};

float point_in_aabb(const glm::vec3, const glm::vec3, const glm::vec3);
