#include "spline_chain.hpp"
#include <cassert>

SplineChain::SplineChain() {
    this->splines.push_back(Spline());
}

SplineChain::SplineChain(std::vector<Spline> splines): splines(splines) {}

void SplineChain::init_vao() {
    for (Spline& spline : this->splines) {
        spline.init_vao();
    }
}

void SplineChain::render() const {
    for (int i = 0; i < this->splines.size(); i++) {
        this->splines[i].render();
    }
}

void SplineChain::update_buffers() {
    for (Spline& spline : this->splines) {
        spline.update_buffers();
    }
}

void SplineChain::clean() {
    for (Spline& spline : this->splines) {
        spline.clean();
    }
}

SplineChain SplineChain::from_points(std::deque<glm::vec3> points, float tau) {
    return SplineChain::from_points_with_outer_tangents(
        glm::vec3(0.0f),
        glm::vec3(0.0f),
        points,
        tau
    );
}

SplineChain SplineChain::from_points_with_outer_tangents(glm::vec3 first_tangent, glm::vec3 last_tangent, std::deque<glm::vec3> points, float tau) {
    assert(points.size() > 1);
    std::vector<Spline> splines;

    points.push_front(points.front() - first_tangent);
    points.push_back(points.back() + last_tangent);

    for (int i = 1; i < points.size() - 2; i++) {
        splines.push_back(
            Spline::with_control_points(
                points[i],
                points[i + 1],
                points[i - 1],
                points[i + 2],
                tau
            )
        );
    }

    return SplineChain(splines);
}

SplineChain SplineChain::from_points_with_tangents(std::vector<glm::vec3> points, std::vector<glm::vec3> tangents) {
    assert(points.size() > 1);
    std::vector<Spline> splines;

    for (int i = 1; i < points.size(); i++) {
        splines.push_back(
            Spline::with_tangents(
                points[i - 1],
                points[i],
                tangents[i - 1],
                tangents[i]
            )
        );
    }

    return SplineChain(splines);
}

glm::vec3 SplineChain::position_on_chain(const float t) const {
    const float clamped_t = std::max(0.0f, std::min(t, 1.0f - 1e-4f));
    const unsigned int index = (unsigned int) floor(clamped_t * float(this->splines.size()));
    const float t_prime = (clamped_t - float(index) / float(this->splines.size())) * float(this->splines.size());

    return this->splines[index].position_on_spline(t_prime);
}

std::vector<float> SplineChain::get_extremes() const {
    std::vector<float> result;

    for (int i = 0; i < this->splines.size(); i++) {
        std::vector<float> temp = this->splines[i].get_extremes();
        for (float t : temp) {
            result.push_back((t + float(i)) / this->splines.size());
        }
    }

    return result;
}

std::optional<float> SplineChain::intersect_spline_plane(const glm::vec3 p) const {
    for (int i = 0; i < this->splines.size(); i++) {
        std::optional<float> result = this->splines[i].intersect_spline_plane(p);
        if (result.has_value()) {
            return (result.value() + i) / this->splines.size();
        }
    }

    return std::nullopt;
}

std::optional<float> SplineChain::intersect_spline_plane(const BetterPlane& p) const {
    for (int i = 0; i < this->splines.size(); i++) {
        std::optional<float> result = this->splines[i].intersect_spline_plane(p);
        if (result.has_value()) {
            return (result.value() + i) / this->splines.size();
        }
    }

    return std::nullopt;
}

void SplineChain::with_transform(const Plane& plane) {
    for (Spline& spline : this->splines) {
        spline.with_transform(plane);
    }
}
