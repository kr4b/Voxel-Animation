#include <algorithm>

#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "spline_map.hpp"


const AABB createEncompassingAABB(Plane base, SplineChain splineChain) {
    std::vector<float> extremes = splineChain.get_extremes();
    glm::vec3 min = glm::vec3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    glm::vec3 max = glm::vec3(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    for (const float t : extremes) {
        if (t >= 0.0f && t <= 1.0f) {
            const glm::vec3 pos = splineChain.position_on_chain(t);
            min = glm::min(min, pos);
            max = glm::max(max, pos);
        }
    }
    
    return AABB(
        min,
        base.size + max
    );
}

const Plane createTopBase(const Plane& base, const SplineChain& splineChain) {
    const glm::vec3 height = splineChain.position_on_chain(1.0f) - splineChain.position_on_chain(0.0f);
    return Plane(base.center + height, base.half_size);
}

const SplineChain transformSplineChain(const Plane& base, const SplineChain& splineChain) {
    return splineChain.transform(base.matrix).transform(
        glm::mat4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            -base.half_size.x, -base.half_size.y, -base.half_size.z, 1.0f));
}

SplineMap::SplineMap(const Plane& base, const SplineChain& splineChain) :
    base(base),
    splineChain(transformSplineChain(this->base, splineChain)),
    aabb(createEncompassingAABB(this->base, this->splineChain)),
    sizeSquared(dot(this->base.size, this->base.size)),
    topBase(createTopBase(this->base, this->splineChain)) {

    this->splineChain.init_vao();
    this->splineChain.update_buffers();
    this->splineChain.with_transform(this->base);
    this->topBase.init_vao(glm::vec3(0.0f, 0.2f, 0.3f));
    this->base.init_vao(glm::vec3(0.0f, 0.2f, 0.3f));
    edgeSplines[0] = this->splineChain.transform(glm::mat4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        this->base.span1.x, this->base.span1.y, this->base.span1.z, 1.0f));
    edgeSplines[0].init_vao();
    edgeSplines[0].update_buffers();
    edgeSplines[1] = this->splineChain.transform(glm::mat4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        this->base.span2.x, this->base.span2.y, this->base.span2.z, 1.0f));
    edgeSplines[1].init_vao();
    edgeSplines[1].update_buffers();
    edgeSplines[2] = this->splineChain.transform(glm::mat4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        this->base.size.x, this->base.size.y, this->base.size.z, 1.0f));
    edgeSplines[2].init_vao();
    edgeSplines[2].update_buffers();
}

std::optional<glm::vec3> SplineMap::texture_coords(const glm::vec3 pos) const {
    const glm::vec4 p = this->base.inv_matrix * glm::vec4(pos, 1.0f);
    std::optional<float> result = this->splineChain.intersect_spline_plane(glm::vec3(p.x, p.y, p.z));

    if (result.has_value()) {
        const float t = result.value();
        const glm::vec3 edgePos1 = this->splineChain.position_on_chain(t);
        const glm::vec3 edgePos2 = edgePos1 + this->base.size;
        const glm::vec3 diff1 = pos - edgePos1;
        const glm::vec3 diff2 = pos - edgePos2;
        if (dot(diff1, diff1) > this->sizeSquared || dot(diff2, diff2) > this->sizeSquared) {
            return std::nullopt;
        }

        // Calculate and return texture coordinates
        // x and z are the distance to the base spline, decomposed into components
        // y is currently assumed to be t
        const float xComp = dot(diff1, this->base.span1) / dot(this->base.span1, this->base.span1);
        const float zComp = dot(diff1, this->base.span2) / dot(this->base.span2, this->base.span2);
        return glm::vec3(xComp, t, zComp);
    }

    return std::nullopt;
}

void SplineMap::render() {
    this->base.render();
    this->topBase.render();
    this->splineChain.render();
    for (const SplineChain& edgeSpline : this->edgeSplines) {
        edgeSpline.render();
    }
}

void SplineMap::clean() {
    this->base.clean();
    this->splineChain.clean();
    this->topBase.clean();
    for (SplineChain& edgeSpline : this->edgeSplines) {
        edgeSpline.clean();
    }
}
