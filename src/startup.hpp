#ifndef STARTUP_HPP_CFF5AD8D_0734_4585_AE40_1CBCFAD3F0C8
#define STARTUP_HPP_CFF5AD8D_0734_4585_AE40_1CBCFAD3F0C8

#include <flux/gl/context.hpp>
#include <flux_dlapi/os/glfw.hpp>
#include <flux/std/unique_ptr.hpp>

bool global_startup();
void global_shutdown();

struct GfxContext
{
	flux::dlapi::os::GLFW::Window* win = nullptr;
	flux::unique_ptr<flux::gl::Context> context;

	// Not copyable, but movable
	GfxContext() = default;

	GfxContext( GfxContext const& ) = delete;
	GfxContext& operator= (GfxContext const& ) = delete;

	GfxContext( GfxContext&& ) noexcept;
	GfxContext& operator= (GfxContext&&) noexcept;

	// Convenience
	bool valid() noexcept;
};

GfxContext gfx_startup();
void gfx_shutdown( GfxContext );

struct GfxCallbacks
{
	flux::dlapi::os::GLFWproto::KeyCb keyCallback;
	flux::dlapi::os::GLFWproto::MouseBtnCb buttonCallback;
	flux::dlapi::os::GLFWproto::ScrollCb scrollCallback;
	flux::dlapi::os::GLFWproto::CursorPosCb cursorCallback;
};

void gfx_set_callbacks( 
	flux::dlapi::os::GLFW::Window*,
	GfxCallbacks const&, 
	void* aUser = nullptr
);

#endif // STARTUP_HPP_CFF5AD8D_0734_4585_AE40_1CBCFAD3F0C8
