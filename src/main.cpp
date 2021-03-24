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
#include <string>
#include <cmath>
#include <algorithm>

#include "defaults.hpp"
#include "spline_source.hpp"
#include "startup.hpp"
#include "sampler.hpp"
#include "volume.hpp"

// Templates also need cpp files?
#include "sampler.cpp"

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

        quatf lastCameraRot = fml::make_identity<quatf>();
        vec3f lastCameraOff = fml::make_vector<vec3f>(0.f, 0.f, -3.f);

        double lastX = 0.0;
        double lastY = 0.0;

        bool inControl = false;
        bool debugMode = false;
        bool refreshSpline = true;
        bool refreshSplineSource = true;
        bool displayRays = false;

        size_t splineCount = 2;
        float splineDist = 0.1f;

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

float map(float current, float min1, float max1, float min2, float max2) {
    return min2 + current / (max1 - min1) * (max2 - min2);
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

    const size_t size = 20;
    const float strength = 10.0f;
    std::vector<unsigned char> samplers;
    std::vector<vec3f> data;
    std::vector<vec3f> colors;

    for (size_t k = 0; k < size; k++) {
        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < size; j++) {
                unsigned char sample = 1;
                vec3f d = fml::make_zero<vec3f>();
                vec3f c = fml::make_zero<vec3f>();

                if (k > 0 && k < size - 1 && i > 0 && i < size - 1 && j == 0) {
                    d = vec3f(
                        -strength,
                        map(i, 1, size - 1, -strength, strength),
                        map(k, 1, size - 1, -strength, strength));

                    c = vec3f(map(k, 1, size - 1, 1.0f, 0), map(i, 1, size - 1, 0, 1.0f), 0);
                }
                else if (k > 0 && k < size - 1 && i > 0 && i < size - 1 && j == size - 1) {
                    d = vec3f(
                        strength,
                        map(i, 1, size - 1, -strength, strength),
                        map(k, 1, size - 1, -strength, strength));

                    c = vec3f(map(i, 1, size - 1, 1.0f, 0), map(k, 1, size - 1, 1.0f, 0), 0);
                }
                else if (k > 0 && k < size - 1 && j > 0 && j < size - 1 && i == 0) {
                    d = vec3f(
                        map(j, 1, size - 1, -strength, strength),
                        strength,
                        map(k, 1, size - 1, -strength, strength));

                    c = vec3f(0, map(j, 1, size - 1, 0, 1.0f), map(k, 1, size - 1, 1.0f, 0));
                }
                else if (k > 0 && k < size - 1 && j > 0 && j < size - 1 && i == size - 1) {
                    d = vec3f(
                        map(j, 1, size - 1, -strength, strength),
                        -strength,
                        map(k, 1, size - 1, -strength, strength));

                    c = vec3f(0, map(k, 1, size - 1, 1.0f, 0), map(j, 1, size - 1, 0, 1.0f));
                }
                else if (i > 0 && i < size - 1 && j > 0 && j < size - 1 && k == 0) {
                    d = vec3f(
                        map(j, 1, size - 1, -strength, strength),
                        map(i, 1, size - 1, -strength, strength),
                        -strength);

                    c = vec3f(map(j, 1, size - 1, 0, 1.0f), 0, map(i, 1, size - 1, 1.0f, 0));
                }
                else if (i > 0 && i < size - 1 && j > 0 && j < size - 1 && k == size - 1) {
                    d = vec3f(
                        map(j, 1, size - 1, -strength, strength), 
                        map(i, 1, size - 1, -strength, strength),
                        strength);

                    c = vec3f(map(i, 1, size - 1, 1.0f, 0), 0, map(j, 1, size - 1, 0, 1.0f));
                }
                else {
                    sample = 0;
                }

                samplers.push_back(sample);
                data.push_back(d);
                colors.push_back(c);
            }
        }
    }

    const vec3f aabbMin = fml::make_splat<vec3f>(-1.0f);
    const vec3f aabbMax = fml::make_splat<vec3f>(1.0f);

    Sampler<vec3f> sampler(
        AABB(aabbMin * 2.0f, aabbMax * 2.0f),
        AABB(aabbMin, aabbMax),
        size,
        samplers,
        data,
        colors,
        [gl](Ray ray, vec3f t, vec3f c) {
            const vec3f P1 = ray.origin;
            const vec3f P2 = P1 + ray.dir;

            const vec3f P0 = fml::make_zero<vec3f>();;
            const vec3f P3 = t;

            Spline spline(gl);
            spline.parameters_from_tangents(P1, P2, P0, P3);
            spline.set_color(c);
            return spline;
        },
        gl
    );

    //FLUX_ENABLE_EXCEPTION_INFO();
    FLUX_GL_CHECKPOINT_ALWAYS();

    auto const program = gl::load_program_from_vfs({
        { GL::VERTEX_SHADER, "/@flux/opt/assets/simple_vol.vert" },
        { GL::FRAGMENT_SHADER, "/@flux/opt/assets/deformation.frag" }
    });

    auto const debugProgram = gl::load_program_from_vfs({
        { GL::VERTEX_SHADER, "/@flux/opt/assets/simple_vol.vert" },
        { GL::FRAGMENT_SHADER, "/@flux/opt/assets/simple_vol.frag" }
    });

    auto const debugSplineProgram = gl::load_program_from_vfs({
        { gl::GL::VERTEX_SHADER, "/@flux/opt/assets/debug.vert" },
        { gl::GL::FRAGMENT_SHADER, "/@flux/opt/assets/debug.frag" }
    });

    GL::UInt vao;
    gl->createVertexArrays(1, &vao);
    // This vao remains empty; we need to be allowed to legally draw vertices, though.

    GL::UInt uVolMeta;
    gl->createBuffers(1, &uVolMeta);
    gl->namedBufferStorage(uVolMeta, sizeof(UVolMeta), &volMeta, 0);

    GL::UInt uCamera;
    gl->createBuffers(1, &uCamera);
    gl->namedBufferStorage(uCamera, sizeof(UCamera), nullptr, GL::DYNAMIC_STORAGE_BIT);

    SplineSource splines;
    splines.set_values(gl, state.splineCount, state.splineDist);

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
    
    GL::UInt dataChecker;
    gl->activeTexture(GL::TEXTURE1);
    gl->createTextures(GL::TEXTURE_3D, 1, &dataChecker);
    gl->bindTexture(GL::TEXTURE_3D, dataChecker);
    gl->texImage3D(GL::TEXTURE_3D, 0, GL::R8UI, sampler.size, sampler.size, sampler.size, 0, GL::RED_INTEGER, GL::UNSIGNED_BYTE, sampler.sampler.data());
    gl->texParameteri(GL::TEXTURE_3D, GL::TEXTURE_MIN_FILTER, GL::NEAREST);
    gl->texParameteri(GL::TEXTURE_3D, GL::TEXTURE_MAG_FILTER, GL::NEAREST);
    
    GL::UInt dataData;
    gl->activeTexture(GL::TEXTURE2);
    gl->createTextures(GL::TEXTURE_3D, 1, &dataData);
    gl->bindTexture(GL::TEXTURE_3D, dataData);
    gl->texImage3D(GL::TEXTURE_3D, 0, GL::RGB32F, sampler.size, sampler.size, sampler.size, 0, GL::RGB, GL::FLOAT, sampler.data.data());
    gl->texParameteri(GL::TEXTURE_3D, GL::TEXTURE_MIN_FILTER, GL::NEAREST);
    gl->texParameteri(GL::TEXTURE_3D, GL::TEXTURE_MAG_FILTER, GL::NEAREST);
    
    FLUX_GL_CHECKPOINT_ALWAYS();

    // Main loop
    auto const* glfw = flux::dlapi::os::glfw();
    FLUX_ASSERT(glfw);

    vec3f tangent1 = fml::make_vector<vec3f>(0.0f, 0.0f, 0.0f);
    vec3f tangent2 = fml::make_vector<vec3f>(0.0f, 0.0f, 0.0f);

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
            * fml::make_rotation_3d<mat44f>(state.cameraRot);

        auto const vp = fml::invert(view) * fml::make_vector<vec4f>(0.f, 0.f, 0.f, 1.f);

        auto const proj = fml::make_projection_perspective_gl_3d<mat44f>(
            60.0_degf,
            float(width) / height,
            0.01f,
            100.f
        );

        // Clear screen
        gl->clear(GL::COLOR_BUFFER_BIT | GL::DEPTH_BUFFER_BIT);

        UCamera camera;
        camera.cameraWorldPos = fml::shrink<vec3f>(vp) / vp.w;
        camera.inverseProjCamera = fml::invert(proj * view);
        camera.reciprocalWindowSize = fml::make_vector<vec2f>(
            1.f / width,
            1.f / height
        );

        gl->namedBufferSubData(uCamera, 0, sizeof(UCamera), &camera);
        auto now = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> diff = now - start;

        if (state.debugMode) {
            gl->useProgram(debugProgram);

            gl->uniformMatrix4fv(gl->getUniformLocation(debugProgram, "view"), 1, gl::GL::GLFALSE, view.data());
            gl->uniformMatrix4fv(gl->getUniformLocation(debugProgram, "proj"), 1, gl::GL::GLFALSE, proj.data());
        } else {
            gl->useProgram(program);
            gl->uniform1i(gl->getUniformLocation(program, "steps"), steps);
            gl->uniform1f(gl->getUniformLocation(program, "time"), diff.count());
            gl->uniform3f(gl->getUniformLocation(program, "tangent1"), tangent1.x, tangent1.y, tangent1.z);
            gl->uniform3f(gl->getUniformLocation(program, "tangent2"), tangent2.x, tangent2.y, tangent2.z);

            gl->activeTexture(GL::TEXTURE1);
            gl->bindTexture(GL::TEXTURE_3D, dataChecker);
            gl->uniform1i(gl->getUniformLocation(program, "sampler.dataCheck"), 1);

            gl->activeTexture(GL::TEXTURE2);
            gl->bindTexture(GL::TEXTURE_3D, dataData);
            gl->uniform1i(gl->getUniformLocation(program, "sampler.data[0]"), 2);
            gl->uniform1i(gl->getUniformLocation(program, "sampler.size"), sampler.size);
            gl->uniform3fv(gl->getUniformLocation(program, "sampler.aabb.min"), 1, sampler.samplerAABB.min.data());
            gl->uniform3fv(gl->getUniformLocation(program, "sampler.aabb.max"), 1, sampler.samplerAABB.max.data());
        }

        gl->bindBufferBase(GL::UNIFORM_BUFFER, 0, uVolMeta);
        gl->bindBufferBase(GL::UNIFORM_BUFFER, 1, uCamera);

        gl->bindTextureUnit(0, vol3d);
        gl->bindVertexArray(vao);

        gl->drawArrays(GL::TRIANGLES, 0, 3);

        if (state.debugMode) {
            if (state.refreshSpline) {
                splines.update_from_screen_coords(
                    fml::make_vector<vec2f>(state.lastX, height - state.lastY) * camera.reciprocalWindowSize,
                    camera.inverseProjCamera,
                    camera.cameraWorldPos
                );
            }

            if (state.refreshSplineSource) {
                splines.set_values(gl, state.splineCount, state.splineDist);
            }

            if (state.refreshSpline || state.refreshSplineSource) {
                splines.update_from_screen_coords(gl, sampler);
                splines.intersect_spline_aabb(volMeta.volMin, volMeta.volMax);
                splines.update_buffers(gl);
                state.refreshSpline = false;
                state.refreshSplineSource = false;
            }

            gl->useProgram(debugSplineProgram);
            gl->uniformMatrix4fv(gl->getUniformLocation(debugSplineProgram, "view"), 1, gl::GL::GLFALSE, view.data());
            gl->uniformMatrix4fv(gl->getUniformLocation(debugSplineProgram, "proj"), 1, gl::GL::GLFALSE, proj.data());
            splines.render(state.displayRays, gl);
        }

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

namespace {
    void glfw_cb_key_(GLFW::Window* aWin, int aKey, int, int aAct, int) {
        auto const* glfw = flux::dlapi::os::glfw();
        FLUX_ASSERT(glfw);

        if (auto state = reinterpret_cast<State*>(glfw->getWindowUserPointer(aWin))) {
            if (aAct != GLFW::PRESS) {
                return;
            }

            switch (aKey) {
            // Escape => exit
            case GLFW::KEY_ESCAPE:
                glfw->setWindowShouldClose(aWin, GLFW::GLFW_TRUE);
                break;
            // Space => create new debug spline
            case GLFW::KEY_SPACE:
                state->refreshSpline = true;
                state->lastCameraRot = state->cameraRot;
                state->lastCameraOff = state->cameraOff;
                break;
            // Tab => toggle debug mode
            case GLFW::KEY_TAB: {
                state->debugMode = !state->debugMode;
                std::string newTitle = std::string(kWindowDefaults.name) + (state->debugMode ? " - Debug" : "");
                glfw->setWindowTitle(aWin, newTitle.c_str());
                break;
            }
            // Backspace => return to camera transformation when spline was created
            case GLFW::KEY_BACKSPACE:
                state->cameraRot = state->lastCameraRot;
                state->cameraOff = state->lastCameraOff;
                break;
            // Up arrow => increase debug spline count
            case GLFW::KEY_UP:
                state->refreshSplineSource = true;
                state->splineCount += 1;
                break;
                // Up arrow => decrease debug spline count
            case GLFW::KEY_DOWN:
                state->refreshSplineSource = true;
                state->splineCount = std::max(1, int(state->splineCount) - 1);
                break;
                // Up arrow => increase debug spline distance
            case GLFW::KEY_RIGHT:
                state->refreshSplineSource = true;
                state->splineDist = std::min(1.0f, state->splineDist + 0.01f);;
                break;
                // Up arrow => decrease debug spline distance
            case GLFW::KEY_LEFT:
                state->refreshSplineSource = true;
                state->splineDist = std::max(0.01f, state->splineDist - 0.01f);;
                break;

            case GLFW::KEY_R:
                state->displayRays = !state->displayRays;
                break;
            }

            return;
        }
    }

    void glfw_cb_button_(GLFW::Window* aWin, int aBut, int aAct, int) {
        auto const* glfw = flux::dlapi::os::glfw();
        FLUX_ASSERT(glfw);

        if (auto state = reinterpret_cast<State*>(glfw->getWindowUserPointer(aWin))) {
            if (GLFW::MOUSE_BUTTON_LEFT == aBut) {
                if ((state->inControl = (GLFW::PRESS == aAct)))
                    glfw->setInputMode(aWin, GLFW::CURSOR, GLFW::CURSOR_DISABLED);
                else
                    glfw->setInputMode(aWin, GLFW::CURSOR, GLFW::CURSOR_NORMAL);
            }
        }
    }

    void glfw_cb_scroll_(GLFW::Window* aWin, double /*aX*/, double aY) {
        auto const* glfw = flux::dlapi::os::glfw();
        FLUX_ASSERT(glfw);

        if (auto state = reinterpret_cast<State*>(glfw->getWindowUserPointer(aWin))) {
            auto dist = 1.f - state->scrollMult * float(aY);
            state->cameraOff *= dist;
        }
    }

    void glfw_cb_motion_(GLFW::Window* aWin, double aX, double aY) {
        auto const* glfw = flux::dlapi::os::glfw();
        FLUX_ASSERT(glfw);

        if (auto state = reinterpret_cast<State*>(glfw->getWindowUserPointer(aWin))) {
            using namespace fml;

            auto const lastX = compat::exchange(state->lastX, aX);
            auto const lastY = compat::exchange(state->lastY, aY);
            if (!std::isnan(lastX)) {
                auto const deltaX = aX - lastX;
                auto const deltaY = aY - lastY;

                if (state->inControl) {
                    state->cameraRot =
                        make_rotation_3d<quatf>(degreesf(state->motionRotMult*deltaX), 0.f, 1.f, 0.f)
                        * make_rotation_3d<quatf>(degreesf(state->motionRotMult*deltaY), 1.f, 0.f, 0.f)
                        * state->cameraRot;
                }
            }
        }
    }
}
