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
    alignas(16) PlaneUniform plane;
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
        setup(Setup(glm::vec3(-1.0), glm::vec3(1.0))),
        shader(Shader("assets/simple_vol.vert", "assets/spline_map.frag")),
        volume(volume),
        base(base),
        spline(spline)
    {
        init();
    };

protected:
    void init() {
        SplineMap spline_map = SplineMap(get_base(), get_spline());

        SplineMapUniform uniform = SplineMapUniform {
            AABBUniform {
                spline_map.aabb.min,
                spline_map.aabb.max
            },
            PlaneUniform {
                spline_map.base.center,
                spline_map.base.half_size,
                spline_map.base.size,
                spline_map.base.normal,
                spline_map.base.span1,
                spline_map.base.span2,
                spline_map.base.min,
                spline_map.base.max,
                spline_map.base.matrix,
                spline_map.base.inv_matrix
            },
            SplineUniform {
                spline_map.spline.a,
                spline_map.spline.b,
                spline_map.spline.c,
                spline_map.spline.d
            },
            spline_map.sizeSquared,
            spline_map.sizeSquared,
            spline_map.sizeSquared
        };

        glCreateBuffers(1, &splineMapUniform);
        glNamedBufferStorage(splineMapUniform, sizeof(SplineMapUniform), &uniform, GL_NONE);
    };

public:
    void render() {
        get_setup().update(get_window(), get_state());
        get_shader().use();
        glBindBufferBase(GL_UNIFORM_BUFFER, 2, splineMapUniform);
        get_volume().bind();
        get_setup().render(get_shader());
    }

    inline virtual State  &get_state()  { return state;  };
    inline virtual Window &get_window() { return window; };
    inline virtual Setup  &get_setup()  { return setup;  };
    inline virtual Shader &get_shader() { return shader; };
    inline virtual Volume &get_volume() { return volume; };
    inline virtual Plane  &get_base()   { return base;   };
    inline virtual Spline &get_spline() { return spline; };

protected:
    GLuint splineMapUniform;
    State  &state;
    Window &window;
    Setup  setup;
    Shader shader;
    Volume &volume;
    Plane  base;
    Spline spline;
};
