#include "spline_map.hpp"
#include "spline.hpp"
#include "depressed_cubic.hpp"

#include <functional>

#include <glm/common.hpp>

const int detail = 100;
const glm::vec3 EPSILON = glm::vec3(1.0e-2f, 1.0e-2f, 1.0e-2f);
const glm::vec3 MAX_VALUE = glm::vec3(2.0f);
const glm::vec3 MIN_VALUE = -MAX_VALUE;

Spline::Spline() {
    this->a = glm::vec3(0.0f);
    this->b = glm::vec3(0.0f);
    this->c = glm::vec3(0.0f);
    this->d = glm::vec3(0.0f);
}

Spline::Spline(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c, const glm::vec3 d) {
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
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

    glBindVertexArray(0);
}

Spline::~Spline() {
    delete this->transformedSpline;
}

Spline Spline::with_tangents(const glm::vec3 P1, const glm::vec3 P2, const glm::vec3 tangent1, const glm::vec3 tangent2) {
    // float tmp = P2.y;
    // P2.y = P2.z;
    // P2.z = tmp - 0.5f;

    const glm::vec3 P0 = tangent1;
    const glm::vec3 P3 = tangent2;

    const glm::vec3 a = 2.0f * P1 - 2.0f * P2 + 1.0f * P0 + 1.0f * P3;
    const glm::vec3 b = -3.0f * P1 + 3.0f * P2 - 2.0f * P0 - 1.0f * P3;
    const glm::vec3 c = P0;
    const glm::vec3 d = P1;

    return Spline(a, b, c, d);
}

Spline Spline::with_control_points(const glm::vec3 P1, const glm::vec3 P2, const glm::vec3 P0, const glm::vec3 P3, const float tau) {
    const glm::vec3 a = -tau * P0 + (2.0f - tau) * P1 + (tau - 2.0f) * P2 + tau * P3;
    const glm::vec3 b = 2.0f * tau * P0 + (tau - 3.0f) * P1 + (3.0f - 2.0f * tau) * P2 - tau * P3;
    const glm::vec3 c = -tau * P0 + tau * P2;
    const glm::vec3 d = P1;

    return Spline(a, b, c, d);
}

Spline Spline::with_control_points(const glm::vec3 P1, const glm::vec3 P2, const glm::vec3 P0, const glm::vec3 P3) {
    return Spline::with_control_points(P1, P2, P0, P3, 0.2f);
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

            colors[i * 3 + 0] = 1.0f;
            colors[i * 3 + 1] = 1.0f;
            colors[i * 3 + 2] = 1.0f;
        }

        glBindVertexArray(this->lineVao);

        glBindBuffer(GL_ARRAY_BUFFER, this->buffers[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * detail * sizeof(GLfloat), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, this->buffers[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * detail * sizeof(GLfloat), colors);

        glBindVertexArray(0);
    }
}

void Spline::render() const {
	glBindVertexArray(this->lineVao);

    glLineWidth(3.0f);
	glDrawArrays(GL_LINE_STRIP, 0, detail);
    glLineWidth(1.0f);

    glBindVertexArray(0);
}

void Spline::clean() {
    glDeleteVertexArrays(1, &this->lineVao);
    glDeleteBuffers(4, this->buffers);
}

void solve_quadratic(const float a, const float b, const float c, float& x1, float& x2) {
    const float D = b * b - 4.0f * a * c;

    if (D >= 0.0f) {
        if (a == 0.0f) {
            if (b == 0.0f) {
                x1 = -1.0f;
                x2 = -1.0f;
            } else {
                x1 = -c / b;
                x2 = x1;
            }
        } else {
            x1 = (-b + sqrtf(D)) / (2.0f * a);
            x2 = (-b - sqrtf(D)) / (2.0f * a);
        }
    } else {
        x1 = -1.0f;
        x2 = -1.0f;
    }
}

std::vector<float> Spline::get_extremes() const {
    const glm::vec3 a = this->a * 3.0f;
    const glm::vec3 b = this->b * 2.0f;

    std::vector<float> values = { -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 1.0f };

    solve_quadratic(a.x, b.x, c.x, values[0], values[1]);
    solve_quadratic(a.y, b.y, c.y, values[2], values[3]);
    solve_quadratic(a.z, b.z, c.z, values[4], values[5]);

    return values;
}

void Spline::with_transform(const BetterPlane& plane) {
    this->transformedSpline = new Spline;
    *this->transformedSpline = this->transform(plane.inv_matrix);
}

std::optional<float> Spline::intersect_spline_plane(const glm::vec3 p) const {
    const glm::vec3 conversion = -this->b / (3.0f * this->a);
    DepressedCubic cubic(
        this->transformedSpline->a.y,
        this->transformedSpline->b.y,
        this->transformedSpline->c.y,
        this->transformedSpline->d.y - p.y
    );

    float t = conversion.y + cubic.first_root();
    if (t < 0.0f || t > 1.0f) {
        t = conversion.y + cubic.second_root();
        if (t < 0.0f || t > 1.0f) {
            t = conversion.y + cubic.third_root();
            if (t < 0.0f || t > 1.0f) {
                return std::nullopt;
            }
        }
    }

    return t;
}

void Spline::intersect_spline_plane(const BetterPlane& p, glm::vec3& ts) const {
    const Spline transformedSpline = transform(p.inv_matrix);
    
    const glm::vec3 conversion = -this->b / (3.0f * this->a);
    DepressedCubic cubic(
        transformedSpline.a.y,
        transformedSpline.b.y,
        transformedSpline.c.y,
        transformedSpline.d.y
    );

    ts.x = conversion.y + cubic.first_root();
    ts.y = conversion.y + cubic.second_root();
    ts.z = conversion.y + cubic.third_root();
}

glm::vec3 Spline::position_on_spline(float t) const {
    return t * t * t * this->a + t * t * this->b + t * this->c + this->d;
}
