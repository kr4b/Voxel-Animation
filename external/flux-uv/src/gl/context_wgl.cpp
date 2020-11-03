/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	CapturedWGLContext implementation
 */
/*-***************************************************************** -}}}1- */

#include "context_wgl.hxx"

#include <flux/gl/errors.hpp>
#include <flux/gl/error_category.hpp>
#include <flux/gl/checkpoint.hpp>

#include <flux/std/throw.hpp>
#include <flux/std/assert.hpp>
#include <flux/std/stdexcept.hpp>

#include <flux/util/narrow.hpp>

#include <flux_dlapi/GL/load.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::gl::detail >>>        ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(gl,detail)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	/* XXX- maybe consolidate the SharedContext_ with the CapturedGLXContext 
	 * code ... there's a bunch of stuff that ends up duplicated/shared.
	 */
	class SharedContext_ : public Context
	{
		public:
			SharedContext_( 
				GLapi const*,
				dlapi::os::Win32api const*,
				dlapi::os::WGLapi const*,
				dlapi::os::Win32::HDC,
				dlapi::os::WGL::HGLRC
			);

			~SharedContext_();

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
			GLapi const* mGL;
			dlapi::os::Win32api const* mWin32;
			dlapi::os::WGLapi const* mWGL;
			dlapi::os::Win32::HDC mDC;
			dlapi::os::WGL::HGLRC mGLRC;
	};

	// helpers
	Caps make_caps_( GLapi const* );
	unique_ptr<Context> make_share_( 
		Caps const&, 
		dlapi::gl::GLapi const*, 
		dlapi::os::Win32api const*, 
		dlapi::os::WGLapi const*, 
		dlapi::os::Win32::HDC, 
		dlapi::os::WGL::HGLRC
	);
}

//--    $ CapturedWGLContext               ///{{{2///////////////////////////

// [cd]tor -{{{3-
CapturedWGLContext::CapturedWGLContext( dlapi::os::Win32api const* aWin32, dlapi::os::WGLapi const* aWGL, dlapi::os::Win32::HDC aDC, dlapi::os::WGL::HGLRC aGLRC )
	: mWin32( aWin32 )
	, mWGL( aWGL )
	, mDC( aDC )
	, mGLRC( aGLRC )
{
	load_api_();

	dlapi::os::ex::acquire( mWin32 );
	dlapi::os::acquire( mWGL );
}

CapturedWGLContext::~CapturedWGLContext()
{
	if( base::thread_catalog().acquire<ThreadContext>() == this )
		base::thread_catalog().forget<ThreadContext>();

	if( gl_api() == mGL && gl_api_key() == this )
		set_gl_api( nullptr );

	unload_gl( mGL );

	dlapi::os::release( mWGL );
	dlapi::os::ex::release( mWin32 );
}

// virtuals -{{{3-
bool CapturedWGLContext::valid()
{
	return !!mGLRC;
}
bool CapturedWGLContext::owned()
{
	return false;
}
bool CapturedWGLContext::bound()
{
	FLUX_ASSERT( mWGL );
	return mGLRC && mGLRC == mWGL->getCurrentContext();
}

void CapturedWGLContext::bind()
{
	FLUX_ASSERT( mWin32 && mWGL );
	if( !mWGL->makeCurrent( mDC, mGLRC ) )
	{
		auto const err = mWin32->getLastError();
		FLUX_THROW( error::ContextError )
			<< einfo::ErrorDesc( "wglMakeCurrent() failed" )
			<< einfo::ApiError( sys::make_win32_error_code(err) )
		;
	}
}
void CapturedWGLContext::unbind()
{
	FLUX_ASSERT( mWin32 && mWGL );
	if( !mWGL->makeCurrent( nullptr, nullptr ) )
	{
		auto const err = mWin32->getLastError();
		FLUX_THROW( error::ContextError )
			<< einfo::ErrorDesc( "wglMakeCurrent() failed" )
			<< einfo::ApiError( sys::make_win32_error_code(err) )
		;
	}
}

Caps CapturedWGLContext::caps()
{
	FLUX_ASSERT( mWGL );

	// remember current state and bind "our" context
	auto const dc = mWGL->getCurrentDC();
	auto const rc = mWGL->getCurrentContext();

	bind();

	// fetch caps
	auto ret = make_caps_( mGL );

	// reset state and return result
	if( rc != mGLRC ) mWGL->makeCurrent( dc, rc );
	return ret;
}

GLapi const* CapturedWGLContext::api() 
{
	return mGL;
}

ftl::Any CapturedWGLContext::native_api()
{
	return ftl::Any( mWGL );
}
ftl::Any CapturedWGLContext::native_handle()
{
	return ftl::Any( mGLRC );
}

unique_ptr<Context> CapturedWGLContext::create_extra_context()
{
	return make_share_( caps(), mGL, mWin32, mWGL, mDC, mGLRC );
}

// private -{{{3-
void CapturedWGLContext::load_api_()
{
	namespace p_ = dlapi::gl::proto;

	// get a minimal set of functions, in order to figure out how to load
	// the rest of the API and to check for errors.
	LoadFnArg_ user;
	user.wgl = mWGL;

	auto const getError = reinterpret_cast<p_::GetErrorFn>(load_fn_("glGetError",&user));
	FLUX_THROW_IF( !getError, error::OpenGLError )
		<< einfo::ErrorDesc( "Missing OpenGL core API" )
		<< einfo::ErrorMeta( "glGetError" )
	;
	
	auto const getIntegerv = reinterpret_cast<p_::GetIntegervFn>(load_fn_("glGetIntegerv",&user));
	FLUX_THROW_IF( !getIntegerv, error::OpenGLError )
		<< einfo::ErrorDesc( "Missing OpenGL core API" )
		<< einfo::ErrorMeta( "glGetIntegerv" )
	;

	// clear current error
	(void)getError();

	// get current version
	GL::Int major, minor;
	getIntegerv( GL::MAJOR_VERSION, &major );
	getIntegerv( GL::MINOR_VERSION, &minor );

	if( GL::Enum err = getError() )
	{
		FLUX_THROW( error::OpenGLError )
			<< einfo::ApiError( make_gl_error(err) )
			<< einfo::ErrorDesc( "While retrieving OpenGL version" )
		;
	}

	// decided on how to load the API
	if( major < 3 || (major == 3 && minor <= 0) )
	{
		// archaic loading method: names of supported extensions are in a
		// single string returned by glGetString(GL_EXTENSIONS).
		auto const getString = reinterpret_cast<p_::GetStringFn>( load_fn_("glGetString",&user));
		FLUX_THROW_IF( !getString, error::GLApiError )
			<< einfo::ErrorDesc( "Missing OpenGL core API" )
			<< einfo::ErrorMeta( "glGetString" )
		;

		char const* exts = reinterpret_cast<char const*>(getString(GL::EXTENSIONS));
		if( !exts )
			exts = ""; // Assume no extensions...

		if( GL::Enum err = getError() )
		{
			FLUX_THROW( error::OpenGLError )
				<< einfo::ApiError( make_gl_error(err) )
				<< einfo::ErrorDesc( "While retrieving OpenGL extension list (archaic)" )
			;
		}

		CheckExtArchaic_ user;
		user.wgl = mWGL;
		user.exts = exts;

		mGL = dlapi::gl::load_gl(
			&CapturedWGLContext::check_ext_archaic_,
			&CapturedWGLContext::load_fn_,
			&user,
			major, minor
		);
	}
	else
	{
		// modern loading method: need to query each extension name
		// individually using glGetStringi(GL_EXTENSIONS,i). The number of
		// supported extensions is available via glGetIntegerv().
		auto const getStringi = reinterpret_cast<p_::GetStringiFn>( load_fn_("glGetStringi",&user));
		FLUX_THROW_IF( !getStringi, error::GLApiError )
			<< einfo::ErrorDesc( FLUX_FMT_STRING("Missing OpenGL {:d}.{:d} API"), major, minor )
			<< einfo::ErrorMeta( "glGetStringi" )
		;

		GL::Int numExts = 0;
		getIntegerv( GL::NUM_EXTENSIONS, &numExts );

	
		CheckExtModern_ user;
		user.wgl = mWGL;

		for( GL::Int i = 0; i < numExts; ++i )
		{
			char const* ext = reinterpret_cast<char const*>(getStringi(GL::EXTENSIONS,i));

			user.exts.insert( ext+3 ); // skip the GL_ part...
		}

		if( GL::Enum err = getError() )
		{
			FLUX_THROW( error::OpenGLError )
				<< einfo::ApiError( make_gl_error(err) )
				<< einfo::ErrorDesc( "While retrieving OpenGL extension list (modern)" )
			;
		}

		mGL = dlapi::gl::load_gl(
			&CapturedWGLContext::check_ext_modern_,
			&CapturedWGLContext::load_fn_,
			&user,
			major, minor
		);
	}

	FLUX_THROW_IF( !mGL, error::ContextError )
		<< einfo::ErrorDesc( "API loader returned nullptr for GLapi" )
	;
}

void* CapturedWGLContext::load_fn_( char const* aFuncName, void* aUser )
{
	auto const* arg = reinterpret_cast<LoadFnArg_ const*>(aUser);
	FLUX_ASSERT( arg );

	if( auto fp = arg->wgl->getProcAddress( aFuncName ) )
		return reinterpret_cast<void*>(fp);

	// try to load from OpenGL32.dll; this is the same DLL as WGL, so ... HAX
	return reinterpret_cast<void*>(sys::dylib_get_proc( arg->wgl->libHandle_, aFuncName ));
}

bool CapturedWGLContext::check_ext_archaic_( char const* aExtName, void* aUser )
{
	auto const* arg = reinterpret_cast<CheckExtArchaic_ const*>(aUser);
	FLUX_ASSERT( arg );

	if( char const* found = std::strstr( aExtName, arg->exts ) )
	{
		auto const len = std::strlen( aExtName );
		return '\0' == found[len] || ' ' == found[len];
	}

	return false;
}
bool CapturedWGLContext::check_ext_modern_( char const* aExtName, void* aUser )
{
	auto const* arg = reinterpret_cast<CheckExtModern_ const*>(aUser);
	FLUX_ASSERT( arg );

	FLUX_ASSERT( aExtName );
	return arg->exts.end() != arg->exts.find( aExtName );
}

//--    $ SharedContext_                    ///{{{2///////////////////////////
namespace 
{
	// [cd]tor -{{{3-
	SharedContext_::SharedContext_( GLapi const* aGL,  dlapi::os::Win32api const* aWin32, dlapi::os::WGLapi const* aWGL, dlapi::os::Win32::HDC aDC, dlapi::os::WGL::HGLRC aGLRC )
		: mGL( aGL )
		, mWin32( aWin32 )
		, mWGL( aWGL )
		, mDC( aDC )
		, mGLRC( aGLRC )
	{
		acquire_gl( mGL );
		dlapi::os::ex::acquire( mWin32 );
		dlapi::os::acquire( mWGL );
	}

	SharedContext_::~SharedContext_()
	{
		if( base::thread_catalog().acquire<ThreadContext>() == this )
			base::thread_catalog().forget<ThreadContext>();

		if( gl_api() == mGL && gl_api_key() == this )
			set_gl_api( nullptr );

		if( mGLRC == mWGL->getCurrentContext() )
			mWGL->makeCurrent( nullptr, nullptr );

		mWGL->deleteContext( mGLRC );

		dlapi::os::release( mWGL );
		dlapi::os::ex::release( mWin32 );
		release_gl( mGL );
	}

	// virtuals -{{{3-
	bool SharedContext_::valid()
	{
		return !!mGLRC;
	}
	bool SharedContext_::owned()
	{
		return true;
	}
	bool SharedContext_::bound()
	{
		FLUX_ASSERT( mWGL );
		return mGLRC && mGLRC == mWGL->getCurrentContext();
	}

	void SharedContext_::bind()
	{
		FLUX_ASSERT( mWin32 && mWGL );
		if( !mWGL->makeCurrent( mDC, mGLRC ) )
		{
			auto const err = mWin32->getLastError();
			FLUX_THROW( error::ContextError )
				<< einfo::ErrorDesc( "wglMakeCurrent() failed" )
				<< einfo::ApiError( sys::make_win32_error_code(err) )
			;
		}
	}
	void SharedContext_::unbind()
	{
		FLUX_ASSERT( mWin32 && mWGL );
		if( !mWGL->makeCurrent( nullptr, nullptr ) )
		{
			auto const err = mWin32->getLastError();
			FLUX_THROW( error::ContextError )
				<< einfo::ErrorDesc( "wglMakeCurrent() failed" )
				<< einfo::ApiError( sys::make_win32_error_code(err) )
			;
		}
}

	Caps SharedContext_::caps()
	{
		FLUX_ASSERT( mWGL );

		// remember current state and bind "our" context
		auto const dc = mWGL->getCurrentDC();
		auto const rc = mWGL->getCurrentContext();

		bind();

		// fetch caps
		auto ret = make_caps_( mGL );

		// reset state and return result
		if( rc != mGLRC ) mWGL->makeCurrent( dc, rc );
		return ret;
	}

	GLapi const* SharedContext_::api() 
	{
		return mGL;
	}

	ftl::Any SharedContext_::native_api()
	{
		return ftl::Any( mWGL );
	}
	ftl::Any SharedContext_::native_handle()
	{
		return ftl::Any( mGLRC );
	}

	unique_ptr<Context> SharedContext_::create_extra_context()
	{
		return make_share_( caps(), mGL, mWin32, mWGL, mDC, mGLRC );
	}
}

//--    $ make_caps_()                      ///{{{2///////////////////////////
namespace 
{
	Caps make_caps_( GLapi const* aGL )
	{
		FLUX_ASSERT( aGL );
		FLUX_GL_CHECKPOINT_ALWAYS();

		Caps ret;

		GL::Int major, minor;
		aGL->getIntegerv( GL::MAJOR_VERSION, &major );
		aGL->getIntegerv( GL::MINOR_VERSION, &minor );
		
		ret.major = util::narrow<unsigned>(major);
		ret.minor = util::narrow<unsigned>(minor);


		ret.vendor = reinterpret_cast<char const*>(aGL->getString( GL::VENDOR ));
		ret.renderer = reinterpret_cast<char const*>(aGL->getString( GL::RENDERER ));
		ret.osapi = "WGL";

		GL::Int flags;
		aGL->getIntegerv( GL::CONTEXT_FLAGS, &flags );
		
		ret.forwardCompatible = !!(flags & GL::CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT);
		ret.debug = !!(flags & GL::CONTEXT_FLAG_DEBUG_BIT);
		ret.noError = !!(flags & GL::CONTEXT_FLAG_NO_ERROR_BIT_KHR);

		ret.direct = true;
		
		FLUX_GL_CHECKPOINT_ALWAYS();
		return ret;
	}
}

//--    $ make_share_()                     ///{{{2///////////////////////////
namespace 
{
	unique_ptr<Context> make_share_( Caps const& aCaps, dlapi::gl::GLapi const* aGL, dlapi::os::Win32api const* aWin32, dlapi::os::WGLapi const* aWGL, dlapi::os::Win32::HDC aDC, dlapi::os::WGL::HGLRC aGLRC )
	{
		FLUX_ASSERT( aWGL );

		if( !aWGL->ARB_create_context )
			load_wgl_extensions( aWGL );

		dlapi::os::WGL::HGLRC ret = nullptr;
		if( aWGL->ARB_create_context )
		{
			namespace WGL_ = dlapi::os::WGL;

			int flags = 0;
			if( aCaps.debug ) flags |= WGL_::CONTEXT_DEBUG_BIT_ARB;
			if( aCaps.forwardCompatible ) flags |= WGL_::CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;

			int attr[] = {
				WGL_::CONTEXT_MAJOR_VERSION_ARB, util::narrow<int>(aCaps.major),
				WGL_::CONTEXT_MINOR_VERSION_ARB, util::narrow<int>(aCaps.minor),
				WGL_::CONTEXT_FLAGS_ARB, flags,
				0, 0
			};

			ret = aWGL->createContextAttribsARB( aDC, aGLRC, attr );
		}
		else
		{
			ret = aWGL->createContext( aDC );
		}

		return ret
			? make_unique<SharedContext_>( aGL, aWin32, aWGL, aDC, ret )
			: nullptr
		;
	}
}

//--    <<< ~ flux::gl::detail namespace <<<        ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(gl,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
