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

const Plane createTopBase(Plane base, Spline spline) {
    const vec3f height = spline.position_on_spline(1.0f) - spline.position_on_spline(0.0f);
    return Plane(base.center + height, base.half_size);
}

SplineMap::SplineMap(Plane base, Spline spline, const gl::GLapi* gl) :
    base(base),
    spline(spline.transform(base.matrix).transform(
        fml::make_matrix<mat44f>(
            1.0f, 0.0f, 0.0f, -base.half_size.x,
            0.0f, 1.0f, 0.0f, -base.half_size.y,
            0.0f, 0.0f, 1.0f, -base.half_size.z,
            0.0f, 0.0f, 0.0f, 1.0f))),
    aabb(createEncompassingAABB(base, this->spline)),
    sizeSquared(dot(base.size, base.size)),
    topBase(createTopBase(base, this->spline)) {

    this->spline.update_buffers();
    this->topBase.init_vao(gl, vec3f(0.0f, 0.2f, 0.3f));
    this->base.init_vao(gl, vec3f(0.0f, 0.2f, 0.3f));
    edgeSplines.push_back(this->spline.transform(fml::make_matrix<mat44f>(
        1.0f, 0.0f, 0.0f, base.span1.x,
        0.0f, 1.0f, 0.0f, base.span1.y,
        0.0f, 0.0f, 1.0f, base.span1.z,
        0.0f, 0.0f, 0.0f, 1.0f)));
    edgeSplines[0].update_buffers();
    edgeSplines.push_back(this->spline.transform(fml::make_matrix<mat44f>(
        1.0f, 0.0f, 0.0f, base.span2.x,
        0.0f, 1.0f, 0.0f, base.span2.y,
        0.0f, 0.0f, 1.0f, base.span2.z,
        0.0f, 0.0f, 0.0f, 1.0f)));
    edgeSplines[1].update_buffers();
    edgeSplines.push_back(this->spline.transform(fml::make_matrix<mat44f>(
        1.0f, 0.0f, 0.0f, base.size.x,
        0.0f, 1.0f, 0.0f, base.size.y,
        0.0f, 0.0f, 1.0f, base.size.z,
        0.0f, 0.0f, 0.0f, 1.0f)));
    edgeSplines[2].update_buffers();
}

void SplineMap::load_uniforms(const gl::GL::UInt program, const gl::GLapi* gl) {
    gl->uniform3f(gl->getUniformLocation(program, "spline_map.aabb.min"), this->aabb.min.x, this->aabb.min.y, this->aabb.min.z);
    gl->uniform3f(gl->getUniformLocation(program, "spline_map.aabb.max"), this->aabb.max.x, this->aabb.max.y, this->aabb.max.z);

    gl->uniform3f(gl->getUniformLocation(program, "spline_map.base.center"), this->base.center.x, this->base.center.y, this->base.center.z);
    gl->uniform3f(gl->getUniformLocation(program, "spline_map.base.half_size"), this->base.half_size.x, this->base.half_size.y, this->base.half_size.z);
    gl->uniform3f(gl->getUniformLocation(program, "spline_map.base.size"), this->base.size.x, this->base.size.y, this->base.size.z);
    gl->uniform3f(gl->getUniformLocation(program, "spline_map.base.normal"), this->base.normal.x, this->base.normal.y, this->base.normal.z);
    gl->uniform3f(gl->getUniformLocation(program, "spline_map.base.span1"), this->base.span1.x, this->base.span1.y, this->base.span1.z);
    gl->uniform3f(gl->getUniformLocation(program, "spline_map.base.span2"), this->base.span2.x, this->base.span2.y, this->base.span2.z);
    gl->uniform3f(gl->getUniformLocation(program, "spline_map.base.min"), this->base.min.x, this->base.min.y, this->base.min.z);
    gl->uniform3f(gl->getUniformLocation(program, "spline_map.base.max"), this->base.max.x, this->base.max.y, this->base.max.z);
    gl->uniformMatrix4fv(gl->getUniformLocation(program, "spline_map.base.matrix"), 1, false, this->base.matrix.data());
    gl->uniformMatrix4fv(gl->getUniformLocation(program, "spline_map.base.inv_matrix"), 1, false, this->base.inv_matrix.data());

    gl->uniform3f(gl->getUniformLocation(program, "spline_map.spline.a"), this->spline.a.x, this->spline.a.y, this->spline.a.z);
    gl->uniform3f(gl->getUniformLocation(program, "spline_map.spline.b"), this->spline.b.x, this->spline.b.y, this->spline.b.z);
    gl->uniform3f(gl->getUniformLocation(program, "spline_map.spline.c"), this->spline.c.x, this->spline.c.y, this->spline.c.z);
    gl->uniform3f(gl->getUniformLocation(program, "spline_map.spline.d"), this->spline.d.x, this->spline.d.y, this->spline.d.z);

    gl->uniform1f(gl->getUniformLocation(program, "spline_map.size_squared"), this->sizeSquared);
    gl->uniform1f(gl->getUniformLocation(program, "spline_map.width"), this->sizeSquared);
    gl->uniform1f(gl->getUniformLocation(program, "spline_map.height"), this->sizeSquared);
}

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

void SplineMap::render(const gl::GLapi* gl) {
    this->base.render(gl);
    this->topBase.render(gl);
    this->spline.render();
    for (const Spline& edgeSpline : this->edgeSplines) {
        edgeSpline.render();
    }
}
