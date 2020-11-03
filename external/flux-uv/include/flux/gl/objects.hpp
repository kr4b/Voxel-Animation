/*-******************************************************* -- HEADER -{{{1- */
/*-	RAII Wrapper for select GL objects
 *
 * EXPERIMENTAL!
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_GL_OBJECTS_HPP_1157BE24_BB0D_4879_912D_033452AC7609
#define FLUX_GL_OBJECTS_HPP_1157BE24_BB0D_4879_912D_033452AC7609

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>
#include <flux/gl/glapi.hpp>

#include <flux/compat/constexpr.hpp>
#include <flux/std/static_assert.hpp>

//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    aspects                             ///{{{2///////////////////////////

namespace aspect
{
	void delete_fbo( GL::UInt ) noexcept;
	void delete_rbo( GL::UInt ) noexcept;
	void delete_vao( GL::UInt ) noexcept;
	void delete_query( GL::UInt ) noexcept;
	void delete_buffer( GL::UInt ) noexcept;
	void delete_shader( GL::UInt ) noexcept;
	void delete_program( GL::UInt ) noexcept;
	void delete_sampler( GL::UInt ) noexcept;
	void delete_texture( GL::UInt ) noexcept;

	//void delete_memoryEXT( GL::UInt ) noexcept; //TODO

	FLUX_CONSTEXPR_EX
	void delete_nothing( GL::UInt ) noexcept {}
}

//--    UniqueObj                                ///{{{2///////////////////////////

/** Wrapper: OpenGL Object 
 *
 * Simple wrapper around OpenGL objects. Similar to `unique_ptr<>`, the wrapper
 * takes ownership of an object once assigned to it, and deallocates the object
 * when the wrapper instance is destroyed. `UniqueObj<>` is move-only. Objects
 * are deallocated using the \a tDeleter function template argument. \a
 * tDeleter is only called if `id` is non-zero.
 *
 * \warning `UniqueObj<>` requires that, at the time its destructore is called,
 * both the correct OpenGL context and the correct `GLapi` is bound to the
 * thread in which the destructor runs. For example, the user must ensure that
 * the OpenGL context/API is not destroyed/unloaded before the resources
 * associated with. Extreme caution should be used if/when dealing with
 * multiple OpenGL contexts!
 */
template< void tDeleter( GL::UInt ) noexcept >
struct UniqueObj
{
	GL::UInt id = 0;

	~UniqueObj();
	UniqueObj() noexcept = default;

	FLUX_CONSTEXPR_EX
	explicit UniqueObj( GL::UInt ) noexcept;

	UniqueObj( UniqueObj const& ) = delete;
	UniqueObj& operator= (UniqueObj const&) = delete;

	FLUX_CONSTEXPR_EX UniqueObj( UniqueObj&& ) noexcept;
	FLUX_CONSTEXPR_EX UniqueObj& operator= (UniqueObj&&) noexcept;

	// Explicit or not?
	FLUX_CONSTEXPR_EX operator GL::UInt&() noexcept;
	FLUX_CONSTEXPR_EX operator GL::UInt const&() const noexcept;

	// The following is really dubious too:
	FLUX_CONSTEXPR_EX GL::UInt* operator&() noexcept;
	FLUX_CONSTEXPR_EX GL::UInt const* operator&() const noexcept;

	// Helpers:
	void reset();

	FLUX_CONSTEXPR_EX bool valid() const noexcept;
	FLUX_CONSTEXPR_EX GL::UInt yield() noexcept;
};

FLUX_STATIC_ASSERT( sizeof(UniqueObj< aspect::delete_nothing >) == sizeof(GL::UInt) );
FLUX_STATIC_ASSERT( alignof(UniqueObj< aspect::delete_nothing >) == alignof(GL::UInt) );

//--    aliases                             ///{{{2///////////////////////////

using Fbo = UniqueObj< aspect::delete_fbo >;
using Rbo = UniqueObj< aspect::delete_rbo >;
using Vao = UniqueObj< aspect::delete_vao >;
using Query = UniqueObj< aspect::delete_query >;
using Buffer = UniqueObj< aspect::delete_buffer >;
using Shader = UniqueObj< aspect::delete_shader >;
using Program = UniqueObj< aspect::delete_program >;
using Sampler = UniqueObj< aspect::delete_sampler >;
using Texture = UniqueObj< aspect::delete_texture >;

//using MemoryEXT = UniqueObj< aspect::delete_memoryEXT >; //TODO

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/objects.inl"
#endif // FLUX_GL_OBJECTS_HPP_1157BE24_BB0D_4879_912D_033452AC7609
