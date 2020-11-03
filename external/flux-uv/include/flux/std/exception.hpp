/*-******************************************************* -- HEADER -{{{1- */
/*-	Standard flux exceptions.
 *
 * Based somewhat on Boost.Exception and N3758 -- arbitrary info can be 
 * attached to the flux Exception. See `except/einfo.hpp` for additional
 * information.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_EXCEPTION_HPP_D7C6452E_1188_4279_AEB7_216E46D7E1D8
#define FLUX_STD_EXCEPTION_HPP_D7C6452E_1188_4279_AEB7_216E46D7E1D8

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/std/enable_if.hpp>
#include <flux/std/shared_ptr.hpp>

#include <flux/compat/type_traits.hpp>

#include "except/forward.hpp"

//--    >>> namespace = flux::error >>>             ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(error)
//--    macros                              ///{{{2///////////////////////////

#define FLUX_ENABLE_EXCEPTION_INFO(...) ::flux::error::detail::enable_einfo(__VA_ARGS__)

//--    Exception                           ///{{{2///////////////////////////
class Exception
{
	public:
		Exception() noexcept;
		virtual ~Exception() noexcept = 0;

	protected:
		void ensure_edata_() const;

	protected:
		mutable shared_ptr<detail::ExceptData> mData;

	friend detail::ExceptData& detail::exception_data( Exception const& );
};

//--    d: ExceptAmalgamation               ///{{{2///////////////////////////
namespace detail
{
	template< class tOriginal >
	struct ExceptAmalgamation
		: tOriginal
		, virtual error::Exception
	{
		explicit ExceptAmalgamation( tOriginal&& ) noexcept(std::is_nothrow_move_constructible<tOriginal>::value);
		explicit ExceptAmalgamation( tOriginal const& ) noexcept(std::is_nothrow_copy_constructible<tOriginal>::value);

		ExceptAmalgamation( ExceptAmalgamation const& ) = default;
		ExceptAmalgamation& operator= (ExceptAmalgamation const&) = default;

		ExceptAmalgamation( ExceptAmalgamation&& ) = default;
		ExceptAmalgamation& operator= (ExceptAmalgamation&&) = default;
	};
}

//--    d: enable_einfo()                   ///{{{2///////////////////////////
namespace detail
{
	template< class tExcept > 
	FLUX_ENABLE_IF( std::is_base_of<Exception,compat::remove_reference_t<tExcept>> )
	(tExcept&&) enable_einfo( tExcept&& ) noexcept;

	//TODO: make conditional noexcept()
	template< class tExcept >
	FLUX_DISABLE_IF( std::is_base_of<Exception,compat::remove_reference_t<tExcept>> )
	(ExceptAmalgamation<compat::decay_t<tExcept>>) enable_einfo( tExcept&& );
}

//--    <<< ~ flux::error namespace <<<             ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(error)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#include "detail/exception.inl"
#endif // FLUX_STD_EXCEPTION_HPP_D7C6452E_1188_4279_AEB7_216E46D7E1D8
