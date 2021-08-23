#include <algorithm>

#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "spline_map.hpp"

const BetterPlane createTopBase(const BetterPlane& base, const SplineChain& splineChain) {
    const glm::vec3 height = splineChain.position_on_chain(1.0f) - splineChain.position_on_chain(0.0f);
    return BetterPlane(base.point - height, base.span1, base.span2);
}

const SplineChain transformSplineChain(const BetterPlane& base, const SplineChain& splineChain) {
    return splineChain.transform(base.matrix);
}

SplineMap::SplineMap(const BetterPlane& base, const SplineChain& splineChain) :
    base(base),
    splineChain(transformSplineChain(this->base, splineChain)),
    topBase(createTopBase(this->base, splineChain)) {

    const glm::vec3 start = splineChain.splines[0].position_on_spline(0.0f);
    const glm::vec3 end = splineChain.splines[splineChain.length - 1].position_on_spline(1.0f);
    this->width = abs(length(this->base.span1));
    this->height = abs(length(end - start));
    this->depth = abs(length(this->base.span2));

    this->splineChain.init_vao();
    this->splineChain.update_buffers();
    this->splineChain.with_transform(this->base);
    this->topBase.init_vao(glm::vec3(0.0f, 0.2f, 0.3f));
    this->base.init_vao(glm::vec3(0.0f, 0.2f, 0.3f));

    edgeSplines[0] = this->splineChain.transform(glm::mat4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        this->base.span1.x,
        this->base.span1.y,
        this->base.span1.z,
        1.0f));
    edgeSplines[0].init_vao();
    edgeSplines[0].update_buffers();
    edgeSplines[1] = this->splineChain.transform(glm::mat4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        this->base.span2.x,
        this->base.span2.y,
        this->base.span2.z,
        1.0f));
    edgeSplines[1].init_vao();
    edgeSplines[1].update_buffers();
    edgeSplines[2] = this->splineChain.transform(glm::mat4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        this->base.span1.x + this->base.span2.x,
        this->base.span1.y + this->base.span2.y,
        this->base.span1.z + this->base.span2.z,
        1.0f));
    edgeSplines[2].init_vao();
    edgeSplines[2].update_buffers();
}

std::optional<glm::vec3> SplineMap::texture_coords(const glm::vec3 pos) const {
    const glm::vec4 pw = this->base.inv_matrix * glm::vec4(pos, 1.0f);
    const glm::vec3 p = glm::vec3(pw.x, pw.y, pw.z) / pw.w;
    std::optional<float> result = this->splineChain.intersect_spline_plane(p);

    if (result.has_value()) {
        const float t = result.value();
        const glm::vec3 edge = this->splineChain.position_on_transformed_chain(t);
        const glm::vec3 diff = edge - p;
        const float xComp = diff.x / this->width;
        const float yComp = p.y / this->height;
        const float zComp = diff.z / this->depth;
        return glm::vec3(xComp, yComp, zComp);
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
    this->topBase.clean();
    this->splineChain.clean();
    for (SplineChain& edgeSpline : this->edgeSplines) {
        edgeSpline.clean();
    }
}
