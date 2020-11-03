#ifndef DETAIL_GL_LOAD_HXX_10578ED1_01E6_4B32_8EF7_DDA701FD644F
#define DETAIL_GL_LOAD_HXX_10578ED1_01E6_4B32_8EF7_DDA701FD644F


//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux_dlapi/GL/forward.hpp>

//--    >>> namespace = dlapi::gl::detail >>>       ///{{{1///////////////////
FLUX_NAMESPACE_ENTER3(dlapi,gl,detail)
//--    types                               ///{{{2///////////////////////////
struct LoaderArg_
{
	bool (*checkExt)(char const*,void*);
	void* (*getFn)(char const*,void*);
	void* user;
	int major;
	int minor;
	char const* extList;
};

//--    functions                           ///{{{2///////////////////////////

	// Main loader functions
void load_gl_core1_( GLapi&, LoaderArg_ const& );
void load_gl_core2_( GLapi&, LoaderArg_ const& );
void load_gl_core3_( GLapi&, LoaderArg_ const& );
void load_gl_core4_( GLapi&, LoaderArg_ const& );
void load_gl_ext_( GLapi&, LoaderArg_ const& );
void load_gl_nv_( GLapi&, LoaderArg_ const& );
void load_gl_amd_( GLapi&, LoaderArg_ const& );
void load_gl_arb_( GLapi&, LoaderArg_ const& );
void load_gl_other_( GLapi&, LoaderArg_ const& );

	// Helper:
inline bool listed_( char const* aList, char const* aItem )
{
	if( !aList ) return true;

	if( char const* found = strstr( aList, aItem ) )
	{
		size_t len = strlen(aItem);
		return found[len] == ' ' || found[len] == '\0';
	}

	return false;
}

//--    <<< ~ dlapi::gl::detail namespace <<<        //{{{1///////////////////
FLUX_NAMESPACE_LEAVE3(dlapi,gl,detail)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // DETAIL_GL_LOAD_HXX_10578ED1_01E6_4B32_8EF7_DDA701FD644F
