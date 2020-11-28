#include <flux/std/log.hpp>
#include <flux/std/assert.hpp>
#include <flux/std/format.hpp>
#include <flux/std/exception.hpp>

#include <flux/gl/glapi.hpp>
#include <flux/gl/checkpoint.hpp>
#include <flux/gl/setup/program.hpp>
namespace gl = flux::gl;

#include <flux/fml/stdtypes.hpp>
#include <flux/fml/transform.hpp>
namespace fml = flux::fml;
using namespace fml::stdtypes;
using namespace fml::literals;

#include <flux/util/scope_exit.hpp>

#include <flux/compat/exchange.hpp>
namespace compat = flux::compat;

#include <flux_dlapi/os/glfw.hpp>
namespace GLFW = flux::dlapi::os::GLFW;

#include <chrono>
#include <limits>
#include <iostream>

#include "startup.hpp"
#include "defaults.hpp"
#include "volume.hpp"

namespace
{
    struct UVolMeta
    {
        alignas(16) vec3f volMin;
        alignas(16) vec3f volMax;
    };
    struct UCamera
    {
        alignas(16) mat44f inverseProjCamera;
        alignas(16) vec3f cameraWorldPos;
        alignas(16) vec2f reciprocalWindowSize;
    };

    struct State
    {
        quatf cameraRot = fml::make_identity<quatf>();
        vec3f cameraOff = fml::make_vector<vec3f>(0.f, 0.f, -3.f);

        double lastX = std::numeric_limits<double>::quiet_NaN();
        double lastY = std::numeric_limits<double>::quiet_NaN();

        bool inControl = false;

        static constexpr float scrollMult = 0.1f;
        static constexpr float motionRotMult = 0.6f;
    };

    // Convenience
    using Secf_ = std::chrono::duration<float>;

    // GLFW callbacks
    void glfw_cb_key_(GLFW::Window*, int, int, int, int);
    void glfw_cb_button_(GLFW::Window*, int, int, int);
    void glfw_cb_scroll_(GLFW::Window*, double, double);
    void glfw_cb_motion_(GLFW::Window*, double, double);
}

extern "C"
{
    // Use NVIDIA drivers if there are multiple GPUs (and at least one NVIDIA).
    // This avoids Intel drivers, which are ... special on occasion. For AMD use
    // __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
    // (This is not included by default, because it's untested at the moment.)
    //
    // Note: Both need to be in a extern "C" block to avoid name mangling!
#	if FLUX_PLATFORM == FLUX_PLATFORM_WIN32
    __declspec(dllexport) uint32_t NvOptimusEnablement = 1;
#	endif // ~ PLATFORM_WIN32
}


int main()
{
    // Startup
    if (!global_startup())
    {
        FLUX_LOG(FATAL, "Startup failed. Bye.");
        return 1;
    }

    FLUX_UTIL_ON_SCOPE_EXIT
    {
        global_shutdown();
    };

    // Create Window and rendering context.
    auto ctx = gfx_startup();
    if (!ctx.valid())
    {
        FLUX_LOG(FATAL, "No rendering context. Bye.");
        return 1;
    }

    FLUX_UTIL_ON_SCOPE_EXIT
    {
        gfx_shutdown(std::move(ctx));
    };

    State state;
    gfx_set_callbacks(ctx.win, GfxCallbacks{
        &glfw_cb_key_,
        &glfw_cb_button_,
        &glfw_cb_scroll_,
        &glfw_cb_motion_
        }, &state);

    // Load/prepare resources
    namespace GL = gl::GL;
    auto const gl = gl::gl_api();
    FLUX_ASSERT(gl);

    UVolMeta volMeta;
    volMeta.volMin = fml::make_splat<vec3f>(-1.0f);
    volMeta.volMax = fml::make_splat<vec3f>(+1.0f);

    //FLUX_ENABLE_EXCEPTION_INFO();
    FLUX_GL_CHECKPOINT_ALWAYS();

    auto const program = gl::load_program_from_vfs({
        { GL::VERTEX_SHADER, "/@flux/opt/assets/simple_vol.vert" },
        { GL::FRAGMENT_SHADER, "/@flux/opt/assets/spline_vol.frag" }
        });

    GL::UInt vao;
    gl->createVertexArrays(1, &vao);
    // This vao remains empty; we need to be allowed to legally draw vertices,
    // though.

    GL::UInt uVolMeta;
    gl->createBuffers(1, &uVolMeta);
    gl->namedBufferStorage(uVolMeta, sizeof(UVolMeta), &volMeta, 0);

    GL::UInt uCamera;
    gl->createBuffers(1, &uCamera);
    gl->namedBufferStorage(uCamera, sizeof(UCamera), nullptr, GL::DYNAMIC_STORAGE_BIT);

    FLUX_GL_CHECKPOINT_ALWAYS();

    //Volume vol = load_mhd_volume( "assets/backpack_small.mhd" );
    Volume vol = load_cube();
    if (0 == vol.total_element_count())
    {
        FLUX_LOG(FATAL, "Unable to load volume. Bye.");
        return 1;
    }

    GL::UInt vol3d;
    gl->createTextures(GL::TEXTURE_3D, 1, &vol3d);
    gl->bindTextureUnit(0, vol3d);

    // Comment this for blurry voxels
    gl->texParameteri(GL::TEXTURE_3D, GL::TEXTURE_MIN_FILTER, GL::NEAREST);
    gl->texParameteri(GL::TEXTURE_3D, GL::TEXTURE_MAG_FILTER, GL::NEAREST);

    gl->textureStorage3D(vol3d, 1, GL::R32F, vol.width(), vol.height(), vol.depth());
    gl->textureSubImage3D(vol3d, 0,
        0, 0, 0, // offset in the volume
        vol.width(), vol.height(), vol.depth(),
        GL::RED, GL::FLOAT, vol.data()
    );

    std::cout << vol.width() << ", " << vol.height() << ", " << vol.depth() << std::endl;

    FLUX_GL_CHECKPOINT_ALWAYS();

    // Main loop
    auto const* glfw = flux::dlapi::os::glfw();
    FLUX_ASSERT(glfw);

    const int steps = 2048;
    int frames = 0;
    double time = 0;
    auto start = std::chrono::high_resolution_clock::now();
    auto prev = std::chrono::high_resolution_clock::now();

    while (!glfw->windowShouldClose(ctx.win))
    {
        FLUX_GL_CHECKPOINT_DEBUG();

        int width, height;
        glfw->getFramebufferSize(ctx.win, &width, &height);
        gl->viewport(0, 0, width, height);

        auto const view =
            fml::make_translation_3d<mat44f>(state.cameraOff)
            * fml::make_rotation_3d<mat44f>(state.cameraRot)
            ;
        auto const vp = fml::invert(view) * fml::make_vector<vec4f>(0.f, 0.f, 0.f, 1.f);

        auto const proj = fml::make_projection_perspective_gl_3d<mat44f>(
            60.0_degf,
            float(width) / height,
            0.1f,
            100.f
            );

        UCamera camera;
        camera.cameraWorldPos = fml::shrink<vec3f>(vp) / vp.w;
        camera.inverseProjCamera = fml::invert(proj * view);
        camera.reciprocalWindowSize = fml::make_vector<vec2f>(
            1.f / width,
            1.f / height
            );

        gl->namedBufferSubData(uCamera, 0, sizeof(UCamera), &camera);

        // Clear screen
        // Note: We don't need a depth buffer for this simple demo, so just
        // clear color.
        gl->clear(GL::COLOR_BUFFER_BIT);

        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> diff = now - start;

        // Run fullscreen shader.
        gl->useProgram(program);
        gl->uniform1i(gl->getUniformLocation(program, "steps"), steps);
        gl->uniform1f(gl->getUniformLocation(program, "time"), diff.count());
        gl->bindBufferBase(GL::UNIFORM_BUFFER, 0, uVolMeta);
        gl->bindBufferBase(GL::UNIFORM_BUFFER, 1, uCamera);

        gl->bindTextureUnit(0, vol3d);

        gl->bindVertexArray(vao);
        gl->drawArrays(GL::TRIANGLES, 0, 3);

        // Clean up state
        gl->useProgram(0);
        gl->bindVertexArray(0);

        // Swap buffers. pollEvents() so that the animation can run.
        FLUX_GL_CHECKPOINT_DEBUG();
        glfw->swapBuffers(ctx.win);
        glfw->pollEvents();

        frames += 1;
        std::chrono::duration<double, std::milli> timePassed = now - prev;

        if (int(timePassed.count()) > 10000) {
            prev = now;
            std::cout << frames / 10 << std::endl;
            frames = 0;
        }
    }

    return 0;
}


namespace
{
    void glfw_cb_key_(GLFW::Window* aWin, int aKey, int, int aAct, int)
    {
        // Special: Escape => exit.
        if (GLFW::KEY_ESCAPE == aKey && GLFW::PRESS == aAct)
        {
            flux::dlapi::os::glfw()->setWindowShouldClose(aWin, GLFW::GLFW_TRUE);
            return;
        }
    }
    void glfw_cb_button_(GLFW::Window* aWin, int aBut, int aAct, int)
    {
        auto const* glfw = flux::dlapi::os::glfw();
        FLUX_ASSERT(glfw);

        if (auto state = reinterpret_cast<State*>(glfw->getWindowUserPointer(aWin)))
        {
            if (GLFW::MOUSE_BUTTON_LEFT == aBut)
            {
                if ((state->inControl = (GLFW::PRESS == aAct)))
                    glfw->setInputMode(aWin, GLFW::CURSOR, GLFW::CURSOR_DISABLED);
                else
                    glfw->setInputMode(aWin, GLFW::CURSOR, GLFW::CURSOR_NORMAL);
            }
        }
    }
    void glfw_cb_scroll_(GLFW::Window* aWin, double /*aX*/, double aY)
    {
        auto const* glfw = flux::dlapi::os::glfw();
        FLUX_ASSERT(glfw);

        if (auto state = reinterpret_cast<State*>(glfw->getWindowUserPointer(aWin)))
        {
            auto dist = 1.f - state->scrollMult * float(aY);
            state->cameraOff *= dist;
        }
    }
    void glfw_cb_motion_(GLFW::Window* aWin, double aX, double aY)
    {
        auto const* glfw = flux::dlapi::os::glfw();
        FLUX_ASSERT(glfw);

        if (auto state = reinterpret_cast<State*>(glfw->getWindowUserPointer(aWin)))
        {
            using namespace fml;

            auto const lastX = compat::exchange(state->lastX, aX);
            auto const lastY = compat::exchange(state->lastY, aY);
            if (!std::isnan(lastX))
            {
                auto const deltaX = aX - lastX;
                auto const deltaY = aY - lastY;

                if (state->inControl)
                {
                    state->cameraRot =
                        make_rotation_3d<quatf>(degreesf(state->motionRotMult*deltaX), 0.f, 1.f, 0.f)
                        * make_rotation_3d<quatf>(degreesf(state->motionRotMult*deltaY), 1.f, 0.f, 0.f)
                        * state->cameraRot
                        ;
                }
            }
        }
    }
}
