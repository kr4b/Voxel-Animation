/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	CapturedGLXContext implementation
 */
/*-***************************************************************** -}}}1- */

#include "context_glx.hxx"

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
	class SharedContext_ : public Context
	{
		public:
			SharedContext_( 
				GLapi const*,
				dlapi::os::X11api const*,
				dlapi::os::GLXapi const*,
				dlapi::os::GLX::Context,
				dlapi::os::GLX::Drawable,
				dlapi::os::X11::Display*
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
			dlapi::os::X11api const* mX11;
			dlapi::os::GLXapi const* mGLX;
			dlapi::os::GLX::Context mContext;
			dlapi::os::GLX::Drawable mDrawable;
			dlapi::os::X11::Display* mDisplay;
	};

	// helpers
	Caps make_caps_( GLapi const*, dlapi::os::GLXapi const*, dlapi::os::X11::Display*, dlapi::os::GLX::Context );

	unique_ptr<Context> make_share_( Caps const&, dlapi::gl::GLapi const*, dlapi::os::X11api const*, dlapi::os::GLXapi const*, dlapi::os::X11::Display*, dlapi::os::GLX::Drawable, dlapi::os::GLX::Context );
}

//--    CapturedGLXContext                 ///{{{2///////////////////////////

// [cd]tor -{{{3-
CapturedGLXContext::CapturedGLXContext( dlapi::os::X11api const* aX11, dlapi::os::GLXapi const* aGLX, dlapi::os::GLX::Context aContext, dlapi::os::GLX::Drawable aDrawable, dlapi::os::X11::Display* aDisplay )
	: mX11( aX11 )
	, mGLX( aGLX )
	, mContext( aContext )
	, mDrawable( aDrawable )
	, mDisplay( aDisplay )
{
	dlapi::os::ex::acquire( mX11 );
	dlapi::os::acquire( mGLX );
	
	load_api_();
}

CapturedGLXContext::~CapturedGLXContext()
{
	if( base::thread_catalog().acquire<ThreadContext>() == this )
		base::thread_catalog().forget<ThreadContext>();

	if( gl_api() == mGL && gl_api_key() == this )
		set_gl_api( nullptr );

	unload_gl( mGL );

	dlapi::os::release( mGLX );
	dlapi::os::ex::release( mX11 );
}

// virtuals -{{{3-
bool CapturedGLXContext::valid()
{
	return !!mContext;
}
bool CapturedGLXContext::owned()
{
	return false;
}
bool CapturedGLXContext::bound()
{
	FLUX_ASSERT( mGLX );
	return mContext && mContext == mGLX->getCurrentContext();
}

void CapturedGLXContext::bind()
{
	FLUX_ASSERT( mGLX );
	if( !mGLX->makeCurrent( mDisplay, mDrawable, mContext ) )
	{
		//XXX: figure out what error occurred? X11 error handling is pretty
		// atrocious, though (XSetErrorHandler()) is global in multiple
		// ways...
		FLUX_THROW( error::ContextError )
			<< einfo::ErrorDesc( "glXMakeCurrent() failed" )
		;
	}
}
void CapturedGLXContext::unbind()
{
	FLUX_ASSERT( mGLX );
	if( !mGLX->makeCurrent( mDisplay, dlapi::os::X11::None, nullptr ) )
	{
		//XXX: see above
		FLUX_THROW( error::ContextError )
			<< einfo::ErrorDesc( "glXMakeCurrent() failed" )
		;
	}
}

Caps CapturedGLXContext::caps()
{
	FLUX_ASSERT( mGLX );

	// remember current state and bind "our" context
	auto const ctx = mGLX->getCurrentContext();
	auto const drw = mGLX->getCurrentDrawable();
	auto const dpy = mGLX->getCurrentDisplay();

	bind();

	// fetch caps
	auto ret = make_caps_( mGL, mGLX, mDisplay, mContext );
	
	// reset state and return result
	if( ctx != mContext ) mGLX->makeCurrent( dpy, drw, ctx );
	return ret;
}

GLapi const* CapturedGLXContext::api() 
{
	return mGL;
}

ftl::Any CapturedGLXContext::native_api()
{
	return ftl::Any( mGLX );
}
ftl::Any CapturedGLXContext::native_handle()
{
	return ftl::Any( mContext );
}

unique_ptr<Context> CapturedGLXContext::create_extra_context()
{
	return make_share_( caps(), mGL, mX11, mGLX, mDisplay, mDrawable, mContext );
}

// private -{{{3-
void CapturedGLXContext::load_api_()
{
	namespace p_ = dlapi::gl::proto;

	// get a minimal set of functions, in order to figure out how to load
	// the rest of the API and to check for errors.
	LoadFnArg_ user;
	user.glx = mGLX;

	auto const getError = reinterpret_cast<p_::GetErrorFn>(load_fn_("glGetError",&user));
	FLUX_THROW_IF( !getError, error::GLApiError )
		<< einfo::ErrorDesc( "Missing OpenGL core API" )
		<< einfo::ErrorMeta( "glGetError" )
	;
	
	auto const getIntegerv = reinterpret_cast<p_::GetIntegervFn>(load_fn_("glGetIntegerv",&user));
	FLUX_THROW_IF( !getIntegerv, error::GLApiError )
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
			<< einfo::ErrorDesc( "Unable to retrieve OpenGL version" )
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
		user.glx = mGLX;
		user.exts = exts;

		mGL = dlapi::gl::load_gl(
			&CapturedGLXContext::check_ext_archaic_,
			&CapturedGLXContext::load_fn_,
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
		user.glx = mGLX;

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
			&CapturedGLXContext::check_ext_modern_,
			&CapturedGLXContext::load_fn_,
			&user,
			major, minor
		);
	}

	FLUX_THROW_IF( !mGL, error::GLApiError )
		<< einfo::ErrorDesc( "API loader returned nullptr for GLapi" )
	;
}

void* CapturedGLXContext::load_fn_( char const* aFuncName, void* aUser )
{
	auto const* arg = reinterpret_cast<LoadFnArg_ const*>(aUser);
	FLUX_ASSERT( arg );

	return reinterpret_cast<void*>(arg->glx->getProcAddress( reinterpret_cast<GL::UByte const*>(aFuncName) ));
}

bool CapturedGLXContext::check_ext_archaic_( char const* aExtName, void* aUser )
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
bool CapturedGLXContext::check_ext_modern_( char const* aExtName, void* aUser )
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
	SharedContext_::SharedContext_( GLapi const* aGL, dlapi::os::X11api const* aX11, dlapi::os::GLXapi const* aGLX, dlapi::os::GLX::Context aContext, dlapi::os::GLX::Drawable aDrawable, dlapi::os::X11::Display* aDisplay )
		: mGL( aGL )
		, mX11( aX11 )
		, mGLX( aGLX )
		, mContext( aContext )
		, mDrawable( aDrawable )
		, mDisplay( aDisplay )
	{
		acquire_gl( mGL );
		dlapi::os::ex::acquire( mX11 );
		dlapi::os::acquire( mGLX );
	}

	SharedContext_::~SharedContext_()
	{
		if( base::thread_catalog().acquire<ThreadContext>() == this )
			base::thread_catalog().forget<ThreadContext>();

		if( gl_api() == mGL && gl_api_key() == this )
			set_gl_api( nullptr );

		if( mContext == mGLX->getCurrentContext() )
			mGLX->makeCurrent( mDisplay, dlapi::os::X11::None, nullptr );

		mGLX->destroyContext( mDisplay, mContext );

		dlapi::os::release( mGLX );
		dlapi::os::ex::release( mX11 );
		release_gl( mGL );
	}

	// virtuals -{{{3-
	bool SharedContext_::valid()
	{
		return !!mContext;
	}
	bool SharedContext_::owned()
	{
		return true;
	}
	bool SharedContext_::bound()
	{
		FLUX_ASSERT( mGLX );
		return mContext && mContext == mGLX->getCurrentContext();
	}

	void SharedContext_::bind()
	{
		FLUX_ASSERT( mGLX );
		if( !mGLX->makeCurrent( mDisplay, mDrawable, mContext ) )
		{
			//XXX: see CapturedGLXContext::bind()
			FLUX_THROW( error::ContextError )
				<< einfo::ErrorDesc( "glXMakeCurrent() failed" )
			;
		}
	}
	void SharedContext_::unbind()
	{
		FLUX_ASSERT( mGLX );
		if( !mGLX->makeCurrent( mDisplay, dlapi::os::X11::None, nullptr ) )
		{
			//XXX: see CapturedGLXContext::bind()
			FLUX_THROW( error::ContextError )
				<< einfo::ErrorDesc( "glXMakeCurrent() failed" )
			;
		}
	}

	Caps SharedContext_::caps()
	{
		FLUX_ASSERT( mGLX );

		// remember current state and bind "our" context
		auto const ctx = mGLX->getCurrentContext();
		auto const drw = mGLX->getCurrentDrawable();
		auto const dpy = mGLX->getCurrentDisplay();

		bind();

		// fetch caps
		auto ret = make_caps_( mGL, mGLX, mDisplay, mContext );
		
		// reset state and return result
		if( ctx != mContext ) mGLX->makeCurrent( dpy, drw, ctx );
		return ret;
	}

	GLapi const* SharedContext_::api() 
	{
		return mGL;
	}

	ftl::Any SharedContext_::native_api()
	{
		return ftl::Any( mGLX );
	}
	ftl::Any SharedContext_::native_handle()
	{
		return ftl::Any( mContext );
	}

	unique_ptr<Context> SharedContext_::create_extra_context()
	{
		return make_share_( caps(), mGL, mX11, mGLX, mDisplay, mDrawable, mContext );
	}
}

//--    $ make_caps_()                      ///{{{2///////////////////////////
namespace 
{
	Caps make_caps_( GLapi const* aGL, dlapi::os::GLXapi const* aGLX, dlapi::os::X11::Display* aDisplay, dlapi::os::GLX::Context aContext )
	{
		FLUX_ASSERT( aGL && aGLX );
		FLUX_GL_CHECKPOINT_ALWAYS();

		Caps ret;

		GL::Int major, minor;
		aGL->getIntegerv( GL::MAJOR_VERSION, &major );
		aGL->getIntegerv( GL::MINOR_VERSION, &minor );
		
		ret.major = util::narrow<unsigned>(major);
		ret.minor = util::narrow<unsigned>(minor);


		ret.vendor = reinterpret_cast<char const*>(aGL->getString( GL::VENDOR ));
		ret.renderer = reinterpret_cast<char const*>(aGL->getString( GL::RENDERER ));
		ret.osapi = "GLX";

		GL::Int flags;
		aGL->getIntegerv( GL::CONTEXT_FLAGS, &flags );
		
		ret.forwardCompatible = !!(flags & GL::CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT);
		ret.debug = !!(flags & GL::CONTEXT_FLAG_DEBUG_BIT);
		ret.noError = !!(flags & GL::CONTEXT_FLAG_NO_ERROR_BIT_KHR);
		
		ret.direct = !!aGLX->isDirect( aDisplay, aContext );

		FLUX_GL_CHECKPOINT_ALWAYS();
		return ret;
	}
}

//--    $ make_share_()                     ///{{{2///////////////////////////
namespace 
{
	unique_ptr<Context> make_share_( Caps const& aCaps, dlapi::gl::GLapi const* aGL, dlapi::os::X11api const* aX11, dlapi::os::GLXapi const* aGLX, dlapi::os::X11::Display* aDisplay, dlapi::os::GLX::Drawable aDrawable, dlapi::os::GLX::Context aContext )
	{
		namespace GLX_ = dlapi::os::GLX;
		namespace X11_ = dlapi::os::X11;

		// Fetch information required to create a context.
		int fbid = 0;
		if( 0 != aGLX->queryContext( aDisplay, aContext, GLX_::FBCONFIG_ID, &fbid ) )
		{
			FLUX_THROW( error::ContextError )
				<< einfo::ErrorMeta( "glXQueryContext(GLX_FBCONFIG_ID)" )
			;
		}

		int rtype = 0;
		if( 0 != aGLX->queryContext( aDisplay, aContext, GLX_::RENDER_TYPE, &rtype ) )
		{
			FLUX_THROW( error::ContextError )
				<< einfo::ErrorMeta( "glXQueryContext(GLX_RENDER_TYPE)" )
			;
		}
			
		int screen = 0;
		if( 0 != aGLX->queryContext( aDisplay, aContext, GLX_::SCREEN, &screen ) )
		{
			FLUX_THROW( error::ContextError )
				<< einfo::ErrorMeta( "glXQueryContext(GLX_SCREEN)" )
			;
		}

		int confCount = 1;
		int attr[] = { GLX_::FBCONFIG_ID, int(fbid), X11_::None, X11_::None };
		auto fbconf = aGLX->chooseFBConfig( aDisplay, screen, attr, &confCount );
		FLUX_THROW_IF( !fbconf || confCount < 1, error::ContextError )
			<< einfo::ErrorMeta( "glXChooseFBConfig() returned null/no valid configs" )
		;

		auto const direct = aGLX->isDirect( aDisplay, aContext );

		// attempt to create the new context
		if( !aGLX->ARB_create_context )
			load_glx_extensions( aGLX );

		unique_ptr<Context> ret;
		if( aGLX->ARB_create_context )
		{
			int flags = 0;
			if( aCaps.debug ) flags |= GLX_::CONTEXT_DEBUG_BIT_ARB;
			if( aCaps.forwardCompatible ) flags |= GLX_::CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
			//TODO: aCaps.noError stuff
			
			int attr[] = {
				GLX_::CONTEXT_MAJOR_VERSION_ARB, util::narrow<int>(aCaps.major),
				GLX_::CONTEXT_MINOR_VERSION_ARB, util::narrow<int>(aCaps.minor),
				GLX_::CONTEXT_FLAGS_ARB, flags,
				0, 0
			};

			if( auto cx = aGLX->createContextAttribsARB( aDisplay, fbconf[0], aContext, direct, attr ) )
			{
				ret = make_unique<SharedContext_>( aGL, aX11, aGLX, cx, aDrawable, aDisplay );
			}
		}
		else
		{
			if( auto cx = aGLX->createNewContext( aDisplay, fbconf[0], rtype, aContext, direct ) )
			{
				ret = make_unique<SharedContext_>( aGL, aX11, aGLX, cx, aDrawable, aDisplay );
			}
		}

		aX11->free( fbconf );
		return ret;
	}
}

//--    <<< ~ flux::gl::detail namespace <<<        ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(gl,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
