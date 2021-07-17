#pragma once

#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/quaternion.hpp>

#include "ray.hpp"
#include "state.hpp"
#include "spline_map.hpp"
#include "volume.hpp"

class RayEmitter {
public:
    void update(const Setup& setup, State& state, const SplineMap& splineMap, const Volume& volume) {
        this->show_ui();
        this->emit_rays(setup, state, splineMap, volume);
    }

    void render() const {
        for (const Ray& ray : rays) {
            ray.render(this->showRays, this->showIntersections);
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
    bool showRays, showIntersections;
    bool refreshRays;
    int rayCount = 1;
    float rayGap = 0.05f;

    void emit_rays(const Setup& setup, State& state, const SplineMap& splineMap, const Volume& volume) {
        if (!state.refreshRayEmitter && !this->refreshRays) {
            return;
        }
        this->clean();

        const glm::mat4 view = glm::translate(glm::mat4(1.0f), state.lastCameraOff) * glm::toMat4(state.lastCameraRot);
        const size_t raysSqrt = this->rayCount * 2 - 1;
        const glm::vec2 eyePos =
            glm::vec2(state.lastX, 1.0f / setup.get_reciprocal_window_size().y - state.lastY) * setup.get_reciprocal_window_size();
        const glm::vec2 startPos = eyePos - this->rayGap * float(raysSqrt / 2);
        const glm::vec4 cameraWorldPos = glm::inverse(view) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        const glm::vec3 origin = glm::vec3(cameraWorldPos.x, cameraWorldPos.y, cameraWorldPos.z) / cameraWorldPos.w;

        for (size_t i = 0; i < raysSqrt; i++) {
            glm::vec2 currentPos = startPos + glm::vec2(0.0f, float(i) * this->rayGap);
            for (size_t j = 0; j < raysSqrt; j++) {
                const glm::vec2 transformed = currentPos * 2.0f - 1.0f;
                const glm::vec4 hray = glm::vec4(transformed, 1.0f, 1.0f);
                const glm::vec4 wray = glm::inverse(setup.get_proj() * view) * hray;

                const glm::vec3 direction =
                    glm::normalize(glm::vec3(wray.x, wray.y, wray.z) / wray.w - origin) * glm::length(origin) * 2.0f;

                Ray ray(cameraWorldPos, direction);
                ray.init_vao();
                ray.update_buffers(ray.walk_spline_map(splineMap, volume, 0.025f), glm::ivec3(volume.width(), volume.height(), volume.depth()));
                rays.push_back(ray);
                currentPos.x += this->rayGap;
            }
        }

        this->refreshRays = false;
        state.refreshRayEmitter = false;
    }

    void show_ui() {
        ImGui::Begin("Ray Emitter");
        ImGui::Checkbox("Show Rays", &this->showRays);
        ImGui::Checkbox("Show Intersections", &this->showIntersections);
        this->refreshRays |= ImGui::SliderFloat("Gap", &this->rayGap, 0.01f, 0.1f);
        this->refreshRays |= ImGui::SliderInt("Rays", &this->rayCount, 1, 100);
        ImGui::End();
    }
};