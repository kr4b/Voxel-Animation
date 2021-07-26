#pragma once

#define MAX_SPLINES 3

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <deque>

#include "plane.hpp"
#include "setup.hpp"
#include "shader.hpp"
#include "spline.hpp"
#include "spline_chain.hpp"
#include "spline_map.hpp"
#include "state.hpp"
#include "volume.hpp"
#include "window.hpp"
#include "debug_scene.hpp"
#include "ray_emitter_scene.hpp"

struct AABBUniform
{
    alignas(16) glm::vec3 min;
    alignas(16) glm::vec3 max;
};

struct SplineUniform
{
    alignas(16) glm::vec3 a;
    alignas(16) glm::vec3 b;
    alignas(16) glm::vec3 c;
    alignas(16) glm::vec3 d;
};

struct SplineChainUniform
{
    alignas(16) SplineUniform splines[MAX_SPLINES];
    alignas(16) float amount;
};

struct PlaneUniform
{
    alignas(16) glm::vec3 center;
    alignas(16) glm::vec3 half_size;
    alignas(16) glm::vec3 size;
    alignas(16) glm::vec3 normal;
    alignas(16) glm::vec3 span1;
    alignas(16) glm::vec3 span2;
    alignas(16) glm::vec3 min;
    alignas(16) glm::vec3 max;
    alignas(16) glm::vec3 transformed_min;
    alignas(16) glm::vec3 transformed_max;

    alignas(16) glm::mat4x4 matrix;
    alignas(16) glm::mat4x4 inv_matrix;
};

struct SplineMapUniform
{
    alignas(16) AABBUniform aabb;
    alignas(16) PlaneUniform base;
    alignas(16) SplineChainUniform spline_chain;
    alignas(16) SplineChainUniform transformed_spline_chain;

    alignas(16) float size_squared;
    alignas(16) float width;
    alignas(16) float height;
};

class SplineMapScene {
public:
    SplineMapScene(
        Window &window,
        Setup& setup,
        Volume volume) :
        window(window),
        setup(setup),
        shader("assets/simple_vol.vert", "assets/spline_map.frag"),
        debugShader("assets/debug.vert", "assets/debug.frag"),
        volume(std::move(volume)),
        splineMap(
            Plane(
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec3(1.0f, 0.0f, 1.0f)
            ),
            SplineChain::from_points_with_outer_tangents(
                tangent0,
                tangent1,
                points,
                tau
            )
        ),
        points(std::deque<glm::vec3>(2, glm::vec3(0.0f))),
        tangent0(0.0f, 0.0f, 0.0f),
        tangent1(0.0f, 0.0f, 0.0f),
        tau(0.2f),
        wireframeAABB(WireframeAABB(this->splineMap.aabb, glm::vec3(0.90, 0.49, 0.13)))
    {
        this->init();
        this->points.back().y = 2.0f;
        this->update_spline_map();
    };

    ~SplineMapScene() {
        glDeleteBuffers(1, &this->splineMapUniform);
        this->splineMap.clean();
    }

protected:
    void init() {
        const SplineMapUniform uniform = create_uniform();

        glCreateBuffers(1, &splineMapUniform);
        glNamedBufferStorage(splineMapUniform, sizeof(SplineMapUniform), &uniform, GL_DYNAMIC_STORAGE_BIT);
    };

private:
    bool showAxis = true;
    bool showOutline = true;
    bool showEncompassingAABB = true;
    bool animate = false;
    glm::vec3 tangent0, tangent1;
    std::deque<glm::vec3> points;
    float tau;
    float threshold = 0.25f;
    float stepSize = 0.025f;
    double time = 0.0;

    SplineMapUniform create_uniform() {
        SplineUniform splineUniforms[MAX_SPLINES];
        SplineUniform transformedSplineUniforms[MAX_SPLINES];

        for (int i = 0; i < this->splineMap.splineChain.splines.size(); i++) {
            const Spline &spline = this->splineMap.splineChain.splines[i];

            splineUniforms[i] = SplineUniform {
                spline.a,
                spline.b,
                spline.c,
                spline.d
            };

            transformedSplineUniforms[i] = SplineUniform {
                spline.transformedSpline->a,
                spline.transformedSpline->b,
                spline.transformedSpline->c,
                spline.transformedSpline->d
            };
        }

        return SplineMapUniform {
            AABBUniform {
                this->splineMap.aabb.min,
                this->splineMap.aabb.max
            },
            PlaneUniform {
                this->splineMap.base.center,
                this->splineMap.base.half_size,
                this->splineMap.base.size,
                this->splineMap.base.normal,
                this->splineMap.base.span1,
                this->splineMap.base.span2,
                this->splineMap.base.min,
                this->splineMap.base.max,
                this->splineMap.base.transformedMin,
                this->splineMap.base.transformedMax,
                this->splineMap.base.matrix,
                this->splineMap.base.inv_matrix
            },
            SplineChainUniform {
                splineUniforms[0],
                splineUniforms[1],
                splineUniforms[2],
                // splineUniforms[3],
                // splineUniforms[4],
                float(this->splineMap.splineChain.splines.size())
            },
            SplineChainUniform {
                transformedSplineUniforms[0],
                transformedSplineUniforms[1],
                transformedSplineUniforms[2],
                // transformedSplineUniforms[3],
                // transformedSplineUniforms[4],
                float(this->splineMap.splineChain.splines.size())
            },
            this->splineMap.sizeSquared,
            this->splineMap.sizeSquared,
            this->splineMap.sizeSquared
        };
    }

    void show_ui() {
        bool splineMapChange = false;
        ImGui::Begin("Debug");
        ImGui::Checkbox("Show Axis", &this->showAxis);
        ImGui::Checkbox("Show Outline", &this->showOutline);
        ImGui::Checkbox("Show Encompassing AABB", &this->showEncompassingAABB);
        ImGui::PushID("Points");
        ImGui::Separator();
        ImGui::Text("Points:");
        ImGui::BeginTable("PointsTable", 1);
        for (int i = 0; i < this->points.size(); i++) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            splineMapChange |= ImGui::DragFloat3(("Point: " + std::to_string(i)).c_str(), glm::value_ptr(this->points[i]));
        }
        ImGui::EndTable();

        if (ImGui::Button("Push")) {
            if (this->splineMap.splineChain.splines.size() < MAX_SPLINES) {
                glm::vec3 next = glm::vec3(this->points.back());
                next.y += 1.0f;
                this->points.push_back(next);
                splineMapChange = true;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Pop")) {
            if (this->points.size() > 2) {
                this->points.pop_back();
                splineMapChange = true;
            }
        }
        ImGui::Separator();
        ImGui::PopID();
        ImGui::PushID("Tangent0");
        ImGui::Text("Tangent 0:");
        splineMapChange |= ImGui::DragFloat("x", &this->tangent0.x);
        splineMapChange |= ImGui::DragFloat("y", &this->tangent0.y);
        splineMapChange |= ImGui::DragFloat("z", &this->tangent0.z);
        ImGui::PopID();
        ImGui::PushID("Tangent1");
        ImGui::Text("Tangent 1:");
        splineMapChange |= ImGui::DragFloat("x", &this->tangent1.x);
        splineMapChange |= ImGui::DragFloat("y", &this->tangent1.y);
        splineMapChange |= ImGui::DragFloat("z", &this->tangent1.z);
        ImGui::PopID();
        ImGui::PushID("Tau");
        splineMapChange |= ImGui::DragFloat("Tau", &this->tau, 0.1f, 0.0f, 1.0f);
        ImGui::PopID();
        ImGui::Checkbox("Animate", &this->animate);
        ImGui::End();

        if (splineMapChange) {
            update_spline_map();
        }
    }

    void update_spline_map() {
        const SplineChain spline = SplineChain::from_points_with_outer_tangents(this->tangent0, this->tangent1, this->points, this->tau);
        this->splineMap.clean();
        const SplineMap splineMap = SplineMap(this->splineMap.base, spline);
        std::memcpy(&this->splineMap, &splineMap, sizeof(SplineMap));
        const SplineMapUniform uniform = create_uniform();
        glNamedBufferSubData(this->splineMapUniform, 0, sizeof(SplineMapUniform), &uniform);

        get_encompassing_aabb().update(this->splineMap.aabb);
    }

public:
    void update() {
        get_setup().update(get_window(), get_state());

        if (this->animate) {
            this->time += get_window().get_delta_time() * 0.001;
            this->tangent0 = glm::vec3(float(sin(this->time)) * 3.0f, 0.0f, 0.0f);
            this->tangent1 = glm::vec3(float(cos(this->time)) * 3.0f, 0.0f, 0.0f);
            update_spline_map();
        }
        if (get_state().debugMode) {
            get_ray_emitter().update(get_setup(), get_state(), get_spline_map(), get_volume(), &this->threshold, &this->stepSize);
            show_ui();
        }
    }

    void render() {
        get_shader().use();
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, splineMapUniform);
        get_shader().uniformFloat("threshold", this->threshold);
        get_shader().uniformFloat("step_size", this->stepSize);
        get_volume().bind();
        get_setup().start_render(get_shader());
        if (get_state().debugMode) {
            get_debug_shader().use();
            get_setup().debug(get_debug_shader());
            if (this->showAxis) get_axis().render(get_setup());
            if (this->showOutline) get_spline_map().render();
            if (this->showEncompassingAABB) get_encompassing_aabb().render(get_setup());
            get_ray_emitter().render();
        }
        get_setup().end_render();
    }

    inline virtual State&           get_state()             { return state;         };
    inline virtual Window&          get_window()            { return window;        };
    inline virtual Setup&           get_setup()             { return setup;         };
    inline virtual Shader&          get_shader()            { return shader;        };
    inline virtual Shader&          get_debug_shader()      { return debugShader;   };
    inline virtual Volume&          get_volume()            { return volume;        };
    inline virtual SplineMap&       get_spline_map()        { return splineMap;     };
    inline virtual Axis&            get_axis()              { return axis;          };
    inline virtual RayEmitter&      get_ray_emitter()       { return rayEmitter;    };
    inline virtual WireframeAABB&   get_encompassing_aabb() { return wireframeAABB; };

protected:
    GLuint     splineMapUniform;
    State      state;
    Window&    window;
    Setup&     setup;
    Shader     shader, debugShader;
    Volume     volume;
    SplineMap  splineMap;
    Axis       axis;
    RayEmitter rayEmitter;
    WireframeAABB wireframeAABB;
};
