#include <algorithm>

#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "spline_map.hpp"

const Plane createTopBase(const Plane& base, const Spline& spline) {
    return Plane(spline.position_on_spline(1.0f), base.span1, base.span2);
}

const Spline transformSpline(const Plane& base, const Spline& spline) {
    return spline.transform(base.matrix);
}

SplineMap::SplineMap(const Plane& base, const Spline& spline) :
    base(base),
    spline(transformSpline(base, spline)),
    topBase(createTopBase(base, this->spline)) {

    // Initialize spline
    this->spline.init_vao();
    this->spline.update_buffers();
    this->spline.with_transform(this->base);

    // Calculated axis aligned spline starting and ending points
    const glm::vec3 start = this->spline.transformedSpline->position_on_spline(0.0f);
    const glm::vec3 end = this->spline.transformedSpline->position_on_spline(1.0f);

    // Calculate width, height and depth
    // Where the width and depth are of each base and the height is the distance between the bases
    this->width = abs(length(this->base.span1));
    this->height = glm::distance(start, end);
    this->depth = abs(length(this->base.span2));

    // Init some other stuff
    this->topBase.init_vao(glm::vec3(0.0f, 0.2f, 0.3f));
    this->base.init_vao(glm::vec3(0.0f, 0.2f, 0.3f));

    edgeSplines[0] = this->spline.transform(glm::mat4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        this->base.span1.x,
        this->base.span1.y,
        this->base.span1.z,
        1.0f));
    edgeSplines[0].init_vao();
    edgeSplines[0].update_buffers();
    edgeSplines[1] = this->spline.transform(glm::mat4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        this->base.span2.x,
        this->base.span2.y,
        this->base.span2.z,
        1.0f));
    edgeSplines[1].init_vao();
    edgeSplines[1].update_buffers();
    edgeSplines[2] = this->spline.transform(glm::mat4x4(
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
    std::optional<float> result = this->spline.intersect_spline_plane(p);

    if (result.has_value()) {
        const float t = result.value();
        const glm::vec3 edge = this->spline.transformedSpline->position_on_spline(t);
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
    this->spline.render();
    for (const Spline& edgeSpline : this->edgeSplines) {
        edgeSpline.render();
    }
}

void SplineMap::clean() {
    this->base.clean();
    this->topBase.clean();
    this->spline.clean();
    for (Spline& edgeSpline : this->edgeSplines) {
        edgeSpline.clean();
    }
}
