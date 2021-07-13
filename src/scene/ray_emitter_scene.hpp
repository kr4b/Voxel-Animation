#pragma once

#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "ray.hpp"
#include "state.hpp"
#include "spline_map.hpp"
#include "volume.hpp"

class RayEmitter {
public:
    void update(const Setup& setup, State& state, const SplineMap& splineMap, const Volume& volume) {
        if (!state.refreshRayEmitter) {
            return;
        }
        this->clean();

        const size_t raysSqrt = state.rayCount * 2 - 1;
        const glm::vec2 eyePos =
            glm::vec2(state.lastX, 1.0f / setup.get_reciprocal_window_size().y - state.lastY) * setup.get_reciprocal_window_size();
        const glm::vec2 startPos = eyePos - state.rayGap * float(raysSqrt / 2);
        const glm::vec4 cameraWorldPos = glm::inverse(setup.get_view()) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        const glm::vec3 origin = glm::vec3(cameraWorldPos.x, cameraWorldPos.y, cameraWorldPos.z) / cameraWorldPos.w;

        for (size_t i = 0; i < raysSqrt; i++) {
            glm::vec2 currentPos = startPos + glm::vec2(0.0f, float(i) * state.rayGap);
            for (size_t j = 0; j < raysSqrt; j++) {
                const glm::vec2 transformed = currentPos * 2.0f - 1.0f;
                const glm::vec4 hray = glm::vec4(transformed, 1.0f, 1.0f);
                const glm::vec4 wray = setup.get_inv_proj_view() * hray;

                const glm::vec3 direction =
                    glm::normalize(glm::vec3(wray.x, wray.y, wray.z) / wray.w - origin) * glm::length(origin) * 2.0f;

                Ray ray(cameraWorldPos, direction);
                ray.init_vao();
                ray.update_buffers(ray.walk_spline_map(splineMap, volume, 0.025f), glm::ivec3(volume.width(), volume.height(), volume.depth()));
                rays.push_back(ray);
                currentPos.x += state.rayGap;
            }
        }

        state.refreshRayEmitter = false;
    }

    void render() const {
        for (const Ray& ray : rays) {
            ray.render();
        }
    }

    void clean() {
        for (Ray& ray : rays) {
            ray.clean();
        }
        rays.clear();
    }

private:
    std::vector<Ray> rays;
};