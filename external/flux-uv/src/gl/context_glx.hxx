#ifndef DETAIL_CONTEXT_GLX_HXX_000073B7_6FBA_4630_8546_2260689E0AD7
#define DETAIL_CONTEXT_GLX_HXX_000073B7_6FBA_4630_8546_2260689E0AD7

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/gl/context.hpp>
#include <flux/gl/glapi.hpp>

#include <flux/compat/string_view.hpp>

#include <flux_dlapi/GL/forward.hpp>

#include <flux_dlapi/os/x11.hpp>
#include <flux_dlapi/os/glx.hpp>

#include <unordered_set>

//--    >>> namespace = flux::gl::detail >>>        ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(gl,detail)
//--    types                               ///{{{2///////////////////////////

using ExtenionSet_ = std::unordered_set<compat::string_view>;

//--    CapturedGLXContext                  ///{{{2///////////////////////////
class CapturedGLXContext final : public Context
{
	public:
		CapturedGLXContext( 
			dlapi::os::X11api const*,
			dlapi::os::GLXapi const*,
			dlapi::os::GLX::Context,
			dlapi::os::GLX::Drawable,
			dlapi::os::X11::Display*
		);

		~CapturedGLXContext();

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
			dlapi::os::GLXapi const* glx;
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
		dlapi::os::X11api const* mX11;
		dlapi::os::GLXapi const* mGLX;
		dlapi::os::GLX::Context mContext;
		dlapi::os::GLX::Drawable mDrawable;
		dlapi::os::X11::Display* mDisplay;
};

//--    <<< ~ flux::gl::detail namespace <<<        ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(gl,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_CONTEXT_GLX_HXX_000073B7_6FBA_4630_8546_2260689E0AD7
