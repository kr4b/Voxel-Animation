#pragma once

#define MAX_SPLINES 4

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
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
    alignas(16) glm::vec3 y_bounds;
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
    float width;
    float height;
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
            SplineChain::from_points_with_tangents(
                std::vector<glm::vec3>(this->anchorPoints, this->anchorPoints + 2),
                std::vector<glm::vec3>(this->anchorTangents, this->anchorTangents + 2)
            )
        ),
        anchorPoints{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f) },
        anchorTangents{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) },
        tau(0.2f),
        wireframeAABB(WireframeAABB(this->splineMap.aabb, glm::vec3(0.90, 0.49, 0.13)))
    {
        this->init();
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
    glm::vec3 anchorPoints[2], anchorTangents[2];
    std::vector<glm::vec2> middlePoints;
    std::vector<glm::vec3> middleTangents;
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

        glm::vec3 yBounds = glm::vec3(
            this->splineMap.splineChain.splines.front().position_on_spline(0.0f).y,
            this->splineMap.splineChain.splines.back().position_on_spline(1.0f).y,
            0.0f
        );
        yBounds.z =  float(this->splineMap.splineChain.splines.size()) / (yBounds.y - yBounds.x);

        glm::vec3 transformedYBounds = glm::vec3(
            this->splineMap.splineChain.splines.front().transformedSpline->position_on_spline(0.0f).y,
            this->splineMap.splineChain.splines.back().transformedSpline->position_on_spline(1.0f).y,
            0.0f
        );
        transformedYBounds.z =  float(this->splineMap.splineChain.splines.size()) / (transformedYBounds.y - transformedYBounds.x);

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
                splineUniforms[3],
                float(this->splineMap.splineChain.splines.size()),
                yBounds
            },
            SplineChainUniform {
                transformedSplineUniforms[0],
                transformedSplineUniforms[1],
                transformedSplineUniforms[2],
                transformedSplineUniforms[3],
                float(this->splineMap.splineChain.splines.size()),
                transformedYBounds
            },
            this->splineMap.sizeSquared,
            this->splineMap.sizeSquared,
            yBounds.x - yBounds.y
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

        ImGui::Text("Last Point:");
        splineMapChange |= ImGui::DragFloat3("xyz", glm::value_ptr(this->anchorPoints[1]));

        for (int i = 0; i < this->middlePoints.size(); i++) {
            ImGui::PushID(i);
            splineMapChange |= ImGui::DragFloat2("pos", glm::value_ptr(this->middlePoints[i]));
            splineMapChange |= ImGui::DragFloat3("tan", glm::value_ptr(this->middleTangents[i]));
            ImGui::PopID();
        }

        if (ImGui::Button("Push")) {
            if (this->splineMap.splineChain.splines.size() < MAX_SPLINES) {
                this->middlePoints.push_back(glm::vec2());
                this->middleTangents.push_back(glm::vec3());
                splineMapChange = true;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Pop")) {
            if (this->middlePoints.size() > 0) {
                this->middlePoints.pop_back();
                this->middleTangents.pop_back();
                splineMapChange = true;
            }
        }
        ImGui::Separator();
        ImGui::PopID();
        ImGui::PushID("Tangent0");
        ImGui::Text("Tangent 0:");
        splineMapChange |= ImGui::DragFloat("x", &this->anchorTangents[0].x);
        splineMapChange |= ImGui::DragFloat("y", &this->anchorTangents[0].y);
        splineMapChange |= ImGui::DragFloat("z", &this->anchorTangents[0].z);
        ImGui::PopID();
        ImGui::PushID("Tangent1");
        ImGui::Text("Tangent 1:");
        splineMapChange |= ImGui::DragFloat("x", &this->anchorTangents[1].x);
        splineMapChange |= ImGui::DragFloat("y", &this->anchorTangents[1].y);
        splineMapChange |= ImGui::DragFloat("z", &this->anchorTangents[1].z);
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
        std::vector<glm::vec3> points;
        std::vector<glm::vec3> tangents;

        const float deltaY = (anchorPoints[1].y - anchorPoints[0].y) / float(middlePoints.size() + 1);
        points.push_back(anchorPoints[0]);
        tangents.push_back(anchorTangents[0]);
        for (int i = 0; i < middlePoints.size(); i++) {
            points.push_back(glm::vec3(middlePoints[i].x, anchorPoints[0].y + deltaY * float(i + 1), middlePoints[i].y));
            tangents.push_back(middleTangents[i]);
        }
        points.push_back(anchorPoints[1]);
        tangents.push_back(anchorTangents[1]);

        const SplineChain splineChain = SplineChain::from_points_with_tangents(points, tangents);
        this->splineMap.clean();
        const SplineMap splineMap = SplineMap(this->splineMap.base, splineChain);
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
            this->anchorTangents[0] = glm::vec3(float(sin(this->time)) * 3.0f, 0.0f, 0.0f);
            this->anchorTangents[1] = glm::vec3(float(cos(this->time)) * 3.0f, 0.0f, 0.0f);
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
