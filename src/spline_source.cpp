#include "spline_source.hpp"

void SplineSource::update_from_screen_coords(const vec2f coords, const mat44f inverseProjCamera, const vec3f cameraWorldPos) {
    this->coords = coords;
    this->inverseProjCamera = inverseProjCamera;
    this->cameraWorldPos = cameraWorldPos;
}

void SplineSource::update_from_screen_coords(const vec3f tangent1, const vec3f tangent2) {
    const vec2f startCoords = this->coords - fml::make_splat<vec2f>(splineDist * (splineCount / 2));

    for (size_t i = 0; i < splineCount; i++) {
        vec2f currentCoords = startCoords + fml::make_vector<vec2f>(0.0f, float(i) * splineDist);
        for (size_t j = 0; j < splineCount; j++) {
            splines[i * splineCount + j].update_from_screen_coords(currentCoords, this->inverseProjCamera, this->cameraWorldPos, tangent1, tangent2);
            currentCoords.x += splineDist;
        }
    }
}

void SplineSource::render(const gl::GLapi* gl) {
    for (const Spline& spline : splines) {
        spline.render(gl);
    }
}

void SplineSource::update_buffers(const gl::GLapi* gl) {
    for (Spline& spline : splines) {
        spline.update_buffers(gl);
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
    splines.clear();
    for (size_t i = 0; i < this->splineCount; i++) {
        for (size_t j = 0; j < this->splineCount; j++) {
            splines.push_back(Spline(gl));
        }
    }
}