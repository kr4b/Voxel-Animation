#include <algorithm>
#include "spline_map.hpp"

const AABB createEncompassingAABB(Plane base, Spline spline) {
    std::vector<float> extremes = spline.get_extremes();
    vec3f min = fml::make_vector<vec3f>(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
    vec3f max = fml::make_vector<vec3f>(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    for (const float t : extremes) {
        if (t >= 0.0f && t <= 1.0f) {
            const vec3f pos = spline.position_on_spline(t);
            min = fml::min(min, pos);
            max = fml::max(max, pos);
        }
    }
    
    return AABB(
        min,
        base.size + max
    );
}

SplineMap::SplineMap(Plane base, Spline spline) :
    base(base),
    spline(spline.transform(
        fml::make_matrix<mat44f>(
            1.0f, 0.0f, 0.0f, -base.half_size.x,
            0.0f, 1.0f, 0.0f, -base.half_size.y,
            0.0f, 0.0f, 1.0f, -base.half_size.z,
            0.0f, 0.0f, 0.0f, 1.0f))),
    aabb(createEncompassingAABB(base, spline)),
    sizeSquared(dot(base.size, base.size)) {}

std::optional<vec3f> SplineMap::texture_coords(const vec3f pos) {
    const Plane plane(pos, this->base.size);

    auto result = this->spline.intersect_spline_plane(plane);
    if (result.has_value()) {
        const float t = result.value();
        const vec3f edgePos1 = this->spline.position_on_spline(t);
        const vec3f edgePos2 = edgePos1 + this->base.size;
        const vec3f diff1 = edgePos1 - pos;
        const vec3f diff2 = edgePos2 - pos;
        if (dot(diff1, diff1) > this->sizeSquared || dot(diff2, diff2) > this->sizeSquared) {
            return std::nullopt;
        }

        // Calculate and return texture coordinates
        // x and z are the distance to the base spline, decomposed into components
        // y is currently assumed to be t
        const float xComp = dot(edgePos1, this->base.span1) / dot(this->base.span1, this->base.span1);
        const float zComp = dot(edgePos2, this->base.span2) / dot(this->base.span2, this->base.span2);
        return vec3f(xComp, t, zComp);
    }

    return std::nullopt;
}

void SplineMap::render(const gl::GLapi*) {

}
