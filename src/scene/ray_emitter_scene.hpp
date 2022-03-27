#pragma once

#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../ray.hpp"
#include "../state.hpp"
#include "../spline_map.hpp"
#include "../volume.hpp"
#include "../plane.hpp"

class RayEmitter {
public:
    void update(const Setup& setup, State& state, SplineMap& splineMap, Volume& volume, float* threshold, float* stepSize) {
        this->show_ui(threshold, stepSize);
        this->emit_rays(setup, state, splineMap, volume, *threshold, *stepSize);
    }

    void render() const {
        for (const Ray& ray : rays) {
            ray.render(this->showRays, this->showIntersections);

            // Plane plane0(ray.origin + ray.dir * (-ray.origin.x / ray.dir.x), glm::vec3(0.0f, ray.dir.y, ray.dir.z), glm::vec3(1.0f, 0.0f, 0.0f));
            // plane0.init_vao(glm::vec3(0.7f, 0.3f, 0.4f));
            // plane0.render();
            // plane0.clean();

            // Plane plane1(ray.origin + ray.dir * (-ray.origin.z / ray.dir.z), glm::vec3(ray.dir.x, ray.dir.y, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            // plane1.init_vao(glm::vec3(0.3f, 0.7f, 0.4f));
            // plane1.render();
            // plane1.clean();
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
    bool showRays = false;
    bool showIntersections = true;
    bool refreshRays = false;
    bool thresholdChange = false;
    bool thresholdActive = false;
    int rayCount = 1;
    float rayGap = 0.05f;

    // Uniformly emit rays from the eye
    void emit_rays(const Setup& setup, State& state, const SplineMap& splineMap, Volume& volume, float threshold, float stepSize) {
        if (!state.refreshRayEmitter && !this->refreshRays) {
            return;
        }

        this->clean();
        if (this->thresholdChange) {
            volume.create_distance_field(threshold);
            this->thresholdChange = false;
        }

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
                ray.update_buffers(ray.intersect_ray_spline_map(splineMap));
                rays.push_back(ray);
                currentPos.x += this->rayGap;
            }
        }

        this->refreshRays = false;
        state.refreshRayEmitter = false;
    }

    void show_ui(float* threshold, float* stepSize) {
        ImGui::Begin("Ray Emitter");
        ImGui::Checkbox("Show Rays", &this->showRays);
        ImGui::Checkbox("Show Intersections", &this->showIntersections);
        this->refreshRays |= ImGui::SliderFloat("Gap", &this->rayGap, 0.01f, 0.1f);
        this->refreshRays |= ImGui::SliderInt("Rays", &this->rayCount, 1, 100);
        bool thresholdActive = ImGui::SliderFloat("Threshold", threshold, 0.0f, 1.0f);
        this->refreshRays |= ImGui::SliderFloat("Step size", stepSize, 1e-3f, 1.0f);
        ImGui::End();

        if (this->thresholdActive && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            this->refreshRays = true;
            this->thresholdChange = true;
            this->thresholdActive = false;
        }
        this->thresholdActive |= thresholdActive;
    }
};