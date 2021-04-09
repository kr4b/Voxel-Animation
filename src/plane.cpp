#include "plane.hpp"

// z = 0: p = (x, -y, 0) | p = (-x, y, 0)
// x = 0: p = (0, y, -z) | p = (0, -y, z)
// y = 0: p = (x, 0, -z) | p = (-x, 0, z)
// p = (x, -y, abs(y) * z + abs(x) * -z)
Plane::Plane(vec3f center, vec3f size) : center(center), size(size) {
    assert(size.x == 0.0f || size.y == 0.0f || size.z == 0.0f);
    const vec3f p1 = center - size;
    const vec3f p2 = center + vec3f(size.x, -size.y, abs(size.y) * size.z + abs(size.x) * -size.z);
    const vec3f p3 = center + size;
    const vec3f v = p2 - p1;
    const vec3f w = p3 - p1;
    this->normal = fml::normalize(vec3f(v.y * w.z - v.z * w.y, v.z * w.x - v.x * w.z, v.x * w.y - v.y * w.x));
}

std::optional<vec2f> Plane::intersect(const Ray& ray) const {
    const float discriminant = dot(this->normal, ray.dir);
    if (abs(discriminant) < 1.0e-6) {
        return std::nullopt;
    }

    const float t = dot(this->center - ray.origin, this->normal) / discriminant;
    if (t < 0.0f) {
        return std::nullopt;
    }

    const vec3f worldPos = ray.origin + ray.dir * t;
    const vec3f samplePos = (worldPos - this->center / this->size + fml::make_splat<vec3f>(1.0f)) * 0.5f;
    vec2f uv;
    if (this->size.x == 0.0f) {
        uv = vec2f(samplePos.z, samplePos.y);
    }
    else if (this->size.y == 0.0f) {
        uv = vec2f(samplePos.x, samplePos.z);
    }
    else {
        uv = vec2f(samplePos.x, samplePos.y);
    }

    if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f) {
        return std::nullopt;
    }

    return std::make_optional(uv);
}
