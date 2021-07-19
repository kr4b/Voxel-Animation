#pragma once

#include <glad/glad.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "plane.hpp"
#include "setup.hpp"
#include "shader.hpp"
#include "spline.hpp"
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
    alignas(16) glm::vec3 transformedMin;
    alignas(16) glm::vec3 transformedMax;

    alignas(16) glm::mat4x4 matrix;
    alignas(16) glm::mat4x4 inv_matrix;
};

struct SplineMapUniform
{
    alignas(16) AABBUniform aabb;
    alignas(16) PlaneUniform base;
    alignas(16) SplineUniform spline;
    alignas(16) SplineUniform transformedSpline;

    alignas(16) float size_squared;
    alignas(16) float width;
    alignas(16) float height;
};

class SplineMapScene {
public:
    SplineMapScene(
        Window &window,
        Setup& setup,
        Volume volume,
        Plane  base,
        glm::vec3 offset,
        glm::vec3 tangent0,
        glm::vec3 tangent1) :
        window(window),
        setup(setup),
        shader("assets/simple_vol.vert", "assets/spline_map.frag"),
        debugShader("assets/debug.vert", "assets/debug.frag"),
        volume(std::move(volume)),
        splineMap(base, Spline::with_tangents(glm::vec3(0.0f), offset, tangent0, tangent1)),
        offset(offset),
        tangent0(tangent0),
        tangent1(tangent1)
    {
        init();
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
    bool showAxis, showOutline;

    SplineMapUniform create_uniform() {
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
            SplineUniform {
                this->splineMap.spline.a,
                this->splineMap.spline.b,
                this->splineMap.spline.c,
                this->splineMap.spline.d
            },
            SplineUniform {
                this->splineMap.spline.transformedSpline->a,
                this->splineMap.spline.transformedSpline->b,
                this->splineMap.spline.transformedSpline->c,
                this->splineMap.spline.transformedSpline->d
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
        ImGui::PushID("Offset");
        ImGui::Text("Offset:");
        splineMapChange |= ImGui::DragFloat("x", &this->offset.x);
        splineMapChange |= ImGui::DragFloat("y", &this->offset.y);
        splineMapChange |= ImGui::DragFloat("z", &this->offset.z);
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
        ImGui::End();

        if (splineMapChange) {
            const Spline spline = Spline::with_tangents(glm::vec3(0.0f), this->offset, this->tangent0, this->tangent1);
            this->splineMap.clean();
            const SplineMap splineMap = SplineMap(this->splineMap.base, spline);
            std::memcpy(&this->splineMap, &splineMap, sizeof(SplineMap));
            const SplineMapUniform uniform = create_uniform();
            glNamedBufferSubData(this->splineMapUniform, 0, sizeof(SplineMapUniform), &uniform);
        }
    }

public:
    void update() {
        get_setup().update(get_window(), get_state());
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
            get_ray_emitter().render();
        }
        get_setup().end_render();
    }

    inline virtual State&      get_state()        { return state;       };
    inline virtual Window&     get_window()       { return window;      };
    inline virtual Setup&      get_setup()        { return setup;       };
    inline virtual Shader&     get_shader()       { return shader;      };
    inline virtual Shader&     get_debug_shader() { return debugShader; };
    inline virtual Volume&     get_volume()       { return volume;      };
    inline virtual SplineMap&  get_spline_map()   { return splineMap;   };
    inline virtual Axis&       get_axis()         { return axis;        };
    inline virtual RayEmitter& get_ray_emitter()  { return rayEmitter;  };

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
    glm::vec3  offset, tangent0, tangent1;
    float threshold = 0.25f;
    float stepSize = 0.025f;
};
