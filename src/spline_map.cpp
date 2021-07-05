#include "spline_map.hpp"

const AABB createEncompassingAABB(const Plane base, const Spline spline) {

}

SplineMap::SplineMap(const Plane base, const Spline spline) : base(base), spline(spline), aabb(createEncompassingAABB(base, spline)), sizeSquared(dot(base.size, base.size)) {}

std::optional<vec3f> SplineMap::texture_coords(const vec3f pos) const {
    const Plane plane = Plane(pos, this->base.size);

    if (const auto t = this->spline.intersect_spline_plane(plane)) {
        const vec3f edgePos1 = this->spline.position_on_spline(*t);
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
        return vec3f(xComp, *t, zComp);
    }

    return std::nullopt;
}

void SplineMap::render(const gl::GLapi*) {

}
