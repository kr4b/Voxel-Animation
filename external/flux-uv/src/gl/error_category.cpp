/*-******************************************** -- <INTERNAL> SOURCE -{{{1- */
/*-	OpenGL error category implementation
 */
/*-***************************************************************** -}}}1- */

#include <flux/gl/error_category.hpp>
#include <flux/gl/glapi.hpp>

#include <flux/std/format.hpp>

#include <flux/compat/system_error.hpp>

//--//////////////////////////////////////////////////////////////////////////
//--    >>> namespace = flux::gl >>>                ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(gl)
//--    $ local                             ///{{{2///////////////////////////
namespace
{
	struct GLECat_ final : compat::error_category
	{
		char const* name() const noexcept override;
		std::string message( int ) const override;
		compat::error_condition default_error_condition( int ) const noexcept override;
	};
}

//--    make_gl_error()                     ///{{{2///////////////////////////
compat::error_code make_gl_error( int aErrNum ) noexcept
{
	return compat::error_code( aErrNum, gl_error_category() );
}

//--    gl_error_category()                 ///{{{2///////////////////////////
compat::error_category const& gl_error_category() noexcept
{
	static GLECat_ cat;
	return cat;
}

//--    $ GLECat_                           ///{{{2///////////////////////////
namespace
{
	char const* GLECat_::name() const noexcept
	{
		return "OpenGL";
	}

	std::string GLECat_::message( int aValue ) const
	{
		switch( aValue )
		{
			case GL::GLNO_ERROR: return "no error";

			case GL::INVALID_ENUM: return "invalid enum";
			case GL::INVALID_VALUE: return "invalid value";
			case GL::INVALID_OPERATION: return "invalid $¡@$¥ operation";
			case GL::INVALID_FRAMEBUFFER_OPERATION: return "invalid framebuffer operation";

			case GL::OUT_OF_MEMORY: return "out of memory";
		}

		return format( "unknown GL error {:d}", aValue );
	}

	compat::error_condition GLECat_::default_error_condition( int aValue ) const noexcept
	{
		switch( aValue )
		{
			case GL::GLNO_ERROR: 
				return compat::error_condition( 0, compat::generic_category() );

			case GL::INVALID_ENUM:
			case GL::INVALID_VALUE:
				return compat::error_condition( compat::errc::invalid_argument );

			case GL::INVALID_OPERATION:
			case GL::INVALID_FRAMEBUFFER_OPERATION:
				return compat::error_condition( compat::errc::operation_not_supported );

			case GL::OUT_OF_MEMORY:
				return compat::error_condition( compat::errc::not_enough_memory );
		}

		return compat::error_condition( aValue, *this );
	}
}

//--    <<< ~ flux::gl namespace <<<                ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(gl)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
