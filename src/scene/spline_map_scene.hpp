#pragma once

#define MAX_SPLINES 4

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <imgui.h>

#include <deque>

#include "../setup.hpp"
#include "../shader.hpp"
#include "../spline.hpp"
#include "../spline_map.hpp"
#include "../state.hpp"
#include "../volume.hpp"
#include "../window.hpp"
#include "debug_scene.hpp"
#include "ray_emitter_scene.hpp"

struct SplineUniform
{
    alignas(16) glm::vec3 a;
    alignas(16) glm::vec3 b;
    alignas(16) glm::vec3 c;
    alignas(16) glm::vec3 d;
};

struct PlaneUniform
{
    alignas(16) glm::vec3 point;
    alignas(16) glm::vec3 span1;
    alignas(16) glm::vec3 span2;
    alignas(16) glm::vec3 normal;

    alignas(16) glm::mat4x4 inv_matrix;
};

struct SplineMapUniform
{
    alignas(16) PlaneUniform base;
    alignas(16) PlaneUniform opposite_base;
    alignas(16) SplineUniform spline;
    alignas(16) SplineUniform opposite_spline;
    alignas(16) SplineUniform transformed_spline;
    alignas(16) glm::vec3 color;
    alignas(16) glm::vec3 size;

    float scale;
};

class SplineMapScene {
public:
    SplineMapScene(
        Window &window,
        Setup& setup,
        std::shared_ptr<Volume> volume,
        glm::ivec3 color,
        float rotation) :
        window(window),
        setup(setup),
        shader("assets/simple_vol.vert", "assets/spline_map.frag"),
        debugShader("assets/debug.vert", "assets/debug.frag"),
        volume(volume),
        translation(0.0f),
        tangents { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) },
        offset(0.0f, 2.0f, 0.0f),
        color(color),
        rotation(rotation),
        splineMap(
            Plane(
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(
                    float(volume->width()) / float(volume->height()) * 2.0f,
                    0.0f,
                    0.0f
                ),
                glm::vec3(
                    0.0f,
                    float(volume->depth()) / float(volume->height()) * 2.0f * sin(rotation),
                    float(volume->depth()) / float(volume->height()) * 2.0f * cos(rotation)
                )
            ),
            Spline::with_tangents(
                glm::vec3(0.0f), offset, tangents[0], tangents[1]
            )
        )
    {
        if (!this->volume->has_distance_field()) {
            this->volume->create_distance_field(this->threshold);
        }
        if (!this->volume->has_gradient_field()) {
            this->volume->create_gradient_field(this->threshold);
        }
        this->init();
    };

    ~SplineMapScene() {
        glDeleteBuffers(1, &this->splineMapUniform);
        this->splineMap.clean();
    }

protected:
    void init() {
        this->uniformData = create_uniform();

        glCreateBuffers(1, &splineMapUniform);
        glNamedBufferStorage(splineMapUniform, sizeof(SplineMapUniform), &this->uniformData, GL_DYNAMIC_STORAGE_BIT);
    };

private:
    bool showAxis = true;
    bool showOutline = true;
    bool animate = false;
    glm::vec3 translation;
    glm::vec3 tangents[2];
    // Offset of top base
    glm::vec3 offset;
    glm::ivec3 color;
    float rotation;
    float threshold = 0.25f; // stent: 0.0975f
    float stepSize = 0.025f;
    float stop0x = 0.2f;
    float stop1x = 0.617f;
    double time = 0.0;

    SplineMapUniform create_uniform() {
        // Find spline extremes
        std::vector<float> extremes = this->splineMap.spline.get_extremes();
        glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
        glm::vec3 max = glm::vec3(-std::numeric_limits<float>::max());
        for (const float t : extremes) {
            if (t >= 0.0f && t <= 1.0f) {
                const glm::vec3 pos = this->splineMap.spline.transformedSpline->position_on_spline(t);
                min = glm::min(min, pos);
                max = glm::max(max, pos);
            }
        }

        // Get minimum scale
        const float scale = glm::min(
            glm::min(
                this->splineMap.width / this->volume->width(),
                this->splineMap.height / this->volume->height()
            ),
            this->splineMap.depth / this->volume->depth()
        );

        return SplineMapUniform {
            PlaneUniform {
                this->splineMap.base.point,
                this->splineMap.base.span1,
                this->splineMap.base.span2,
                this->splineMap.base.normal,
                this->splineMap.base.inv_matrix,
            },
            PlaneUniform {
                this->splineMap.topBase.point,
                this->splineMap.topBase.span1,
                this->splineMap.topBase.span2,
                this->splineMap.topBase.normal,
                this->splineMap.topBase.inv_matrix,
            },
            SplineUniform {
                this->splineMap.spline.a,
                this->splineMap.spline.b,
                this->splineMap.spline.c,
                this->splineMap.spline.d
            },
            SplineUniform {
                this->splineMap.edgeSplines[2].a,
                this->splineMap.edgeSplines[2].b,
                this->splineMap.edgeSplines[2].c,
                this->splineMap.edgeSplines[2].d
            },
            SplineUniform {
                this->splineMap.spline.transformedSpline->a,
                this->splineMap.spline.transformedSpline->b,
                this->splineMap.spline.transformedSpline->c,
                this->splineMap.spline.transformedSpline->d
            },
            glm::vec3(this->color) / 255.0f,
            glm::vec3(this->splineMap.width, this->splineMap.height, this->splineMap.depth),
            scale,
        };
    }

    void show_ui() {
        bool splineMapChange = false;
        ImGui::Begin("Debug");
        ImGui::Checkbox("Show Axis", &this->showAxis);
        ImGui::Checkbox("Show Outline", &this->showOutline);
        ImGui::PushID("Points");
        ImGui::Separator();
        ImGui::Text("Points:");

        ImGui::Separator();
        ImGui::PopID();
        ImGui::PushID("Tangent0");
        ImGui::Text("Tangent 0:");
        splineMapChange |= ImGui::DragFloat("x", &this->tangents[0].x);
        splineMapChange |= ImGui::DragFloat("y", &this->tangents[0].y);
        splineMapChange |= ImGui::DragFloat("z", &this->tangents[0].z);
        ImGui::PopID();
        ImGui::PushID("Tangent1");
        ImGui::Text("Tangent 1:");
        splineMapChange |= ImGui::DragFloat("x", &this->tangents[1].x);
        splineMapChange |= ImGui::DragFloat("y", &this->tangents[1].y);
        splineMapChange |= ImGui::DragFloat("z", &this->tangents[1].z);
        ImGui::PopID();
        ImGui::Text("Color:");
        splineMapChange |= ImGui::SliderInt("R", &this->color.x, 0, 255);
        splineMapChange |= ImGui::SliderInt("G", &this->color.y, 0, 255);
        splineMapChange |= ImGui::SliderInt("B", &this->color.z, 0, 255);
        ImGui::Checkbox("Animate", &this->animate);
        ImGui::End();

        if (splineMapChange) {
            update_spline_map();
        }
    }

    void update_spline_map() {
        const Spline spline = Spline::with_tangents(glm::vec3(0.0f), this->offset, this->tangents[0], this->tangents[1]);
        this->splineMap.clean();
        SplineMap splineMap = SplineMap(this->splineMap.base, spline);
        std::memmove(&this->splineMap, &splineMap, sizeof(SplineMap));
        this->uniformData = create_uniform();
    }

public:
    void update() {
        get_state().translateCamera(-this->translation);
        get_setup().update(get_window(), get_state());
        get_state().translateCamera(this->translation);

        // Smoothly animate spline tangents
        if (this->animate) {
            this->time += get_window().get_delta_time() * 0.001;
            this->tangents[0] = glm::vec3(float(sin(this->time)) * 3.0f, 0.0f, 0.0f);
            this->tangents[1] = glm::vec3(float(cos(this->time)) * 3.0f, 0.0f, 0.0f);
            update_spline_map();
        }

        if (get_state().debugMode) {
            get_ray_emitter().update(get_setup(), get_state(), get_spline_map(), *(this->volume), &this->threshold, &this->stepSize);
            if (get_state().showUi) {
                show_ui();
            }
        }
    }

    void render() {
        get_shader().use();
        glNamedBufferSubData(this->splineMapUniform, 0, sizeof(SplineMapUniform), &this->uniformData);
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, splineMapUniform);
        get_shader().uniformFloat("threshold", this->threshold);
        get_shader().uniformFloat("step_size", this->stepSize);
        get_shader().uniformFloat("stop0x", this->stop0x);
        get_shader().uniformFloat("stop1x", this->stop1x);
        this->volume->bind();
        get_setup().start_render();
        if (get_state().debugMode) {
            get_debug_shader().use();
            get_setup().debug(get_debug_shader());
            if (this->showAxis) get_axis().render(get_setup());
            if (this->showOutline) get_spline_map().render();
            get_ray_emitter().render();
        }
        get_setup().end_render();
    }

    void translate(glm::vec3 translation) {
        this->translation += translation;
    }

    void set_state(State state) { this->state = state; }

    inline virtual State&           get_state()             { return state;        };
    inline virtual Window&          get_window()            { return window;       };
    inline virtual Setup&           get_setup()             { return setup;        };
    inline virtual Shader&          get_shader()            { return shader;       };
    inline virtual Shader&          get_debug_shader()      { return debugShader;  };
    inline virtual SplineMap&       get_spline_map()        { return splineMap;    };
    inline virtual Axis&            get_axis()              { return axis;         };
    inline virtual RayEmitter&      get_ray_emitter()       { return rayEmitter;   };
    inline virtual std::shared_ptr<Volume> get_volume()     { return volume;       };
    inline virtual glm::ivec3       get_color()             { return color;        }
    inline virtual float            get_rotation()          { return rotation;     };

protected:
    SplineMapUniform uniformData;
    GLuint     splineMapUniform;
    State      state;
    Window&    window;
    Setup&     setup;
    Shader     shader, debugShader;
    std::shared_ptr<Volume> volume;
    SplineMap  splineMap;
    Axis       axis;
    RayEmitter rayEmitter;
};
