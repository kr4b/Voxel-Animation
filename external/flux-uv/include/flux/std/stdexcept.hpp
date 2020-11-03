/*-******************************************************* -- HEADER -{{{1- */
/*-	Standard flux sub-exceptions.
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_STD_STDEXCEPT_HPP_DEF27F30_57EE_490F_8B45_DCE25E81C2A0
#define FLUX_STD_STDEXCEPT_HPP_DEF27F30_57EE_490F_8B45_DCE25E81C2A0

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <flux/compat/system_error.hpp>

#include <stdexcept>

#include "exception.hpp"

//--    >>> namespace = flux::error >>>             ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(error)
//--    exceptions                          ///{{{2///////////////////////////
namespace detail
{
	// wrap runtime_error & logic_error to give them a default constructor
	// what() is overridden anyway, so the message gets lost either way; the
	// reason to use these is mostly that it's nice to integrate with the 
	// standard exceptions.
	struct runtime_error : std::runtime_error
	{
		runtime_error();
	};
	struct logic_error : std::logic_error
	{
		logic_error();
	};

	// helper: set einfo::ApiError on exception
	void set_api_error( Exception const&, compat::error_code&& ) noexcept;
}

/* Base class for recoverable errors
 *
 * All errors from which recovery is (theoretically) possible derive from
 * `Error`.
 *
 * \note `Error` cannot be thrown directly.
 *
 * \see RuntimeError, LogicError
 */
struct Error : Exception
{
	// Prevent throwing Error directly.
	virtual ~Error() noexcept = 0;
};

	/** Runtime error
	 *
	 * Runtime errors should be used to signal errors that cannot be 
	 * reasonably prevented, but that are still recoverable. An example of
	 * this is e.g., attempting to open a file that does not exist (any
	 * longer).
	 */
	struct RuntimeError : Error, detail::runtime_error
	{
		char const* what() const noexcept override;
	};

	/** Logic error
	 *
	 * Logic errors indicate errors that are preventable and typically 
	 * arise from a programmer error. Examples include:
	 *   - bad arguments (null pointer instead of object)
	 *   - attempting to write to a file that was opened for reading only
	 */
	struct LogicError : Error, detail::logic_error
	{
		char const* what() const noexcept override;
	};

	/** System error
	 *
	 * `SystemError` represents a specific system error. The error code is
	 * given as a template argument, which enables the user to catch 
	 * a specific system error. Optionally, a base class may be provided;
	 * this defaults to `RuntimeError`.
	 *
	 * \note Experimental. *flux-ultraviolet* includes `SystemError` for
	 * testing purposes -- i.e., to see if this format is useful. Previous
	 * versions used a `std::system_error`-like class, but unfortunately,
	 * handling specific error codes with that mechanism was very painful.
	 *
	 * \note 
	 */
	template< 
		typename tErrorEnum, 
		tErrorEnum tErrorCode, 
		class tBase = RuntimeError
	> 
	struct SystemError : tBase
	{
		SystemError()
		{
			detail::set_api_error( static_cast<Exception const&>(*this), make_error_code( tErrorCode ) );
		}
	};

/* Base class for non-recoverable exceptions
 *
 * A `FatalException` indicates a condition from which the current process
 * cannot reasonably recover. Ever. It is expected that the current process 
 * will terminate shortly after a `FatalException` is thrown.
 *
 * Few code locations should catch `FatalException` - typically, the
 * exception would propagate all the way up to `main()`, where the current
 * process is eventually terminated. (In some cases it might be admissible to 
 * catch a `FatalException` in order to fail faster.)
 */
struct FatalException : Exception, virtual std::exception
{
	char const* what() const noexcept override;
};

/* See
 *  http://blogs.msdn.com/b/ericlippert/archive/2008/09/10/vexing-exceptions.aspx
 *
 * We have:
 *   - Logic     := Boneheaded
 *   - Runtime   := Exogenous
 *   - Fatal     := Fatal
 *
 * We'll avoid `vexing`, in hope that we don't do APIs that need it.
 */

//--    <<< ~ flux::error namespace <<<             ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(error)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_STD_STDEXCEPT_HPP_DEF27F30_57EE_490F_8B45_DCE25E81C2A0
