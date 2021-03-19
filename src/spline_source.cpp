#include "spline_source.hpp"

void SplineSource::update_from_screen_coords(const vec2f coords, const mat44f inverseProjCamera, const vec3f cameraWorldPos) {
    this->coords = coords;
    this->inverseProjCamera = inverseProjCamera;
    this->cameraWorldPos = cameraWorldPos;
}

void SplineSource::render(bool displayRays, const gl::GLapi* gl) {
    for (const Spline& spline : splines) {
        spline.render(gl);
    }

    if (displayRays) {
        for (Ray& ray : rays) {
            ray.render(gl);
        }
    }
}

void SplineSource::update_buffers(const gl::GLapi* gl) {
    for (Spline& spline : splines) {
        spline.update_buffers(gl);
    }

    for (Ray& ray : rays) {
        ray.update_buffers(gl);
    }
}

void SplineSource::intersect_spline_aabb(const vec3f aAABBMin, const vec3f aAABBMax) {
    for (Spline& spline : splines) {
        spline.intersect_spline_aabb(aAABBMin, aAABBMax);
    }
}

void SplineSource::set_values(const gl::GLapi* gl, const size_t splineCount, const float splineDist) {
    this->splineCount = splineCount * 2 - 1;
    this->splineDist = splineDist;

    this->clean(gl);
}

void SplineSource::clean(const gl::GLapi* gl) {
    for (size_t i = 0; i < this->rays.size(); i++) {
        this->rays[i].clean(gl);
    }

    for (size_t i = 0; i < this->splines.size(); i++) {
        this->splines[i].clean(gl);
    }

    this->rays.clear();
    this->splines.clear();
}