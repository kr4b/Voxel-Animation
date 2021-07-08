#pragma once

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat4x4.hpp>

#include <algorithm>
#include <iostream>
#include <optional>

#include "math_util.hpp"
#include "plane.hpp"

class Spline {
public:
	glm::vec3 a, b, c, d;

	Spline();
	Spline(const glm::vec3, const glm::vec3, const glm::vec3, const glm::vec3);

	void render() const;
	void update_buffers();
    void clean();

	void update_from_screen_coords(const glm::vec2, const glm::mat4x4, const glm::vec3, const glm::vec3, const glm::vec3);
	void parameters_from_tangents(const glm::vec3, const glm::vec3, const glm::vec3, const glm::vec3);
	void parameters_from_points(const glm::vec3, const glm::vec3, const glm::vec3, const glm::vec3);

	glm::vec3 position_on_spline(const float t) const;

	std::optional<glm::vec2> intersect_spline_aabb(const glm::vec3, const glm::vec3);

    void set_color(const glm::vec3);

	std::vector<float> get_extremes();
	std::optional<float> intersect_spline_plane(const Plane&);

	Spline transform(const glm::mat4x4 &matrix) const {
		glm::vec4 ta = matrix * glm::vec4(this->a.x, this->a.y, this->a.z, 0.0f);
		glm::vec4 tb = matrix * glm::vec4(this->b.x, this->b.y, this->b.z, 0.0f);
		glm::vec4 tc = matrix * glm::vec4(this->c.x, this->c.y, this->c.z, 0.0f);
		glm::vec4 td = matrix * glm::vec4(this->d.x, this->d.y, this->d.z, 1.0f);

		return Spline(
			glm::vec3(ta.x, ta.y, ta.z),
			glm::vec3(tb.x, tb.y, tb.z),
			glm::vec3(tc.x, tc.y, tc.z),
			glm::vec3(td.x, td.y, td.z)
		);
	}

private:
	bool intersection;
	glm::vec3 worldEntry, worldExit;
	glm::vec2 lastCursorPos;
    glm::vec3 start;
    glm::vec3 color;
	GLuint lineVao, pointsVao;
	GLuint buffers[4];

    void init_vao();

	glm::vec3 intersected_aabb(const glm::vec3 t, glm::vec3 aAABBMin, glm::vec3 aAABBMax);
	bool calculate_near_far(const glm::vec3, const glm::vec3, const glm::vec3, const glm::vec3, glm::vec2*);
};

float point_in_aabb(const glm::vec3, const glm::vec3, const glm::vec3);