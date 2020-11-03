/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	flux-dlapi_gl loader
 *
 * WARNING: auto-generated file! do not change!
 */
/*-***************************************************************** -}}}1- */

// generated on 2020-08-17 12:18


//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux_dlapi/GL/gl.hpp>
#include <flux_dlapi/GL/load.hpp>

#include <cassert>
#include <cstring>

#include "gl_load.hxx"

//--    >>> namespace = dlapi::gl >>>               ///{{{1///////////////////
FLUX_NAMESPACE_ENTER2(dlapi,gl)
//--    unload_gl()                     ///{{{2///////////////////////////////
void unload_gl( GLapi const* aGL )
{
	assert( aGL );

	GLapi* mptr = const_cast<GLapi*>(aGL);
	mptr->valid = false;
	
	release_gl( aGL );
}

//--    acquire_gl()                    ///{{{2///////////////////////////////
void acquire_gl( GLapi const* aGL )
{
	assert( aGL );
	++const_cast<GLapi*>(aGL)->referenceCount_;
}

//--    release_gl()                    ///{{{2///////////////////////////////
void release_gl( GLapi const* aGL )
{
	assert( aGL );
	if( 0 == --const_cast<GLapi*>(aGL)->referenceCount_ )
		delete aGL;
}

//--    load_gl()                       ///{{{2///////////////////////////////
GLapi const* load_gl( bool (*aCheckExt)(char const*,void*), void* (*aGetFn)(char const*,void*), void* aUser, int aMajor, int aMinor, char const* aExtList )
{
	assert( aCheckExt );
	assert( aGetFn );

	GLapi* gl = new GLapi();
	assert( gl );

	detail::LoaderArg_ arg;
	arg.checkExt = aCheckExt;
	arg.getFn = aGetFn;
	arg.user = aUser;
	arg.major = aMajor >= 0 ? aMajor : 999;
	arg.minor = aMinor >= 0 ? aMinor : 999;
	arg.extList = aExtList;

	detail::load_gl_core1_( *gl, arg );
	detail::load_gl_core2_( *gl, arg );
	detail::load_gl_core3_( *gl, arg );
	detail::load_gl_core4_( *gl, arg );
	detail::load_gl_ext_( *gl, arg );
	detail::load_gl_nv_( *gl, arg );
	detail::load_gl_amd_( *gl, arg );
	detail::load_gl_arb_( *gl, arg );
	detail::load_gl_other_( *gl, arg );

	gl->valid = true;
	gl->referenceCount_ = 1;
	return gl;
}

//--    <<< ~ dlapi::gl namespace <<<               ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE2(dlapi,gl)
//--//////////////////////////////////////////////////////////////////////////
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
