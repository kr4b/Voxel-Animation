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

    alignas(16) glm::mat4x4 matrix;
    alignas(16) glm::mat4x4 inv_matrix;
};

struct SplineMapUniform
{
    alignas(16) AABBUniform aabb;
    alignas(16) PlaneUniform base;
    alignas(16) SplineUniform spline;

    alignas(16) float size_squared;
    alignas(16) float width;
    alignas(16) float height;
};

class SplineMapScene {
public:
    SplineMapScene(
        Window &window,
        State  &state,
        Volume &volume,
        Plane  base,
        Spline spline) :
        state(state),
        window(window),
        shader("assets/simple_vol.vert", "assets/spline_map.frag"),
        debugShader("assets/debug.vert", "assets/debug.frag"),
        volume(volume),
        base(base),
        spline(spline),
        splineMap(this->base, this->spline)
    {
        init();
    };

    ~SplineMapScene() {
        glDeleteBuffers(1, &this->splineMapUniform);
        this->spline.clean();
        this->base.clean();
        this->splineMap.clean();
    }

protected:
    void init() {
        SplineMapUniform uniform = SplineMapUniform {
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
                this->splineMap.base.matrix,
                this->splineMap.base.inv_matrix
            },
            SplineUniform {
                this->splineMap.spline.a,
                this->splineMap.spline.b,
                this->splineMap.spline.c,
                this->splineMap.spline.d
            },
            this->splineMap.sizeSquared,
            this->splineMap.sizeSquared,
            this->splineMap.sizeSquared
        };

        glCreateBuffers(1, &splineMapUniform);
        glNamedBufferStorage(splineMapUniform, sizeof(SplineMapUniform), &uniform, GL_NONE);
    };

public:
    void update() {
        get_setup().update(get_window(), get_state());
        get_ray_emitter().update(get_setup(), get_state(), get_spline_map(), get_volume());
    }

    void render() {
        get_shader().use();
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, splineMapUniform);
        get_volume().bind();
        get_setup().start_render(get_shader());
        if (get_state().debugMode) {
            get_debug_shader().use();
            get_setup().debug(get_debug_shader());
            get_axis().render(get_setup());
            get_spline_map().render();
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
    inline virtual Plane&      get_base()         { return base;        };
    inline virtual Spline&     get_spline()       { return spline;      };
    inline virtual SplineMap&  get_spline_map()   { return splineMap;   };
    inline virtual Axis&       get_axis()         { return axis;        };
    inline virtual RayEmitter& get_ray_emitter()  { return rayEmitter;  };

protected:
    GLuint     splineMapUniform;
    State&     state;
    Window&    window;
    Setup      setup;
    Shader     shader, debugShader;
    Volume&    volume;
    Plane      base;
    Spline     spline;
    SplineMap  splineMap;
    Axis       axis;
    RayEmitter rayEmitter;
};
