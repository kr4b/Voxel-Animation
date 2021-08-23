#include "ray.hpp"
#include "aabb.hpp"
#include "spline_map.hpp"
#include "volume.hpp"
#include "better_plane.hpp"

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
    glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers[3]);
    glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Ray::update_buffers(glm::vec2 intersection) {
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

    {
        GLfloat vertices[2 * 3];
        GLfloat colors[2 * 3];
        const glm::vec3 pos1 = this->origin + this->dir * intersection.x;
        vertices[0] = pos1.x; vertices[1] = pos1.y; vertices[2] = pos1.z;
        colors[0] = 0.9f; colors[1] = 0.3f; colors[2] = 0.5f;

        const glm::vec3 pos2 = this->origin + this->dir * intersection.y;
        vertices[3] = pos2.x; vertices[4] = pos2.y; vertices[5] = pos2.z;
        colors[3] = 0.3f; colors[4] = 0.9f; colors[5] = 0.5f;

        glBindVertexArray(this->pointVao);

        glBindBuffer(GL_ARRAY_BUFFER, this->buffers[2]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * 2 * sizeof(GLfloat), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, this->buffers[3]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * 2 * sizeof(GLfloat), colors);
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
        glDrawArrays(GL_POINTS, 0, 2);
        glPointSize(1.0f);
    }

    glBindVertexArray(0);
}

std::optional<std::pair<glm::ivec3, float>> Ray::walk_spline_map(const SplineMap& splineMap, const Volume& volume, const float threshold, const float stepSize) {
    const glm::vec2 ts = this->intersect_ray_spline_map(splineMap);
    const float len = glm::length(this->dir);

    for (float t = ts.x; t <= ts.y; t += stepSize / len) {
        const glm::vec3 pos = this->origin + this->dir * t;
        const std::optional<glm::vec3> texCoords = splineMap.texture_coords(pos);
        if (texCoords.has_value()) {
            const glm::ivec3 voxel = glm::ivec3(texCoords.value() * glm::vec3(volume.width(), volume.height(), volume.depth()));
            const float color = volume(voxel.x, voxel.y, voxel.z);
            if (color > threshold) return std::make_pair(glm::ivec3(voxel.x, voxel.y, voxel.z), t);
        }
    }
    return std::nullopt;
}

std::optional<float> Ray::intersect_ray_plane(const BetterPlane& plane) const {
    const float del = glm::dot(plane.normal, dir);

    if (del == 0.0f) return std::nullopt;

    const float result = glm::dot(plane.normal, (plane.point - origin)) / del;

    const glm::vec3 intersection = origin + result * dir;
    const glm::vec3 min = plane.point;
    const glm::vec3 max = plane.point + plane.span1 + plane.span2;
    if (
        intersection.x >= min.x &&
        intersection.y >= min.y &&
        intersection.z >= min.z &&
        intersection.x <= max.x &&
        intersection.y <= max.y &&
        intersection.z <= max.z
    ) {
        return result;
    }

    return std::nullopt;
}

std::optional<float> Ray::intersect_ray_line_segment(const glm::vec3 point1, const glm::vec3 point2) const {
    const glm::vec3 dpoint = point1 - point2;

    const float dxy = dir.y * dpoint.x - dir.x * dpoint.y;
    const float dxz = dir.z * dpoint.x - dir.x * dpoint.z;
    const float dyz = dir.z * dpoint.y - dir.y * dpoint.z;

    if (dxy == 0.0f && dxz == 0.0f && dyz == 0.0f) return std::nullopt;

    float result;
    if (abs(dxy) > abs(dxz) && abs(dxy) > abs(dyz)) result = (dir.y * (point1.x - origin.x) - dir.x * (point1.y - origin.y)) / dxy;
    if (abs(dxz) > abs(dyz) && abs(dxz) > abs(dxy)) result = (dir.z * (point1.x - origin.x) - dir.x * (point1.z - origin.z)) / dxz;
    if (abs(dyz) > abs(dxy) && abs(dyz) > abs(dxz)) result = (dir.z * (point1.y - origin.y) - dir.y * (point1.z - origin.z)) / dyz;

    if (result < 0.0f || result > 1.0f) return std::nullopt;

    const glm::vec3 intersection = point1 * (1.0f - result) + point2 * result;
    return (intersection.x - origin.x) / dir.x;
}

void find_ray_spline_intersection(
    const Ray* ray, const SplineChain& splineChain,
    const BetterPlane& plane, const float ts[3 * MAX_SPLINE_CHAIN_LENGTH],
    const glm::vec3& offset, glm::vec2& returnValue)
{
    for (int i = 0; i < 3 * splineChain.length; i++) {
        if (ts[i] < 0.0f || ts[i] > 1.0f) continue;

        const glm::vec3 pos1 = splineChain.position_on_chain(ts[i]);
        const glm::vec3 pos2 = pos1 + offset;

        const std::optional<float> t = ray->intersect_ray_line_segment(pos1, pos2);
        if (t.has_value()) {
            returnValue.x = std::min(returnValue.x, t.value());
            returnValue.y = std::max(returnValue.y, t.value());
        }
    }
}

glm::vec2 Ray::intersect_ray_spline_map(const SplineMap& splineMap) const {
    glm::vec2 returnValue(5.0f, -5.0f);

    const glm::vec3 span1 = splineMap.base.span1;
    const glm::vec3 span2 = splineMap.base.span2;

    const BetterPlane plane1(origin, glm::normalize(dir * (1.0f - span1)), span1);
    const BetterPlane plane2(origin, glm::normalize(dir * (1.0f - span2)), span2);

    // Up to 3 intersection points per spline, up to n splines per chain
    float ts[3 * MAX_SPLINE_CHAIN_LENGTH];

    splineMap.splineChain.intersect_spline_plane(plane1, ts);
    find_ray_spline_intersection(this, splineMap.splineChain, plane1, ts, span1, returnValue);

    splineMap.edgeSplines[2].intersect_spline_plane(plane1, ts);
    find_ray_spline_intersection(this, splineMap.edgeSplines[2], plane1, ts, -span1, returnValue);

    splineMap.splineChain.intersect_spline_plane(plane2, ts);
    find_ray_spline_intersection(this, splineMap.splineChain, plane2, ts, span2, returnValue);

    splineMap.edgeSplines[2].intersect_spline_plane(plane2, ts);
    find_ray_spline_intersection(this, splineMap.edgeSplines[2], plane2, ts, -span2, returnValue);

    const std::optional<float> ogBaseT = intersect_ray_plane(splineMap.base);
    const std::optional<float> opBaseT = intersect_ray_plane(splineMap.topBase);

    if (ogBaseT.has_value()) {
        returnValue.x = std::min(returnValue.x, ogBaseT.value());
        returnValue.y = std::max(returnValue.y, ogBaseT.value());
    }

    if (opBaseT.has_value()) {
        returnValue.x = std::min(returnValue.x, opBaseT.value());
        returnValue.y = std::max(returnValue.y, opBaseT.value());
    }

    return returnValue;
}
