#include "spline_map.hpp"
#include "spline.hpp"
#include "depressed_cubic.hpp"

#include <functional>

#include <glm/common.hpp>

const int detail = 100;
const glm::vec3 EPSILON = glm::vec3(1.0e-2f, 1.0e-2f, 1.0e-2f);
const glm::vec3 ZERO_VECTOR = glm::vec3(0.0f);
const glm::vec3 ONE_VECTOR = glm::vec3(1.0f);
const glm::vec3 MAX_VALUE = glm::vec3(2.0f, 2.0f, 2.0f);
const glm::vec3 MIN_VALUE = -MAX_VALUE;

Spline::Spline() {
	this->intersection = false;
	this->worldEntry = glm::vec3(0.0f);
	this->worldExit = glm::vec3(0.0f);

	this->lastCursorPos = glm::vec2(0.0f);

	this->a = glm::vec3(0.0f);
	this->b = glm::vec3(0.0f);
	this->c = glm::vec3(0.0f);
	this->d = glm::vec3(0.0f);
    this->start = glm::vec3(0.0f);
    this->color = glm::vec3(1.0f);

	this->init_vao();
}

Spline::Spline(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c, const glm::vec3 d) {
	this->intersection = false;
	this->worldEntry = glm::vec3(0.0f);
	this->worldExit = glm::vec3(0.0f);

	this->lastCursorPos = glm::vec2(0.0f);

	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
	this->start = glm::vec3(0.0f);
	this->color = glm::vec3(1.0f);

	this->init_vao();
}

void Spline::clean() {
    glDeleteVertexArrays(1, &this->lineVao);
    glDeleteVertexArrays(1, &this->pointsVao);
    glDeleteBuffers(4, this->buffers);
}

void Spline::init_vao() {
	glGenVertexArrays(1, &this->lineVao);
	glBindVertexArray(this->lineVao);

    glGenBuffers(4, this->buffers);

    // Spline vertices
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * detail * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

    // Spline colors
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * detail * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &this->pointsVao);
	glBindVertexArray(this->pointsVao);

    // Point vertices
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

    // Point colors
	glBindBuffer(GL_ARRAY_BUFFER, this->buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Spline::parameters_from_tangents(const glm::vec3 P1, const glm::vec3 P2, const glm::vec3 tangent1, const glm::vec3 tangent2) {
	// float tmp = P2.y;
	// P2.y = P2.z;
	// P2.z = tmp - 0.5f;

	const glm::vec3 P0 = tangent1;
	const glm::vec3 P3 = tangent2;

	this->a = 2.0f * P1 - 2.0f * P2 + 1.0f * P0 + 1.0f * P3;
	this->b = -3.0f * P1 + 3.0f * P2 - 2.0f * P0 - 1.0f * P3;
	this->c = P0;
	this->d = P1;

    this->start = this->position_on_spline(0.0f);
}


void Spline::parameters_from_points(const glm::vec3 P1, const glm::vec3 P2, const glm::vec3 P0, const glm::vec3 P3) {
	const float tau = 0.2f;

	this->a = -tau * P0 + (2.0f - tau) * P1 + (tau - 2.0f) * P2 + tau * P3;
	this->b = 2.0f * tau * P0 + (tau - 3.0f) * P1 + (3.0f - 2.0f * tau) * P2 - tau * P3;
	this->c = -tau * P0 + tau * P2;
	this->d = P1;

    this->start = this->position_on_spline(0.0f);
}

void Spline::update_from_screen_coords(const glm::vec2 coords, const glm::mat4x4 inverseProjCamera, const glm::vec3 cameraWorldPos, const glm::vec3 tangent1, const glm::vec3 tangent2) {
	const glm::vec2 transformed = coords * 2.0f - glm::vec2(1.0f, 1.0f);
	const glm::vec4 hray = glm::vec4(transformed.x, transformed.y, 1.0f, 1.0f);
	const glm::vec4 wray = inverseProjCamera * hray;

	const glm::vec3 origin = cameraWorldPos;
	const glm::vec3 direction = glm::normalize(glm::vec3(wray.x, wray.y, wray.z) / wray.w - origin);

	const glm::vec3 P1 = origin;
	const glm::vec3 P2 = origin + direction * glm::length(origin) * 2.0f;

    parameters_from_tangents(P1, P2, tangent1, tangent2);
}

void Spline::update_buffers() {
	{
		const float stepSize = 1.0f / (float)detail;

		GLfloat vertices[detail * 3];
		GLfloat colors[detail * 3];

		float t = 0;
		for (int i = 0; i < detail; i++, t += stepSize) {
			const glm::vec3 point = this->position_on_spline(t);
			vertices[i * 3 + 0] = point.x;
			vertices[i * 3 + 1] = point.y;
			vertices[i * 3 + 2] = point.z;

            colors[i * 3 + 0] = this->color.x;
			colors[i * 3 + 1] = this->color.y;
			colors[i * 3 + 2] = this->color.z;
		}

		glBindVertexArray(this->lineVao);

		glBindBuffer(GL_ARRAY_BUFFER, this->buffers[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * detail * sizeof(GLfloat), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, this->buffers[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * detail * sizeof(GLfloat), colors);

        glBindVertexArray(0);
	}

	{
		GLfloat vertices[3 * 3];
		GLfloat colors[3 * 3];

        vertices[0] = this->start.x; vertices[1] = this->start.y; vertices[2] = this->start.z;
        vertices[3] = this->worldEntry.x; vertices[4] = this->worldEntry.y; vertices[5] = this->worldEntry.z;
        vertices[6] = this->worldExit.x; vertices[7] = this->worldExit.y; vertices[8] = this->worldExit.z;

        colors[0] = 0.0f; colors[1] = 1.0f; colors[2] = 0.0f;
        colors[3] = 1.0f; colors[4] = 0.0f; colors[5] = 0.0f;
        colors[6] = 1.0f; colors[7] = 0.0f; colors[8] = 0.0f;

        glBindVertexArray(this->pointsVao);

		glBindBuffer(GL_ARRAY_BUFFER, this->buffers[2]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * 3 * sizeof(GLfloat), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, this->buffers[3]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * 3 * sizeof(GLfloat), colors);

        glBindVertexArray(0);
	}
}

void Spline::render() const {
		glBindVertexArray(this->lineVao);

    glLineWidth(3.0f);
		glDrawArrays(GL_LINE_STRIP, 0, detail);
    glLineWidth(1.0f);

    glBindVertexArray(this->pointsVao);
    glPointSize(10.0f);

    glDrawArrays(GL_POINTS, 0, 3);
    glPointSize(1.0f);

		glBindVertexArray(0);
}

void Spline::set_color(const glm::vec3 color) {
    this->color = color;
}

std::vector<float> Spline::get_extremes() {
	const glm::vec3 a = this->a * glm::vec3(3.0f, 3.0f, 3.0f);
	const glm::vec3 b = this->b * glm::vec3(2.0f, 2.0f, 2.0f);

	std::vector<float> values = { -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f };

	const glm::vec3 D = b * b - glm::vec3(4.0f, 4.0f, 4.0f) * a * this->c;

	if (D.x >= 0.0f) {
		values[0] = (-b.x + sqrt(D.x)) / (2.0f * a.x);
		values[1] = (-b.x - sqrt(D.x)) / (2.0f * a.x);
	}
	if (D.y >= 0.0f) {
		values[2] = (-b.y + sqrt(D.y)) / (2.0f * a.y);
		values[3] = (-b.y - sqrt(D.y)) / (2.0f * a.y);
	}
	if (D.z >= 0.0f) {
		values[4] = (-b.z + sqrt(D.z)) / (2.0f * a.z);
		values[5] = (-b.z - sqrt(D.z)) / (2.0f * a.z);
	}

	return values;
}

std::optional<float> Spline::intersect_spline_plane(const Plane &plane) {
	const glm::vec4 transformed_min = plane.inv_matrix * glm::vec4(plane.min.x, plane.min.y, plane.min.z, 1.0f);
	const glm::vec4 transformed_max = plane.inv_matrix * glm::vec4(plane.max.x, plane.max.y, plane.max.z, 1.0f);
	Spline transformed_spline = this->transform(plane.inv_matrix);

	std::optional<glm::vec2> result = transformed_spline.intersect_spline_aabb(
		glm::vec3(transformed_min.x, transformed_min.y, transformed_min.z),
		glm::vec3(transformed_max.x, transformed_max.y, transformed_max.z)
	);

	if (result.has_value()) return std::optional<float>(result.value().x);
	return std::nullopt;
}

inline glm::vec3 Spline::position_on_spline(float t) const {
	return t * t * t * this->a + t * t * this->b + t * this->c + this->d;
}

std::optional<glm::vec2> Spline::intersect_spline_aabb(const glm::vec3 aAABBMin, const glm::vec3 aAABBMax) {
	const glm::vec3 conversion = -this->b / (3.0f * this->a);

	DepressedCubic cubic_min_x(this->a.x, this->b.x, this->c.x, this->d.x - aAABBMin.x);
	DepressedCubic cubic_min_y(this->a.y, this->b.y, this->c.y, this->d.y - aAABBMin.y);
	DepressedCubic cubic_min_z(this->a.z, this->b.z, this->c.z, this->d.z - aAABBMin.z);
	DepressedCubic cubic_max_x(this->a.x, this->b.x, this->c.x, this->d.x - aAABBMax.x);
	DepressedCubic cubic_max_y(this->a.y, this->b.y, this->c.y, this->d.y - aAABBMax.y);
	DepressedCubic cubic_max_z(this->a.z, this->b.z, this->c.z, this->d.z - aAABBMax.z);
	
	const glm::vec3 t1 = conversion + glm::vec3(
		cubic_min_x.second_root(),
		cubic_min_y.second_root(),
		cubic_min_z.second_root());
	const glm::vec3 t2 = conversion + glm::vec3(
		cubic_max_x.second_root(),
		cubic_max_y.second_root(),
		cubic_max_z.second_root());

	glm::vec2 ts = glm::vec2(2.0f, -2.0f);
	bool result = calculate_near_far(t1, t2, aAABBMin, aAABBMax, &ts);

	if (!result) {
		const glm::vec3 first_t1 = conversion + glm::vec3(
			cubic_min_x.first_root(),
			cubic_min_y.first_root(),
			cubic_min_z.first_root());
		const glm::vec3 first_t2 = conversion + glm::vec3(
			cubic_max_x.first_root(),
			cubic_max_y.first_root(),
			cubic_max_z.first_root());

		result = calculate_near_far(first_t1, first_t2, aAABBMin, aAABBMax, &ts);

		if (!result) {
			const glm::vec3 third_t1 = conversion + glm::vec3(
				cubic_min_x.third_root(),
				cubic_min_y.third_root(),
				cubic_min_z.third_root());
			const glm::vec3 third_t2 = conversion + glm::vec3(
				cubic_max_x.third_root(),
				cubic_max_y.third_root(),
				cubic_max_z.third_root());

			result = calculate_near_far(third_t1, third_t2, aAABBMin, aAABBMax, &ts);
		}
	}

	//std::cout << ts.x << ", " << ts.y << std::endl;
	this->intersection = result;
	this->worldEntry = this->position_on_spline(ts.x);
	this->worldExit = this->position_on_spline(ts.y);

	if (!result) return std::nullopt;
	return std::optional<glm::vec2>(ts);
}

bool Spline::calculate_near_far(const glm::vec3 t1, const glm::vec3 t2, const glm::vec3 aAABBMin, const glm::vec3 aAABBMax, glm::vec2* ts) {
	const glm::vec3 it1 = this->intersected_aabb(t1, aAABBMin, aAABBMax);
	const glm::vec3 it2 = this->intersected_aabb(t2, aAABBMin, aAABBMax);

	const glm::vec3 nt1 = t1 * it1 + (ONE_VECTOR - it1) * MAX_VALUE;
	const glm::vec3 nt2 = t2 * it2 + (ONE_VECTOR - it2) * MAX_VALUE;

	const glm::vec3 ft1 = t1 * it1 + (ONE_VECTOR - it1) * MIN_VALUE;
	const glm::vec3 ft2 = t2 * it2 + (ONE_VECTOR - it2) * MIN_VALUE;

	const glm::vec3 inear = glm::vec3(std::min(nt1.x, nt2.x), std::min(nt1.y, nt2.y), std::min(nt1.z, nt2.z));
	ts->x = std::min(ts->x, std::min(inear.x, std::min(inear.y, inear.z)));

	const glm::vec3 ifar = glm::vec3(std::max(ft1.x, ft2.x), std::max(ft1.y, ft2.y), std::max(ft1.z, ft2.z));
	ts->y = std::max(ts->y, std::max(ifar.x, std::max(ifar.y, ifar.z)));

	return ts->x <= ts->y && ts->y >= 0.0f;
}

glm::vec3 Spline::intersected_aabb(const glm::vec3 t, glm::vec3 aAABBMin, glm::vec3 aAABBMax) {
	const glm::vec3 P0 = this->position_on_spline(t.x);
	const glm::vec3 P1 = this->position_on_spline(t.y);
	const glm::vec3 P2 = this->position_on_spline(t.z);

	const glm::vec3 resultT = step(ZERO_VECTOR, t) * step(t, ONE_VECTOR);
	const float result0 = point_in_aabb(P0, aAABBMin, aAABBMax);
	const float result1 = point_in_aabb(P1, aAABBMin, aAABBMax);
	const float result2 = point_in_aabb(P2, aAABBMin, aAABBMax);

	return glm::vec3(
		resultT.x * result0,
		resultT.y * result1,
		resultT.z * result2);
}

float point_in_aabb(const glm::vec3 aPoint, const glm::vec3 aAABBMin, const glm::vec3 aAABBMax) {
	const glm::vec3 result = step(aAABBMin - EPSILON, aPoint) * step(aPoint, aAABBMax + EPSILON);
	return result.x * result.y * result.z;
}
