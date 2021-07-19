#pragma once

#include <optional>

#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/mat4x4.hpp>

class Ray;

class Plane {
public:
    const glm::vec3 center;
    const glm::vec3 half_size;
    glm::vec3 span1, span2;

    glm::vec3 size;
    glm::vec3 normal;

    glm::vec3 min;
    glm::vec3 max;

    glm::mat4x4 matrix;
    glm::mat4x4 inv_matrix;

    glm::vec3 transformedMin, transformedMax;

    Plane(glm::vec3 center, glm::vec3 half_size);

    void init_vao(const glm::vec3);
    void render();
    void clean();

    Plane transform(const glm::mat4x4 &matrix) {
        glm::vec4 transformed = matrix * glm::vec4(this->center.x, this->center.y, this->center.z, 1.0);
        return Plane(
            glm::vec3(transformed.x, transformed.y, transformed.z),
            this->half_size
        );
    }

    std::optional<glm::vec2> intersect(const Ray&) const;

private:
    GLuint vao;
    GLuint buffers[2];
};
