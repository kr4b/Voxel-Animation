/*-******************************************************* -- HEADER -{{{1- */
/*-	integer_sequence et al. fallback
 */
/*-***************************************************************** -}}}1- */

#ifndef FLUX_NIH_INTEGER_SEQUENCE_HPP_F964FEC1_A684_4B14_9F00_4A718B90C08D
#define FLUX_NIH_INTEGER_SEQUENCE_HPP_F964FEC1_A684_4B14_9F00_4A718B90C08D

//--//////////////////////////////////////////////////////////////////////////
//--    include                     ///{{{1///////////////////////////////////

#include <flux/setup.pkg>

#include <cstddef>

//--    >>> namespace = flux::nih_ >>>              ///{{{1///////////////////
FLUX_NAMESPACE_ENTER1(nih_)
//--    integer_sequence<>          ///{{{1///////////////////////////////////

template< typename tType, tType... tSeq >
struct integer_sequence
{
	using value_type = tType;

	static constexpr
	std::size_t size() noexcept { return sizeof...(tSeq); }
};

//--    index_sequence<>            ///{{{1///////////////////////////////////

template< std::size_t... tSeq >
using index_sequence = integer_sequence< std::size_t, tSeq... >;

//--    make_integer_sequence<>     ///{{{1///////////////////////////////////
namespace detail
{
	template< class, class > struct ConcatSeq_;
	template< typename T, T... tHead, T... tTail >
	struct ConcatSeq_< integer_sequence<T,tHead...>, integer_sequence<T,tTail...> >
	{
		using type = integer_sequence<T,tHead...,(sizeof...(tHead)+tTail)...>;
	};

	template< typename T, std::size_t N > struct MakeSeq_
	{
		using type = typename ConcatSeq_< 
			typename MakeSeq_<T,N/2>::type,
			typename MakeSeq_<T,N-N/2>::type
		>::type;
	};
	
	template< typename T > 
	struct MakeSeq_<T,0> { using type = integer_sequence<T>; };
	template< typename T > 
	struct MakeSeq_<T,1> { using type = integer_sequence<T,0>; };
	template< typename T > 
	struct MakeSeq_<T,2> { using type = integer_sequence<T,0,1>; };
	template< typename T > 
	struct MakeSeq_<T,3> { using type = integer_sequence<T,0,1,2>; };
	template< typename T > 
	struct MakeSeq_<T,4> { using type = integer_sequence<T,0,1,2,3>; };
	template< typename T > 
	struct MakeSeq_<T,5> { using type = integer_sequence<T,0,1,2,3,4>; };
	template< typename T > 
	struct MakeSeq_<T,6> { using type = integer_sequence<T,0,1,2,3,4,5>; };
	template< typename T > 
	struct MakeSeq_<T,7> { using type = integer_sequence<T,0,1,2,3,4,5,6>; };
	template< typename T > 
	struct MakeSeq_<T,8> { using type = integer_sequence<T,0,1,2,3,4,5,6,7>; };
	template< typename T > 
	struct MakeSeq_<T,9> { using type = integer_sequence<T,0,1,2,3,4,5,6,7,8>; };
	template< typename T > 
	struct MakeSeq_<T,10> { using type = integer_sequence<T,0,1,2,3,4,5,6,7,8,9>; };
}

template< typename tType, tType tValue >
using make_integer_sequence = typename detail::MakeSeq_<tType,tValue>::type;

//--    make_index_sequence<>       ///{{{1///////////////////////////////////

template< std::size_t tValue >
using make_index_sequence = typename detail::MakeSeq_<std::size_t,tValue>::type;

//--    <<< ~ flux::nih_ namespace <<<              ///{{{1///////////////////
FLUX_NAMESPACE_LEAVE1(nih_)
//--///}}}1/////////////// vim:syntax=cpp:foldmethod=marker:ts=4:noexpandtab: 
#endif // FLUX_NIH_INTEGER_SEQUENCE_HPP_F964FEC1_A684_4B14_9F00_4A718B90C08D
