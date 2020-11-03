/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*- GLapi inline implementation
 */
/*-***************************************************************** -}}}1- */

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    gl_api()                            ///{{{2///////////////////////////
inline
GLapi const* gl_api() noexcept
{
	return detail::ThreadGLApi::instance().api;
}

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
