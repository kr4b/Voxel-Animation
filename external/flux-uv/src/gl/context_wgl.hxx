#ifndef DETAIL_CONTEXT_WGL_HXX_CAD5C39D_F1BD_4097_BDA4_71D4440FD76E
#define DETAIL_CONTEXT_WGL_HXX_CAD5C39D_F1BD_4097_BDA4_71D4440FD76E

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/gl/context.hpp>
#include <flux/gl/glapi.hpp>

#include <flux/compat/string_view.hpp>
#include <flux/sys/win32_category.hpp>

#include <flux_dlapi/GL/forward.hpp>

#include <flux_dlapi/os/win32.hpp>
#include <flux_dlapi/os/wgl.hpp>

#include <unordered_set>

//--    >>> namespace = flux::gl::detail >>>        ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(gl,detail)
//--    types                               ///{{{2///////////////////////////

using ExtenionSet_ = std::unordered_set<compat::string_view>;

//--    CapturedWGLContext                  ///{{{2///////////////////////////
class CapturedWGLContext final : public Context
{
	public:
		CapturedWGLContext( 
			dlapi::os::Win32api const*,
			dlapi::os::WGLapi const*,
			dlapi::os::Win32::HDC,
			dlapi::os::WGL::HGLRC
		);

		~CapturedWGLContext();

	public: /* virtuals from Context */
		bool valid() override;
		bool owned() override;
		bool bound() override;

		void bind() override;
		void unbind() override;

		Caps caps() override;
		dlapi::gl::GLapi const* api() override;

		ftl::Any native_api() override;
		ftl::Any native_handle() override;

		unique_ptr<Context> create_extra_context() override;

	private:
		struct LoadFnArg_
		{
			dlapi::os::WGLapi const* wgl;
		};

		struct CheckExtArchaic_ final : LoadFnArg_
		{
			char const* exts;
		};
		struct CheckExtModern_ final : LoadFnArg_
		{
			ExtenionSet_ exts;
		};

	private:
		void load_api_();
		
		static void* load_fn_( char const*, void* );
		static bool check_ext_archaic_( char const*, void* );
		static bool check_ext_modern_( char const*, void* );

	private:
		GLapi const* mGL;
		dlapi::os::Win32api const* mWin32;
		dlapi::os::WGLapi const* mWGL;
		dlapi::os::Win32::HDC mDC;
		dlapi::os::WGL::HGLRC mGLRC;
};

//--    <<< ~ flux::gl::detail namespace <<<        ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(gl,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_CONTEXT_WGL_HXX_CAD5C39D_F1BD_4097_BDA4_71D4440FD76E
