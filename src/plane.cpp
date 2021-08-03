#include "plane.hpp"
#include "ray.hpp"

#include <glm/common.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// z = 0: p = (x, -y, 0) | p = (-x, y, 0)
// x = 0: p = (0, y, -z) | p = (0, -y, z)
// y = 0: p = (x, 0, -z) | p = (-x, 0, z)
// p = (x, -y, abs(y) * z + abs(x) * -z)
Plane::Plane(glm::vec3 center, glm::vec3 half_size) : center(center), half_size(half_size) {
    // assert((half_size.x == 0.0f || half_size.y == 0.0f || half_size.z == 0.0f));
    const glm::vec3 p1 = center - half_size;
    const glm::vec3 p2 = center + glm::vec3(half_size.x, -half_size.y, abs(half_size.y) * half_size.z + abs(half_size.x) * -half_size.z);
    const glm::vec3 p3 = center + half_size;
    const glm::vec3 v = p2 - p1;
    const glm::vec3 w = p3 - p1;
    this->normal = glm::normalize(glm::vec3(v.y * w.z - v.z * w.y, v.z * w.x - v.x * w.z, v.x * w.y - v.y * w.x));
    this->span1 = v;
    this->span2 = p3 - p2;

    this->size = half_size * 2.0f;

    this->min = this->center - this->half_size;
    this->max = this->center + this->half_size;

    // https://stackoverflow.com/questions/13199126/find-opengl-rotation-matrix-for-a-plane-given-the-normal-vector-after-the-rotat
    glm::vec3 rotationAxis = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), this->normal);
    float rotationAngle = acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), this->normal));

    float cosAngle = cos(rotationAngle);
    float sinAngle = sin(rotationAngle);

    // https://en.wikipedia.org/wiki/Rotation_matrix#Conversion_from_rotation_matrix_and_to_axis%E2%80%93angle
    this->matrix = glm::transpose(glm::mat4x4(
        cosAngle + rotationAxis.x * rotationAxis.x * (1.0f - cosAngle),
        rotationAxis.x * rotationAxis.y * (1.0f - cosAngle) - rotationAxis.z * sinAngle,
        rotationAxis.x * rotationAxis.z * (1.0f - cosAngle) + rotationAxis.y * sinAngle,
        this->center.x,

        rotationAxis.y * rotationAxis.x * (1.0f - cosAngle) + rotationAxis.z * sinAngle,
        cosAngle + rotationAxis.y * rotationAxis.y * (1.0f - cosAngle),
        rotationAxis.y * rotationAxis.z * (1.0f - cosAngle) - rotationAxis.x * sinAngle,
        this->center.y,

        rotationAxis.z * rotationAxis.x * (1.0f - cosAngle) - rotationAxis.y * sinAngle,
        rotationAxis.z * rotationAxis.y * (1.0f - cosAngle) + rotationAxis.x * sinAngle,
        cosAngle + rotationAxis.z * rotationAxis.z * (1.0f - cosAngle),
        this->center.z,

        0.0f, 0.0f, 0.0f, 1.0f
    ));
    /*
    float hypotXy = hypotf(this->normal.x, this->normal.y);
    this->matrix = glm::mat4x4(
        this->normal.y / hypotXy, -this->normal.x / hypotXy, 0.0f, this->center.x,
        this->normal.x * this->normal.z / hypotXy, this->normal.y * this->normal.z / hypotXy, -hypotXy, this->center.y,
        this->normal.x, this->normal.y, this->normal.z, this->center.z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
    */
    this->inv_matrix = glm::inverse(this->matrix);

    this->transformedMin = this->inv_matrix * glm::vec4(this->min, 1.0f);
    this->transformedMax = this->inv_matrix * glm::vec4(this->max, 1.0f);
}

void Plane::init_vao(const glm::vec3 color) {
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(2, this->buffers);

    GLfloat vertices[4 * 3];
    GLfloat colors[4 * 3];

    const glm::vec3 v1 = center - half_size;
    const glm::vec3 v2 = v1 + span1;
    const glm::vec3 v3 = center + half_size;
    const glm::vec3 v4 = v1 + span2;
    vertices[0] = v1.x; vertices[1] = v1.y; vertices[2] = v1.z;
    vertices[3] = v2.x; vertices[4] = v2.y; vertices[5] = v2.z;
    vertices[6] = v3.x; vertices[7] = v3.y; vertices[8] = v3.z;
    vertices[9] = v4.x; vertices[10] = v4.y; vertices[11] = v4.z;

    colors[0] = color.x; colors[1] = color.y; colors[2] = color.z;
    colors[3] = color.x; colors[4] = color.y; colors[5] = color.z;
    colors[6] = color.x; colors[7] = color.y; colors[8] = color.z;
    colors[9] = color.x; colors[10] = color.y; colors[11] = color.z;

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), colors, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Plane::clean() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(2, this->buffers);
}

void Plane::render() {
    glBindVertexArray(this->vao);

    glLineWidth(3.0f);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glLineWidth(1.0f);

    glBindVertexArray(0);
}

std::optional<glm::vec2> Plane::intersect(const Ray& ray) const {
    const float discriminant = glm::dot(this->normal, ray.dir);
    if (abs(discriminant) < 1.0e-6) {
        return std::nullopt;
    }

    const float t = glm::dot(this->center - ray.origin, this->normal) / discriminant;
    if (t < 0.0f) {
        return std::nullopt;
    }

    const glm::vec3 worldPos = ray.origin + ray.dir * t;
    const glm::vec3 samplePos = (worldPos - this->center / this->size + glm::vec3(1.0f)) * 0.5f;
    glm::vec2 uv;
    if (this->size.x == 0.0f) {
        uv = glm::vec2(samplePos.z, samplePos.y);
    }
    else if (this->size.y == 0.0f) {
        uv = glm::vec2(samplePos.x, samplePos.z);
    }
    else {
        uv = glm::vec2(samplePos.x, samplePos.y);
    }

    if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f) {
        return std::nullopt;
    }

    return std::make_optional(uv);
}
