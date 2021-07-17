#include "ray.hpp"
#include "aabb.hpp"
#include "spline_map.hpp"
#include "volume.hpp"

#include <glm/common.hpp>

Ray::Ray(const glm::vec3 origin, const glm::vec3 dir) : origin(origin), dir(dir) {
}

void Ray::clean() {
    glDeleteVertexArrays(1, &this->lineVao);
    glDeleteVertexArrays(1, &this->pointVao);
    glDeleteBuffers(4, this->buffers);
}

void Ray::init_vao() {
    glGenVertexArrays(1, &this->lineVao);
    glBindVertexArray(this->lineVao);

    glGenBuffers(4, this->buffers);

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &this->pointVao);
    glBindVertexArray(this->pointVao);

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 1 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers[3]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 1 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Ray::update_buffers(std::optional<std::pair<glm::ivec3, float>> intersection, const glm::ivec3 size) {
    {
        GLfloat vertices[2 * 3];
        GLfloat colors[2 * 3];

        const glm::vec3 from = this->origin;
        const glm::vec3 to = from + this->dir;
        vertices[0] = from.x; vertices[1] = from.y; vertices[2] = from.z;
        vertices[3] = to.x; vertices[4] = to.y; vertices[5] = to.z;

        colors[0] = 0.5f; colors[1] = 0.4f; colors[2] = 0.0f;
        colors[3] = 0.5f; colors[4] = 0.4f; colors[5] = 0.0f;

        glBindVertexArray(this->lineVao);

        glBindBuffer(GL_ARRAY_BUFFER, this->buffers[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * 2 * sizeof(GLfloat), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, this->buffers[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * 2 * sizeof(GLfloat), colors);
    }

    this->intersect = intersection.has_value();
    if (this->intersect) {
        GLfloat vertices[1 * 3];
        GLfloat colors[1 * 3];
        const glm::vec3 color = glm::vec3(intersection.value().first) / glm::vec3(size);
        const float t = intersection.value().second;
        const glm::vec3 pos = this->origin + this->dir * t;
        vertices[0] = pos.x; vertices[1] = pos.y; vertices[2] = pos.z;
        colors[0] = 1.0f; colors[1] = 1.0f; colors[2] = 1.0f;

        glBindVertexArray(this->pointVao);

        glBindBuffer(GL_ARRAY_BUFFER, this->buffers[2]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * 1 * sizeof(GLfloat), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, this->buffers[3]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * 1 * sizeof(GLfloat), colors);
    }

    glBindVertexArray(0);
}

glm::vec2 Ray::intersect_ray_aabb(const AABB& aabb) const {
    const glm::vec3 t1 = (aabb.min - this->origin) / this->dir;
    const glm::vec3 t2 = (aabb.max - this->origin) / this->dir;

    const glm::vec3 mins = glm::min(t1, t2);
    const glm::vec3 maxs = glm::max(t1, t2);

    const float near = std::max(std::max(mins.x, mins.y), mins.z);
    const float far = std::min(std::min(maxs.x, maxs.y), maxs.z);
    return glm::vec2(near, far);
}

void Ray::render(bool showRays, bool showIntersections) const {
    glBindVertexArray(this->lineVao);

    if (showRays) {
        glLineWidth(3.0f);
        glDrawArrays(GL_LINE_STRIP, 0, 2);
        glLineWidth(1.0f);
    }

    if (this->intersect && showIntersections) {
        glBindVertexArray(this->pointVao);

        glPointSize(10.0f);
        glDrawArrays(GL_POINTS, 0, 1);
        glPointSize(1.0f);
    }

    glBindVertexArray(0);
}

std::optional<std::pair<glm::ivec3, float>> Ray::walk_spline_map(const SplineMap& splineMap, const Volume& volume, const float step) {
    const glm::vec2 ts = this->intersect_ray_aabb(splineMap.aabb);
    const float len = glm::length(this->dir);

    for (float t = ts.x; t <= ts.y; t += step / len) {
        const glm::vec3 pos = this->origin + this->dir * t;
        const std::optional<glm::vec3> texCoords = splineMap.texture_coords(pos);
        if (texCoords.has_value()) {
            const glm::ivec3 voxel = glm::ivec3(texCoords.value() * glm::vec3(volume.width(), volume.height(), volume.depth()));

            if (voxel.x >= 0 && voxel.x < volume.width() &&
                voxel.y >= 0 && voxel.y < volume.height() &&
                voxel.z >= 0 && voxel.z < volume.depth()) {
                const float color = volume(voxel.x, voxel.y, voxel.z);

                if (color > 0.25f) return std::make_pair(glm::ivec3(voxel.x, voxel.y, voxel.z), t);
            }
        }
    }
    return std::nullopt;
}
