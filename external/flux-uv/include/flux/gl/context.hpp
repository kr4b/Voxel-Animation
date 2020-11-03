/*-******************************************************* -- HEADER -{{{1- */
/*-	GL Context Wrapper
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_GL_CONTEXT_HPP_EB8A60E8_A825_4DC6_9F29_7C4C77AAF32C
#define FLUX_GL_CONTEXT_HPP_EB8A60E8_A825_4DC6_9F29_7C4C77AAF32C

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/ftl/any.hpp>
#include <flux/ftl/catalog_key.hpp>

#include <flux/std/unique_ptr.hpp>

#include <flux/base/default_catalogs.hpp>

#include <flux_dlapi/GL/forward.hpp>

//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    Caps                                ///{{{2///////////////////////////

struct Caps
{
	unsigned major, minor;

	std::string vendor;
	std::string renderer;
	std::string osapi;

	bool forwardCompatible;
	bool debug;
	bool noError;

	bool direct;
};

//--    Context                             ///{{{2///////////////////////////

struct Context
{
	virtual ~Context() = 0;

	virtual bool valid() = 0;
	virtual bool owned() = 0;
	virtual bool bound() = 0;

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual Caps caps() = 0;
	virtual dlapi::gl::GLapi const* api() = 0;

	virtual ftl::Any native_api() = 0;
	virtual ftl::Any native_handle() = 0;

	virtual unique_ptr<Context> create_extra_context() = 0;
};

//--    functions                           ///{{{2///////////////////////////

// TODO: docs :: note re Catalog and unique_ptr.
unique_ptr<Context> create_from_native_current(
	bool aBindToThreadCatalog = true,
	bool aBindGLApi = true
);

//--    catalog                             ///{{{2///////////////////////////

namespace detail
{

}

using ThreadContext = ftl::CatalogKey<
	Context*,
	Context*,
	struct ThreadContextKeyTag_
>;

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_GL_CONTEXT_HPP_EB8A60E8_A825_4DC6_9F29_7C4C77AAF32C
